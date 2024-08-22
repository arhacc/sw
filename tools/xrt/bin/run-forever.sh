#!/bin/bash


python3 "${XPU_SW_PATH}/tools/xrt/bin/xrt-reaper.py" &
sleep 3

while true
do
	"${XPU_SW_PATH}/tools/xrt/bin/run.sh" "${@}"
done
