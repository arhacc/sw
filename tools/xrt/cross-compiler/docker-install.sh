#!/bin/sh

cd / &&

mkdir xrt-build-cross-compiler &&
cd xrt-build-cross-compiler &&

apt-get update &&
apt-get upgrade -y &&
apt-get autoremove -y &&

apt-get install -y autotools-dev automake bison build-essential cmake curl file \
    flex gawk help2man libncurses-dev libprotoc-dev libprotobuf-dev libtool-bin \
    ninja-build protobuf-compiler python3 rsync texinfo unzip wget &&

wget http://crosstool-ng.org/download/crosstool-ng/crosstool-ng-1.25.0.tar.xz &&
unxz crosstool-ng-1.25.0.tar.xz &&
tar xf crosstool-ng-1.25.0.tar &&
cd crosstool-ng-1.25.0 &&

./bootstrap &&
./configure --prefix=/usr/local &&
make &&
make install &&

mkdir /root/src &&

mv /ct-ng.config .config &&

CT_EXPERIMENTAL=y CT_ALLOW_BUILD_AS_ROOT=y CT_ALLOW_BUILD_AS_ROOT_SURE=y ct-ng build &&

cd .. &&

export XRT_CROSS_TOOLCHAIN="$HOME/x-tools/arm-unknown-linux-gnueabihf" &&
export XRT_CROSS_CC="$XRT_CROSS_TOOLCHAIN/bin/arm-unknown-linux-gnueabihf-gcc" &&
export XRT_CROSS_CXX="$XRT_CROSS_TOOLCHAIN/bin/arm-unknown-linux-gnueabihf-g++" &&
export XRT_CROSS_PREFIX="$XRT_CROSS_TOOLCHAIN/arm-unknown-linux-gnueabihf" &&

export CXXFLAGS="-Wno-psabi" &&

# fmt

wget https://github.com/fmtlib/fmt/releases/download/10.0.0/fmt-10.0.0.zip &&
unzip fmt-10.0.0.zip &&
cd fmt-10.0.0 &&

cmake \
	-B build \
    -S . \
    -G Ninja \
	-D CMAKE_C_COMPILER="$XRT_CROSS_CC" \
	-D CMAKE_CXX_COMPILER="$XRT_CROSS_CXX" \
	-D CMAKE_INSTALL_PREFIX="$XRT_CROSS_PREFIX" \
    -D CMAKE_LIBRARY_PATH="$XRT_CROSS_PREFIX/lib" \
    -D FMT_TEST=OFF &&

cmake --build build &&
cmake --install build &&

cd .. &&

# ncurses

wget ftp://ftp.gnu.org/gnu/ncurses/ncurses-6.4.tar.gz &&
tar xvfz ncurses-6.4.tar.gz &&
cd ncurses-6.4 &&

CC="$XRT_CROSS_CC" CXX="$XRT_CROSS_CXX" \
    ./configure \
        --prefix="$XRT_CROSS_PREFIX" \
        --host=arm \
        --enable-static \
        --disable-shared \
        --disable-stripping \
        --without-manpages &&

make -j$(nproc) &&
make install &&

cd .. &&

# readline

wget ftp://ftp.gnu.org/gnu/readline/readline-8.2.tar.gz &&
tar xvfz readline-8.2.tar.gz &&
cd readline-8.2 &&

CC="$XRT_CROSS_CC" CXX="$XRT_CROSS_CXX" \
    ./configure \
        --prefix="$XRT_CROSS_PREFIX" \
        --host=arm \
        --enable-static \
        --disable-shared &&

make -j$(nproc) &&
make install &&

cd .. &&

# OpenSSL

wget https://github.com/openssl/openssl/releases/download/openssl-3.1.0/openssl-3.1.0.tar.gz &&
tar xvfz openssl-3.1.0.tar.gz &&
cd openssl-3.1.0 &&

CC="$XRT_CROSS_CC" CXX="$XRT_CROSS_CXX" \
    ./Configure linux-generic32 no-shared \
        --prefix="$XRT_CROSS_PREFIX" \
        --openssldir="$XRT_CROSS_PREFIX" &&
make -j$(nproc) &&
make install &&

cd .. &&

# Protobuf

wget -O protobuf-3.21.12.tar.gz https://github.com/protocolbuffers/protobuf/archive/refs/tags/v3.21.12.tar.gz &&
tar xvfz protobuf-3.21.12.tar.gz &&
cd protobuf-3.21.12 &&

cmake \
	-B build \
    -S . \
    -G Ninja \
	-D CMAKE_C_COMPILER="$XRT_CROSS_CC" \
	-D CMAKE_CXX_COMPILER="$XRT_CROSS_CXX" \
	-D CMAKE_INSTALL_PREFIX="$XRT_CROSS_PREFIX" \
    -D CMAKE_LIBRARY_PATH="$XRT_CROSS_PREFIX/lib" \
    -D CMAKE_CXX_STANDARD=14 \
    -D protobuf_BUILD_TESTS=OFF \
    -D protobuf_BUILD_SHARED_LIBS=OFF &&

cmake --build build &&
cmake --install build &&

cd .. &&

# ONNX

wget -O onnx-1.14.0.tar.gz https://github.com/onnx/onnx/archive/refs/tags/v1.14.0.tar.gz &&
tar xvfz onnx-1.14.0.tar.gz &&
cd onnx-1.14.0 &&

cmake \
	-B build \
    -S . \
    -G Ninja \
	-D CMAKE_C_COMPILER="$XRT_CROSS_CC" \
	-D CMAKE_CXX_COMPILER="$XRT_CROSS_CXX" \
	-D CMAKE_INSTALL_PREFIX="$XRT_CROSS_PREFIX" \
    -D CMAKE_LIBRARY_PATH="$XRT_CROSS_PREFIX/lib" \
    -D ONNX_USE_PROTOBUF_SHARED_LIBS=OFF &&

cmake --build build &&
cmake --install build &&

cd / &&

rm -rf /xrt-build-cross-compiler /root/src /install.sh
