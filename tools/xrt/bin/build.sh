#!/bin/bash

set -e

# Get arguments
usage() { echo "Usage: $0 [-p <conan profile>] [-r <cmake release type>]" 1>&2; }

p=debug
r=Debug

CMAKE_EXTRA_ARGS=()

while getopts ":v:p:r:MXSFG" o; do
    case "${o}" in
		v)
			VERBOSITY_LEVEL="${OPTARG}"
			if [[ "${VERBOSITY_LEVEL}" != "NONE" && "${VERBOSITY_LEVEL}" != "LOW" && "${VERBOSITY_LEVEL}" != "MEDIUM" && \
				  "${VERBOSITY_LEVEL}" != "HIGH" && "${VERBOSITY_LEVEL}" != "FULL" && "${VERBOSITY_LEVEL}" != "DEBUG" ]]
			then 
				usage
				exit 1
			fi
			;;
        p)
            p="${OPTARG}"
            ;;
        r)
            r="${OPTARG}"
            ;;
        M)
            CMAKE_EXTRA_ARGS+=('-DXRT_SKIP_MIDLEVEL=ON')
            ;;
        X)
            CMAKE_EXTRA_ARGS+=('-DXRT_SKIP_XRT=ON')
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
            exit 1
            ;;
    esac
done
shift $((OPTIND-1))

if [[ ! -z "${XPU_SW_PATH}" ]]
then
    cd "${XPU_SW_PATH}/tools/xrt/"
fi

# Build

"${XPU_SW_PATH}/tools/xrt/src/targets/sim/statelogparser/genfiles.sh"
ln -sf "${XPU_SW_PATH}/tools/xrt/include/targets/sim/statelogparser/Parser.gen.hpp" "${XPU_SW_PATH}/tools/xrt/src/targets/sim/statelogparser/Parser.gen.hpp"


if ! ping -c 1 -q google.com >/dev/null
then
  conan install . --output-folder=build --build=missing --profile="${p}" --no-remote
else
  conan install . --output-folder=build --build=missing --profile="${p}"
fi
cd build
source conanbuild.sh

cmake -B . -S .. -G Ninja ${VERBOSITY_LEVEL:+-DXPU_DEBUG_VERBOSITY_LEVEL=XPU_DEBUG_VERBOSITY_LEVEL_${VERBOSITY_LEVEL}} -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE="${r}" "${CMAKE_EXTRA_ARGS[@]}"

cmake --build .


if [[ ! -p ~/.xpu/logs/simulation_files/print_log_debug_file_function.txt ]]
then
  mkdir -p ~/.xpu/logs/simulation_files
  rm -f ~/.xpu/logs/simulation_files/print_log_debug_file_function.txt
  mkfifo ~/.xpu/logs/simulation_files/print_log_debug_file_function.txt
fi

mkdir -p ~/.xpu/lib/midlevel

cp lib/libxrtcore.so ~/.xpu/lib
if [[ -f lib/libxpumidlevel.so ]]
then
    cp lib/libxpumidlevel.so ~/.xpu/lib/midlevel
fi
