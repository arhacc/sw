#!/bin/bash
#source "./setenv.sh"

set -e

while getopts ":a:" o; do
    case "${o}" in
        a)
            export ARCH="${OPTARG}"
            ;;
        *)
            echo "build-libesign-do-compile.sh: unkown parameter ${o}" 1>&2
            exit 1
            ;;
    esac
done

if [[ -z "$ARCH" ]]
then
	echo "build-libesign-do-compile.sh: must specify -a paramter" 1>&2
	exit 1
fi

mkdir -p "$XPU_HOME/lib/designs/$ARCH"

if [[ -e "$XPU_HOME/lib/designs/$ARCH/xsim.dir" ]]
then
    rm -r "$XPU_HOME/lib/designs/$ARCH/xsim.dir"
fi

xelab -prj "$XPU_HW_PATH/low_level/xpu/simulator_include/xpu_simulator.prj" \
xil_defaultlib.simulator_axi_xrt xil_defaultlib.glbl -dll -debug all --relax --mt 8 -s simulator_axi \
-timescale 1ns/1ps -override_timeunit -override_timeprecision                                \
-i "$XPU_HW_PATH/low_level/xpu/xpu.srcs/sim_1/imports/new"                                   \
-i "$XPU_HW_PATH/low_level/xpu/xpu.srcs/sources_1/imports/new"                               \
-i "$XPU_HW_PATH/low_level/xpu/xpu.srcs/sources_1/new"                                       \
-L xil_defaultlib -L uvm -L unisims_ver -L all                                                      \
-L unimacro_ver -L secureip -log elaborate.log -stats -v 2 

cp -rf xsim.dir "$XPU_HOME/lib/designs/$ARCH/xsim.dir"
rm -r xsim.dir
rm xelab.pb
