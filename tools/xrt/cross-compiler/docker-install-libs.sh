#!/bin/sh

# Software versions
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


# ncurses

p "Installing ncurses..." &&

wget ftp://ftp.gnu.org/gnu/ncurses/ncurses-${NCURSES_VERSION}.tar.gz &&
tar xfz ncurses-${NCURSES_VERSION}.tar.gz &&
cd ncurses-${NCURSES_VERSION} &&

./configure \
    --prefix="$XRT_CROSS_PREFIX" \
    --host="$XRT_AUTOCONF_HOST" \
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

./configure \
    --prefix="$XRT_CROSS_PREFIX" \
    --host="$XRT_AUTOCONF_HOST" \
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

./Configure "$XRT_OPENSSL_TARGET" no-shared \
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
    -D CMAKE_INSTALL_PREFIX="$XRT_CROSS_PREFIX" \
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
    -D CMAKE_INSTALL_PREFIX="$XRT_CROSS_PREFIX" \
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
make -j$(nproc) &&
make install &&

cd / &&

p "Cleaning up..." &&

rm -rf /xrt-build-cross-compiler /root/src /install-ct-ng.sh /install-libs.sh &&

p "Done."

