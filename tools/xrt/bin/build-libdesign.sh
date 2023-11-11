#!/bin/bash
#source "./setenv.sh"

xelab -prj $ARHACC_PATH/sw/tools/xrt/etc/simulator_axi_vlog.prj \
xil_defaultlib.xpu xil_defaultlib.glbl -dll -debug all --relax --mt 8 -s simulator_axi \
-i $ARHACC_PATH/hw/low_level/xpu/xpu.srcs/sim_1/imports/new/                                  \
-i $ARHACC_PATH/hw/low_level/xpu/xpu.srcs/sources_1/imports/new                               \
-i $ARHACC_PATH/hw/low_level/xpu/xpu.srcs/sources_1/new                                       \
-L xil_defaultlib -L uvm -L unisims_ver                                                      \
-L unimacro_ver -L secureip -log elaborate.log -stat --nolog

cp -rf xsim.dir $XPU_HOME/lib
rm -r xsim.dir
rm xelab.pb