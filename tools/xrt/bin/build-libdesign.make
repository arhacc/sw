#!/usr/bin/env make -f

include $(XPU_HW_PATH)/architecture_implementations/$(ARCH)/hardware_files.make

$(XPU_HOME)/lib/designs/$(ARCH)/xsim.dir/simulator_axi/xsimk.so: $(hw_sources)
	$(XPU_SW_PATH)/tools/xrt/bin/build-libdesign-do-compile.sh -a $(ARCH)
