#!/bin/bash -f
#rm -rf ../build
mkdir -p ../build/
cd ../build/

xelab -prj ../src/targets/sim/xsim/simulator.prj xil_defaultlib.xpu \
xil_defaultlib.glbl -dll -debug all --relax --mt 8 \
-L xil_defaultlib -L uvm -L unisims_ver \
-L unimacro_ver -L secureip -log elaborate.log --snapshot xsim
