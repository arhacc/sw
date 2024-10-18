#!/bin/bash

set -ex

p=debug

while getopts ":p:r:" o;
do
    case "${o}" in
        p)
            p="${OPTARG}"
            ;;
        r)
            r="${OPTARG}"
            ;;
    esac
done

export XPU_HOME="${XPU_SW_PATH}/tools/xrt/build/${p}-ship"

mkdir -p "${XPU_HOME}"/{bin,lib/designs,lib/lowlevel,lib/midlevel,tmp/cache,tmp/onnx2json,logs/simulation_files,etc/architecture_implementations}

# 1. Install xrt and midlevel
# ===========================

"${XPU_SW_PATH}/tools/xrt/bin/build.sh" -p "${p}"

cp "${XPU_SW_PATH}/tools/xrt/build/${p}/bin/xrt" "${XPU_HOME}/bin/xrt"
cp "${XPU_SW_PATH}/tools/xrt/build/${p}/lib/libxrtcore.so" "${XPU_HOME}/lib/libxrtcore.so"
cp "${XPU_SW_PATH}/tools/xrt/build/${p}/lib/libxpumidlevel.so" "${XPU_HOME}/lib/midlevel/libxpumidlevel.so"

# 2. Install sdk and sdk-libs
# ===========================

cd "${ARHACC_PATH}/sdk-libs/bin"
./install
cd -

cd "${XPU_SW_PATH}/tools/sdk/bin"
./install
cd -

cp -rv "${XPU_SW_PATH}/tools/xrt/xpu-home-skel"/* "${XPU_HOME}"
chmod +x "${XPU_HOME}/bin"/*

# 3. Install hardware description files
# =====================================

cp -r "${XPU_HW_PATH}/architecture_implementations"/* "${XPU_HOME}/etc/architecture_implementations"
readarray -d '' ARCHS < <(find "${XPU_HW_PATH}/architecture_implementations/" -maxdepth 1 -mindepth 1 ! -name xpu_default -print0 | sed -z 's!.*/!!')

# 4. Install lowlevel libraries
# =============================

for ARCH in "${ARCHS[@]}"
do
  XPU_HOME=~/.xpu "${XPU_LIBRARIES_PATH}/asm.sh" "${ARCH}"
  mkdir -p "${XPU_HOME}/lib/lowlevel/${ARCH}"
  cp "${XPU_LIBRARIES_PATH}/low_level/libraries/${ARCH}/hexes"/*.hex "${XPU_HOME}/lib/lowlevel/${ARCH}"
done

# 5. Install simulator
# ====================

for ARCH in "${ARCHS[@]}"
do
  echo
  "${XPU_SW_PATH}/tools/xrt/bin/build-libdesign.sh" -a "${ARCH}"
done

# 6. Create zip file
# ==================

cd "${XPU_SW_PATH}/tools/xrt/build"
rm -rf .xpu
cp -r "${p}-ship" .xpu
zip -r "${p}-ship.zip" .xpu
rm -rf .xpu
