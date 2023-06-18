#!/bin/sh

if [ -f CMakeLists.txt ]
then
    SOURCE_DIR=.
else
    if [ -d tools/xrt ] && [ -f tools/xrt/CMakeLists.txt ]
    then
        SOURCE_DIR=tools/xrt
    else
        echo "ERROR: Could not find CMakeLists.txt" >&2
        exit 1
    fi
fi

if ! [ -d cross-build ]
then
    cmake -B cross-build -S "${SOURCE_DIR}" -G Ninja
fi

cmake --build cross-build
