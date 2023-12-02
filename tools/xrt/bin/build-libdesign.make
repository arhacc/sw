#!/usr/bin/env make -f

include ./objlist.make

$(XPU_HOME)/lib/xsim.dir/simulator_axi/xsimk.so: $(hw_sources)
	$(XPU_SW_PATH)/tools/xrt/bin/build-libdesign.sh