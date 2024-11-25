#!/bin/bash

set -ex

export LD_LIBRARY_PATH="${XILINX_VIVADO}/lib/lnx64.o"
source "${HOME}/.sdkman/bin/sdkman-init.sh"
source "${XILINX_VIVADO}/settings64.sh"

export XPU_HOME=/root/.xpu

cd /root
cp "${ZIP_FILE}" .
unzip "${ZIP_FILE}"

source /root/.xpu/bin/test/all_tests.sh
