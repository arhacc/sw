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
c=

CMAKE_EXTRA_ARGS=(-DCMAKE_EXPORT_COMPILE_COMMANDS=1)

while getopts ":v:p:r:c:zMXSFG" o
do
    case "${o}"
    in
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
        c)
            c="${OPTARG}"
            ;;
        z)
            export BUILD_CC=gcc
	    source "${XPU_CROSS_COMPILER_ENV:-${XPU_HW_PATH}/petalinux/zynq_xpu_w_scatter_gather/images/linux/sdk/environment-setup-cortexa9t2hf-neon-xilinx-linux-gnueabi}"

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
OUTPUT_DIR="${XPU_SW_PATH}/tools/xrt/build/${p}"

export CPM_SOURCE_CACHE="${OUTPUT_DIR}/CPM"

# Build dependencies
bash "${SOURCE_DIR}/bin/build-deps.sh" -p "${p}"
export PKG_CONFIG_PATH="${OUTPUT_DIR}/deps-prefix/lib/pkgconfig"
CMAKE_EXTRA_ARGS+=('-DPKG_CONFIG_ARGN=--env-only')

# Generate flex/bison files
"${SOURCE_DIR}/src/targets/sim/statelogparser/genfiles.sh"

# Run cmake pre-build
cmake -B "${OUTPUT_DIR}/xrt" -S "${SOURCE_DIR}" -G Ninja -DCMAKE_BUILD_TYPE="${r}" "${CMAKE_EXTRA_ARGS[@]}"

# Bring in compile commands for neovim/clangd
if [[ "${p}" == debug ]]
then
    cp "${OUTPUT_DIR}/xrt/compile_commands.json" "${OUTPUT_DIR}/.."
fi

# Run cmake build
cmake --build "${OUTPUT_DIR}/xrt"

# Patch binary
patchelf --set-rpath '$ORIGIN/../lib' "${OUTPUT_DIR}/xrt/bin/xrt"


# Fixes for local system
if [[ ! -p ~/.xpu/logs/simulation_files/print_log_debug_file_function.txt ]]
then
  mkdir -p ~/.xpu/logs/simulation_files
  rm -f ~/.xpu/logs/simulation_files/print_log_debug_file_function.txt
  mkfifo ~/.xpu/logs/simulation_files/print_log_debug_file_function.txt
fi

mkdir -p ~/.xpu/lib/midlevel
cp "${OUTPUT_DIR}/xrt/lib/libxrtcore.so" ~/.xpu/lib
if [[ -f "${OUTPUT_DIR}/xrt/lib/libxpumidlevel.so" ]]
then
    cp "${OUTPUT_DIR}/xrt/lib/libxpumidlevel.so" ~/.xpu/lib/midlevel
fi
