#!/bin/bash
#source "./setenv.sh"

set -e

export ARCH=$(cat "$XPU_HW_PATH/architecture_implementations/xpu_default")

while getopts ":a:" o; do
    case "${o}" in
        a)
            export ARCH="${OPTARG}"
            ;;
        *)
            echo "build-libesign.sh: unkown parameter ${o}" 1>&2
            exit 1
            ;;
    esac
done

if [[ -z "$ARCH" ]]
then
	echo "build-libesign.sh: must specify -a paramter with architecture" 1>&2
	exit 1
fi

"$XPU_LIBRARIES_PATH/architecture_parameters_generator.sh" "${ARCH}"

make -f "$XPU_SW_PATH/tools/xrt/bin/build-libdesign.make"
