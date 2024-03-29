#!/bin/bash

set -e

if [[ ! -z "${XPU_SW_PATH}" ]]
then
    cd "${XPU_SW_PATH}/tools/xrt/"
fi

if [[ ! -d build ]]
then
    echo "run build.sh first" > /dev/stderr
fi

LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:$(patchelf --print-rpath ./build/bin/xrt)"
echo "./build/bin/xrt" "$@"
./build/bin/xrt "$@"