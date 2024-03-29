#!/bin/bash

LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:$(patchelf --print-rpath ./build/bin/xrt)"
echo "./build/bin/xrt" "$@"
./build/bin/xrt "$@"