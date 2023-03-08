#!/bin/bash -f
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
	sudo gdb --args ../build/xrt -source:net 49000 -source:file ../models/simple_conv_000.onnx -source:cmd -target:sim
elif [[ "$OSTYPE" == "darwin"* ]]; then
	sudo lldb ../build/xrt -- -source:net 49000 -source:file ../models/simple_conv_000.onnx -source:cmd -target:sim
elif [[ "$OSTYPE" == "cygwin" ]]; then
        # TBD!
	sudo gdb --args ../build/xrt -source:net 49000 -source:file ../models/simple_conv_000.onnx -source:cmd -target:sim
elif [[ "$OSTYPE" == "win32" ]]; then
        # TBD!
	sudo gdb --args ../build/xrt -source:net 49000 -source:file ../models/simple_conv_000.onnx -source:cmd -target:sim	
fi