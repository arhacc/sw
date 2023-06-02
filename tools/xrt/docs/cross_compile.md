Cross-compiling XRT
===================

This document describes the process of preparing a cross-compilation toolchain
targeting the development pynq board, and using it to compile xrt. The result
of this process will be a static binary which can be simply copied and run on
the pynq board.

There are 2 ways to achieve this: by using the Docker container (recommended), or by
installing the cross-compilation toolchain locally on your machine.

Using the Docker container
==========================

To use the Docker container you will have to build it from the provided Dockerfile:

```bash
cd $GIT_ROOT/sw/tools/xrt/cross-compiler
sudo docker build -t xrt-cross:0.1.0 .
```

This will install the cross compiler and all required libraries in the docker image,
except for Vivado, which needs to be present on the host machine.

To compile xrt, run the docker image as follows:

```bash
cd $GIT_ROOT/sw/tools/xrt

# Set up cmake directory
sudo docker run --rm -it -v "$PWD":/srcdir -v "$VIVADO_HOME":/vivado \
    xrt-cross:0.1.0 cmake -B cross-build -S . -G Ninja

# Build xrt
sudo docker run --rm -it -v "$PWD":/srcdir -v "$VIVADO_HOME":/vivado \
    xrt-cross:0.1.0 cmake --build cross-build
```

Installing the toolchain locally
================================

Setting up
----------

This process involves fetching and building quite a few libraries so we recommend you
set up a directory just for this. After everything is installed, you can safely
delete this directory.

```bash
mkdir xrt-build-cross-compiler
cd xrt-build-cross-compiler
```

Building the cross-compiler
---------------------------

To build a cross-compilation toolchain, we use the Crosstool-NG toolchain
generator. Crosstool-NG allows for easy and detailed configuration of the
cross-compiler for a specific target architecture or machine. We provide a
configuration file targeting the pynq board.

To start, we need to fetch and install Crosstool-NG:

```bash
# Fetch source
wget http://crosstool-ng.org/download/crosstool-ng/crosstool-ng-1.25.0.tar.xz
unxz crosstool-ng-1.25.0.tar.xz
tar xvf crosstool-ng-1.25.0.tar
cd crosstool-ng-1.25.0

# Build and install
./bootstrap
./configure --prefix=/usr/local # Can be installed anywhere
make
sudo make install

# Set configuration and build toolchain
cp $GIT_ROOT/sw/tools/xrt/cross-compiler/ct-ng.config ./.config
ct-ng build

# Return to parent directory
cd ..
```

The build can take half an hour, or even more. After the build is finished, the
toolchain will be available in `$HOME/x-tools/arm-unknown-linux-gnueabihf/`. We
set some enviornment variables here which we will use later, but you can
reference everything directly and not use the enviornment at all if you do not
want to:

```bash
export XRT_CROSS_TOOLCHAIN="$HOME/x-tools/arm-unknown-linux-gnueabihf"
export XRT_CROSS_CC="$XRT_CROSS_TOOLCHAIN/bin/arm-unknown-linux-gnueabihf-gcc"
export XRT_CROSS_CXX="$XRT_CROSS_TOOLCHAIN/bin/arm-unknown-linux-gnueabihf-g++"
export XRT_CROSS_PREFIX="$XRT_CROSS_TOOLCHAIN/arm-unknown-linux-gnueabihf"
```

We also export the following C++ flag to silence a GCC warning about an ARM32
C++ ABI change: [read more](https://stackoverflow.com/questions/48149323/what-does-the-gcc-warning-project-parameter-passing-for-x-changed-in-gcc-7-1-m).

```bash
export CXXFLAGS="-Wno-psabi"
```

Building the libraries
----------------------

The next step is to build and install the libraries required by XRT. We prefer
using static libraries where possible, for simplicty. The dependency tree for
the libraries is as follows:

```
        ┌───────┐       ┌────┐
        │       ├──────►│fmt │
        │       │       └────┘
        │       │
        │       │       ┌─────────┐    ┌────────┐
        │       ├──────►│readline ├───►│ncurses │
        │       │       └─────────┘    └────────┘
        │  XRT  │
        │       │       ┌────────┐
        │       ├──────►│openssl │
        │       │       └────────┘
        │       │
        │       │       ┌──────┐       ┌─────────┐    ┌───────┐
        │       ├──────►│ONNX  ├──────►│Protobuf ├───►│Abseil │
        └───────┘       └──────┘       └─────────┘    └───────┘
 ```

In addition the following small libraries are included directly in the XRT
source tree, and do not need to be compiled separetly:

* md5-c
* rxterm

and the following libraries are fetched by cmake dynamically at build time
and are compiled by it along with XRT:

* nlohmann_json

### Building fmt

```bash
# Fetch source 
wget https://github.com/fmtlib/fmt/releases/download/10.0.0/fmt-10.0.0.zip
unzip fmt-10.0.0.zip
cd fmt-10.0.0

# Build and install
cmake \
	-B build \
    -S . \
	-D CMAKE_C_COMPILER="$XRT_CROSS_CC" \
	-D CMAKE_CXX_COMPILER="$XRT_CROSS_CXX" \
	-D CMAKE_INSTALL_PREFIX="$XRT_CROSS_PREFIX" \
    -D CMAKE_LIBRARY_PATH="$XRT_CROSS_PREFIX/lib" \
    -D FMT_TEST=OFF

cmake --build build
sudo cmake --install build

# Return to parent directory
cd ..
```

### Building ncurses

```bash
wget ftp://ftp.gnu.org/gnu/ncurses/ncurses-6.4.tar.gz
tar xvfz ncurses-6.4.tar.gz
cd ncurses-6.4

CC="$XRT_CROSS_CC" CXX="$XRT_CROSS_CXX" \
    ./configure \
        --prefix="$XRT_CROSS_PREFIX" \
        --host=arm \
        --enable-static \
        --disable-shared \
        --disable-stripping \
        --without-manpages

make -j$(nproc)
sudo make install

cd ..
```

### Building readline

```bash
wget ftp://ftp.gnu.org/gnu/readline/readline-8.2.tar.gz
tar xvfz readline-8.2.tar.gz
cd readline-8.2

CC="$XRT_CROSS_CC" CXX="$XRT_CROSS_CXX" \
    ./configure \
        --prefix="$XRT_CROSS_PREFIX" \
        --host=arm \
        --enable-static \
        --disable-shared

make -j$(nproc)
sudo make install

cd ..
```

### Building OpenSSL

```bash

wget https://github.com/openssl/openssl/releases/download/openssl-3.1.0/openssl-3.1.0.tar.gz
tar xvfz openssl-3.1.0.tar.gz
cd openssl-3.1.0

CC="$XRT_CROSS_CC" CXX="$XRT_CROSS_CXX" \
    ./Configure linux-generic32 no-shared \
        --prefix="$XRT_CROSS_PREFIX" \
        --openssldir="$XRT_CROSS_PREFIX"
make -j$(nproc)
sudo make install

cd ..
```

### Building Protobuf

Building the ONNX library can be a bit more complicated, as it requires
protobuf both at runtime, and at compile time. To start, you will need to
install protobuf on your machine. On Ubuntu, this is done by

```bash
sudo apt-get install protobuf-compiler libprotobuf-dev libprotoc-dev
```

The next steps depend on which version of protobuf you have installed on your
system. You will need to install and build the same version for
cross-compilation. To check the version you have installed run:

```bash
protoc --version
```

#### Version 3.21.12 and below

```bash
# Replace 3.21.12 with your version if you have a different one
wget -O protobuf-3.21.12.tar.gz https://github.com/protocolbuffers/protobuf/archive/refs/tags/v3.21.12.tar.gz
tar xvfz protobuf-3.21.12.tar.gz
cd protobuf-3.21.12

cmake \
	-B build \
    -S . \
	-D CMAKE_C_COMPILER="$XRT_CROSS_CC" \
	-D CMAKE_CXX_COMPILER="$XRT_CROSS_CXX" \
	-D CMAKE_INSTALL_PREFIX="$XRT_CROSS_PREFIX" \
    -D CMAKE_LIBRARY_PATH="$XRT_CROSS_PREFIX/lib" \
    -D CMAKE_CXX_STANDARD=14 \
    -D protobuf_BUILD_TESTS=OFF

cmake --build build
sudo cmake --install build

cd ..
```

#### Version 3.22.0 or newer

If your distribution ships a more recent version of Protobuf, some extra steps
need to be taken to ensure the protobuf dependency library, Abseil, builds correctly.
Version 22 or newer is also not supported by the ONNX library.

### Building ONNX

```bash

wget -O onnx-1.14.0.tar.gz https://github.com/onnx/onnx/archive/refs/tags/v1.14.0.tar.gz
tar xvfz onnx-1.14.0.tar.gz
cd onnx-1.14.0

cmake \
	-B build \
    -S . \
	-D CMAKE_C_COMPILER="$XRT_CROSS_CC" \
	-D CMAKE_CXX_COMPILER="$XRT_CROSS_CXX" \
	-D CMAKE_INSTALL_PREFIX="$XRT_CROSS_PREFIX" \
    -D CMAKE_LIBRARY_PATH="$XRT_CROSS_PREFIX/lib" \
    -D ONNX_USE_PROTOBUF_SHARED_LIBS=OFF

cmake --build build
sudo cmake --install build

```

Building XRT
------------

```bash
cd $GIT_ROOT/sw/tools/xrt

cmake \
	-B build \
    -S . \
	-D CMAKE_C_COMPILER="$XRT_CROSS_CC" \
	-D CMAKE_CXX_COMPILER="$XRT_CROSS_CXX" \
    -D CMAKE_LIBRARY_PATH="$XRT_CROSS_PREFIX/lib"

cmake --build build
