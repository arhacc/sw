#!/bin/bash

source "${XPU_SW_PATH}/tools/xrt/bin/build-functions.sh"

check-wd run.sh
set-variables

echo "${builddir}/xrt/${target}/bin/xrt" "$@"
"${builddir}/xrt/${target}/bin/xrt" "$@"
#../build/xrt -source:net 49000 -source:file ../models/simple_conv_000.onnx -source:cmd -target:$1
#../build/xrt -source:file ../models/simple_conv_000.onnx
