#!/bin/sh

# Software versions
CTNG_VERSION="1.25.0"
DYNCALL_VERSION="1.4"
NCURSES_VERSION="6.4"
READLINE_VERSION="8.2"
OPENSSL_VERSION="3.1.1"
PROTOBUF_VERSION="3.21.12" # DO NOT UPDATE
ONNX_VERSION="1.14.0"


# Pretty print to terminal
p() {
    printf '\n\033[35m%s\033[0m\n\n' "$@"
}

# Basic setup

cd / &&

mkdir xrt-build-cross-compiler &&
cd xrt-build-cross-compiler &&

# Update

p "Updating base system..." &&

apt-get update &&
apt-get upgrade -y &&
apt-get autoremove -y &&

p "Installing dependencies..." &&

apt-get install -y autotools-dev automake bison build-essential cmake curl   \
    file flex gawk git help2man libncurses-dev libprotoc-dev libprotobuf-dev \
    libtool-bin ninja-build protobuf-compiler python3 rsync texinfo unzip    \
    wget &&

p "Installing cross-compiler..." &&

wget http://crosstool-ng.org/download/crosstool-ng/crosstool-ng-${CTNG_VERSION}.tar.xz &&
unxz crosstool-ng-${CTNG_VERSION}.tar.xz &&
tar xf crosstool-ng-${CTNG_VERSION}.tar &&
cd crosstool-ng-${CTNG_VERSION} &&

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

# ncurses

p "Installing ncurses..." &&

wget ftp://ftp.gnu.org/gnu/ncurses/ncurses-${NCURSES_VERSION}.tar.gz &&
tar xfz ncurses-${NCURSES_VERSION}.tar.gz &&
cd ncurses-${NCURSES_VERSION} &&

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

wget ftp://ftp.gnu.org/gnu/readline/readline-${READLINE_VERSION}.tar.gz &&
tar xfz readline-${READLINE_VERSION}.tar.gz &&
cd readline-${READLINE_VERSION} &&

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

p "Installing OpenSSL..." &&

wget https://github.com/openssl/openssl/releases/download/openssl-${OPENSSL_VERSION}/openssl-${OPENSSL_VERSION}.tar.gz &&
tar xfz openssl-${OPENSSL_VERSION}.tar.gz &&
cd openssl-${OPENSSL_VERSION} &&

CC="$XRT_CROSS_CC" CXX="$XRT_CROSS_CXX" \
    ./Configure linux-generic32 no-shared \
        --prefix="$XRT_CROSS_PREFIX" \
        --openssldir="$XRT_CROSS_PREFIX" &&
make -j$(nproc) &&
make install_sw install_ssldirs &&

cd .. &&

# Protobuf

p "Installing Protobuf..." &&

wget -O protobuf-${PROTOBUF_VERSION}.tar.gz https://github.com/protocolbuffers/protobuf/archive/refs/tags/v${PROTOBUF_VERSION}.tar.gz &&
tar xfz protobuf-${PROTOBUF_VERSION}.tar.gz &&
cd protobuf-${PROTOBUF_VERSION} &&

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

p "Installing ONNX..." &&

wget -O onnx-${ONNX_VERSION}.tar.gz https://github.com/onnx/onnx/archive/refs/tags/v${ONNX_VERSION}.tar.gz &&
tar xfz onnx-${ONNX_VERSION}.tar.gz &&
cd onnx-${ONNX_VERSION} &&

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

cd .. &&

# dyncall

p "Installing dyncall..." &&

wget https://www.dyncall.org/r${DYNCALL_VERSION}/dyncall-${DYNCALL_VERSION}.tar.gz &&
tar xfz dyncall-${DYNCALL_VERSION}.tar.gz &&
cd dyncall-${DYNCALL_VERSION} &&

./configure --prefix="$XRT_CROSS_PREFIX" &&
CC="$XRT_CROSS_CC" CXX="$XRT_CROSS_CXX" ASFLAGS="-mcpu=cortex-a9 -mfpu=neon-vfpv3 -mfloat-abi=hard" make -j$(nproc) &&
make install &&

cd / &&

p "Cleaning up..." &&

rm -rf /xrt-build-cross-compiler /root/src /install.sh &&

p "Done."
