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

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:$(patchelf --print-rpath ./build/bin/xrt | sed -E 's/:\$ORIGIN[^:]*:?//g')"
echo "./build/bin/xrt" "$@"
./build/bin/xrt "$@"