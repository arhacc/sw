#!/bin/bash

set -e

if [[ $# -ne 1 ]]
then
  echo "script takes one argument: path to ship to"
fi

export XPU_HOME="${1}"

mkdir -p "${XPU_HOME}"/{bin,lib/designs,lib/lowlevel,lib/midlevel,tmp/cache,tmp/onnx2json,logs/simulation_files,etc/architecture_implementations}

# 1. Install xrt and midlevel
# ===========================

"${XPU_SW_PATH}/tools/xrt/bin/build.sh"

cp "${XPU_SW_PATH}/tools/xrt/build/bin/xrt" "${XPU_HOME}/bin/xrt"
patchelf --set-rpath '$ORIGIN/../lib' "${XPU_HOME}/bin/xrt"
cp "${XPU_SW_PATH}/tools/xrt/build/lib/libxrtcore.so" "${XPU_HOME}/lib/libxrtcore.so"
cp "${XPU_SW_PATH}/tools/xrt/build/lib/libxpumidlevel.so" "${XPU_HOME}/lib/midlevel/libxpumidlevel.so"

export SEARCHINV="$(patchelf --print-rpath "${XPU_SW_PATH}/tools/xrt/build/bin/xrt" | sed -E 's/:\$ORIGIN[^:]*:?//g')"

IFS=: read -r -d '' -a SEARCHIN < <(printf '%s:\0' "${SEARCHINV}")

for i in "${SEARCHIN[@]}"
do
  if [[ -f "${i}/libtbbmalloc_proxy_debug.so.2" ]]
  then
    cp "${i}/libtbbmalloc_proxy_debug.so.2" "${XPU_HOME}/lib/libtbbmalloc_proxy_debug.so.2"
    patchelf --set-rpath '$ORIGIN' "${XPU_HOME}/lib/libtbbmalloc_proxy_debug.so.2"
  fi

  if [[ -f "${i}/libtbbmalloc_debug.so.2" ]]
  then
    cp "${i}/libtbbmalloc_debug.so.2" "${XPU_HOME}/lib/libtbbmalloc_debug.so.2"
  fi
done

# 2. Install sdk and sdk-libs
# ===========================

cd "${ARHACC_PATH}/sdk-libs/bin"
./install
cd -

cd "${XPU_SW_PATH}/tools/sdk/bin"
./install
cd -

echo $XPU_HOME

cat >"${XPU_HOME}/bin/sdk" <<'EOF'
#!/bin/bash
#-----------------------------------------------------------------------------------

export XPU_LIB="$XPU_HOME/lib"
export CLASSPATH="$XPU_LIB/*:\
$CLASSPATH"

if [[ "$*" == *-prf* ]] ; then
    export JAVA_PROFILING_OPTIONS="-agentpath:/Applications/JProfiler.app/Contents/Resources/app/bin/macos/libjprofilerti.jnilib=port=8849"
fi

export JAVA_ACCESS_OPTIONS="--add-opens java.base/java.nio=ALL-UNNAMED --add-opens java.base/jdk.internal.misc=ALL-UNNAMED -Dio.netty.tryReflectionSe
tAccessible=true"

java \
        $JAVA_PROFILING_OPTIONS \
        $JAVA_ACCESS_OPTIONS \
        -Djava.classpath=. \
        xpu.sw.tools.sdk.Sdk $*
EOF
chmod +x "${XPU_HOME}/bin/sdk"

cat >"${XPU_HOME}/bin/asm" << 'EOF'
#!/bin/bash
#-----------------------------------------------------------------------------------

"${XPU_HOME}/bin/sdk" -cmd asm "$@"
EOF
chmod +x "${XPU_HOME}/bin/asm"

cat >"${XPU_HOME}/bin/rexec" << 'EOF'
#!/bin/bash
#-----------------------------------------------------------------------------------

"${XPU_HOME}/bin/sdk" -cmd rexec "$@"
EOF
chmod +x "${XPU_HOME}/bin/rexec"

cat >"${XPU_HOME}/bin/gui" << 'EOF'
#!/bin/bash
#-----------------------------------------------------------------------------------

"${XPU_HOME}/bin/sdk" -cmd gui "$@"
EOF
chmod +x "${XPU_HOME}/bin/gui"

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
  "${XPU_SW_PATH}/tools/xrt/bin/build-libdesign.sh" -a "${ARCH}"
done

