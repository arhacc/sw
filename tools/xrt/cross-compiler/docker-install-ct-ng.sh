#!/bin/sh

# Software versions
CTNG_VERSION="1.25.0"

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

cd ..
