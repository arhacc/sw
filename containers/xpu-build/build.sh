#!/bin/bash

set -ex
shopt -s lastpipe

while [[ -n "${1}" ]]
do
    case "${1}"
    in
        -t)
            tests=1
            ;;
        -p)
            p="${2}"
            shift
            ;;
    esac
    shift
done

if [[ -z "${p}" ]]
then
    p=debug
fi

export PATH="${PATH}:${HOME}/.local/bin"

source "${XILINX_VIVADO}/settings64.sh"

mkdir /arhacc
cd /arhacc

git clone --recurse-submodules "https://${GITHUB_USER}:${GITHUB_PASS}@github.com/arhacc/sw"
git clone "https://${GITHUB_USER}:${GITHUB_PASS}@github.com/arhacc/hw"
git clone "https://${GITHUB_USER}:${GITHUB_PASS}@github.com/arhacc/libraries"
git clone "https://${GITHUB_USER}:${GITHUB_PASS}@github.com/arhacc/sdk-libs"

export ARHACC_PATH=/arhacc
export XPU_SW_PATH="${ARHACC_PATH}/sw"
export XPU_HW_PATH="${ARHACC_PATH}/hw"
export XPU_LIBRARIES_PATH="${ARHACC_PATH}/libraries"
export XPU_SDK_LIBS_PATH="${ARHACC_PATH}/sdk-libs"

echo -n "${p}" | readarray -d ':' -t profiles

for i in "${profiles[@]}"
do
    "${XPU_SW_PATH}/tools/xrt/bin/ship.sh" -p "${i}" "${tests:+-t}"
done

if [[ -d /output ]]
then
    cp "${XPU_SW_PATH}/tools/xrt/build"/*.zip /output
fi
