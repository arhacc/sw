#!/bin/bash

source "${XPU_SW_PATH}/tools/xrt/bin/build-functions.sh"

check-wd run.sh
set-variables

echo "${builddir}/xrt/${target}/bin/xrt" "$@"
# gdb --args "${builddir}/xrt/${target}/bin/xrt" "$@"
"${builddir}/xrt/${target}/bin/xrt" "$@"