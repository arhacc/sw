#!/bin/sh

if ! [ -f CMakeCache.txt ]; then
    echo "Please run this script from the build directory."
    exit 1
fi

if [ -z "$XPU_HOME" ]; then
    XPU_HOME="$HOME/.xpu"
fi

mkdir -p "$XPU_HOME/bin"
mkdir -p "$XPU_HOME/etc/architecture_implementations"
mkdir -p "$XPU_HOME/lib/lowlevel"
mkdir -p "$XPU_HOME/lib/midlevel"
mkdir -p "$XPU_HOME/tmp/build"
mkdir -p "$XPU_HOME/tmp/cache"

cp bin/xrt "$XPU_HOME/bin"
cp lib/libxrtcore.so "$XPU_HOME/lib/midlevel"