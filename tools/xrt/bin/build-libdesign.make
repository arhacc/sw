#!/usr/bin/env make -f

include $(XPU_HW_PATH)/low_level/xpu/simulator_include/hardware_files.make

$(XPU_HOME)/lib/xsim.dir/simulator_axi/xsimk.so: $(hw_sources)
	$(XPU_SW_PATH)/tools/xrt/bin/build-libdesign-do-compile.sh