#!/bin/zsh
source "./setenv.sh"

$XSIM_ELAB -prj $XPU_HW_FOLDER/low_level/xpu/xpu.sim/sim_1/behav/xsim/simulator_axi_vlog.prj \
xil_defaultlib.xpu xil_defaultlib.glbl -dll -debug all --relax --mt 8 -s $OUT_SIM_SNAPSHOT   \
-i $XPU_HW_FOLDER/low_level/xpu/xpu.srcs/sim_1/imports/new/                                  \
-i $XPU_HW_FOLDER/low_level/xpu/xpu.srcs/sources_1/imports/new                               \
-i $XPU_HW_FOLDER/low_level/xpu/xpu.srcs/sources_1/new                                       \
-L xil_defaultlib -L uvm -L unisims_ver                                                      \
-L unimacro_ver -L secureip -log elaborate.log -stat --nolog

cp -rf xsim.dir ../cmake-build-debug
rm -r xsim.dir
rm xelab.pb