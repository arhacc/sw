#!/bin/bash

set -ex

# Protection for Vlad Serbu enviornement
(
    source /etc/os-release
    if [[ "${ID}" == arch ]]
    then
        read -p 'You are building on arch. Are you sure? [yN] ' RESPONSE
        if [[ "${RESPONSE}" != y && "${RESPONSE}" != Y ]]
        then
            echo Exiting. >&2
            exit 1
        fi
    fi
)

# Get arguments
usage() { echo "Usage: $0 [-p <profile directory>] [-r <cmake release type>]" 1>&2; }

p=debug
r=Debug

CMAKE_EXTRA_ARGS=(-DCMAKE_EXPORT_COMPILE_COMMANDS=1)

while getopts ":v:p:r:MXSFG" o; do
    case "${o}" in
        v)
            v="${OPTARG}"
            if [[ "${v}" != "NONE" && "${v}" != "LOW" && "${v}" != "MEDIUM" && \
                  "${v}" != "HIGH" && "${v}" != "FULL" && "${v}" != "DEBUG" ]]
            then 
              usage
              exit 1
            fi

            CMAKE_EXTRA_ARGS+=("-DXPU_DEBUG_VERBOSITY_LEVEL=XPU_DEBUG_VERBOSITY_LEVEL_${v}")
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

SOURCE_DIR="${XPU_SW_PATH}/tools/xrt"
OUTPUT_DIR="build/${p}"

export CPM_SOURCE_CACHE="${HOME}/.cache/xpu/CPM/${p}/${r}"

# Generate flex/bison files
"${SOURCE_DIR}/src/targets/sim/statelogparser/genfiles.sh"

# Run cmake pre-build
cmake -B "${OUTPUT_DIR}" -S "${SOURCE_DIR}" -G Ninja -DCMAKE_BUILD_TYPE="${r}" "${CMAKE_EXTRA_ARGS[@]}"

# Run cmake build
cmake --build "${OUTPUT_DIR}"

# Patch binary
patchelf --set-rpath '$ORIGIN/../lib' "${OUTPUT_DIR}/bin/xrt"

# Bring in compile commands for neovim/clangd
if [[ "${p}" == debug ]]
then
    cp "${OUTPUT_DIR}/compile_commands.json" "${OUTPUT_DIR}/.."
fi

# Fixes for local system
if [[ ! -p ~/.xpu/logs/simulation_files/print_log_debug_file_function.txt ]]
then
  mkdir -p ~/.xpu/logs/simulation_files
  rm -f ~/.xpu/logs/simulation_files/print_log_debug_file_function.txt
  mkfifo ~/.xpu/logs/simulation_files/print_log_debug_file_function.txt
fi

mkdir -p ~/.xpu/lib/midlevel
cp "${OUTPUT_DIR}/lib/libxrtcore.so" ~/.xpu/lib
if [[ -f lib/libxpumidlevel.so ]]
then
    cp "${OUTPUT_DIR}/lib/libxpumidlevel.so" ~/.xpu/lib/midlevel
fi
