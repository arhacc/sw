#!/bin/bash

set -e

if [[ ! -z "${XPU_SW_PATH}" ]]
then
    cd "${XPU_SW_PATH}/tools/xrt"
fi

if [[ ! -d build ]]
then
    echo "run build.sh first" > /dev/stderr
fi

echo "./build/debug/xrt/bin/xrt" "$@"
./build/debug/xrt/bin/xrt "$@"
