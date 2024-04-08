#!/bin/bash

set -e

# Get arguments
usage() { echo "Usage: $0 [-p <conan profile>] [-r <cmake release type>]" 1>&2; exit 0; }

p=debug
r=Debug

CMAKE_EXTRA_ARGS=()

while getopts ":p:r:MXSFG" o; do
    case "${o}" in
        p)
            p="${OPTARG}"
            ;;
        r)
            r="${OPTARG}"
            ;;
        M)
            CMAKE_EXTRA_ARGS+=('-DXPU_SKIP_MIDLEVEL=ON')
            ;;
        X)
            CMAKE_EXTRA_ARGS+=('-DXPU_SKIP_XRT=ON')
            ;;
        S)
            CMAKE_EXTRA_ARGS+=('-DXPU_TARGET_SIM=OFF')
            ;;
        F)
            CMAKE_EXTRA_ARGS+=('-DXPU_TARGET_FPGA=OFF')
            ;;
        G)
            CMAKE_EXTRA_ARGS+=('-DXPU_TARGET_GOLDEN_MODEL=OFF')
            ;;
        
        *)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

if [[ ! -z "${XPU_SW_PATH}" ]]
then
    cd "${XPU_SW_PATH}/tools/xrt/"
fi

# Build

conan install . --output-folder=build --build=missing --profile="${p}"
cd build
source conanbuild.sh

cmake -B . -S .. -G Ninja -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE="${r}" "${CMAKE_EXTRA_ARGS[@]}"
cmake --build .

mkdir -p ~/.xpu/lib/midlevel

cp lib/libxrtcore.so ~/.xpu/lib
if [[ -f lib/libxpumidlevel.so ]]
then
    lib/libxpumidlevel.so ~/.xpu/lib/midlevel
fi
