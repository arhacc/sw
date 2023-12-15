#!/bin/bash
#source "./setenv.sh"

if [[ -e $XPU_HOME/lib/xsim.dir ]]
then
    rm -r $XPU_HOME/lib/xsim.dir
fi

xelab -prj "$XPU_HW_PATH/low_level/xpu/simulator_include/xpu_simulator.prj" \
xil_defaultlib.simulator_axi_xrt xil_defaultlib.glbl -dll -debug all --relax --mt 8 -s simulator_axi \
-i "$XPU_HW_PATH/low_level/xpu/xpu.srcs/sim_1/imports/new"                                   \
-i "$XPU_HW_PATH/low_level/xpu/xpu.srcs/sources_1/imports/new"                               \
-i "$XPU_HW_PATH/low_level/xpu/xpu.srcs/sources_1/new"                                       \
-L xil_defaultlib -L uvm -L unisims_ver -L all                                                      \
-L unimacro_ver -L secureip -log elaborate.log -stats -v 2 

cp -rf xsim.dir "$XPU_HOME/lib"
rm -r xsim.dir
rm xelab.pb
