#!/bin/bash

set -ex

# Protection for vlad serbu enviornement
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
usage() { echo "Usage: $0 [-p <conan profile>] [-r <cmake release type>]" 1>&2; }

p=debug

CMAKE_EXTRA_ARGS=()

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

if [[ ! -f "${HOME}/.conan2/profiles/${p}" ]]
then
    echo "Error: can not find profile ${p} in ${HOME}/.conan2" >&2
    usage
    exit 1
fi

if [[ -z "${r}" ]]
then
  r="$(awk -F= '$1 == "build_type" {print $2}' "${HOME}/.conan2/profiles/${p}")"

  if [[ -z "${r}" ]]
  then
      echo "Could not find build_type in ${HOME}/.conan2/profiles/${p}"
      usage
      exit 1
  fi
fi

SOURCE_DIR="${XPU_SW_PATH}/tools/xrt"
OUTPUT_DIR="build/${p}"


export CPM_SOURCE_CACHE="${HOME}/.conan2/CPM/${p}/${r}"

# Generate flex/bison files
"${XPU_SW_PATH}/tools/xrt/src/targets/sim/statelogparser/genfiles.sh"


# Run conan pre-build
cd "${XPU_SW_PATH}/tools/xrt/"
if ! ping -c 1 -q google.com >/dev/null && [[ -d ~/.conan2/p ]]
then
  conan install . --output-folder="${OUTPUT_DIR}" --build=missing --profile="${p}" --no-remote
else
  conan install . --output-folder="${OUTPUT_DIR}" --build=missing --profile="${p}"
fi


# Source conan pre-build
cd "${OUTPUT_DIR}"
source conanbuild.sh

# Run cmake pre-build
cmake -B . -S "${SOURCE_DIR}" -G Ninja -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE="${r}" "${CMAKE_EXTRA_ARGS[@]}"


# Run cmake build
cmake --build .

# Patch binary
patchelf --set-rpath '$ORIGIN/../lib' bin/xrt

# Bring in compile commands for nvim/clangd
if [[ "${p}" == debug ]]
then
    cp compile_commands.json ..
fi

# Fixes for local system
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
