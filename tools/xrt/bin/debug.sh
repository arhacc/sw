#!/bin/bash

export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:$(patchelf --print-rpath ./build/bin/xrt | sed -E 's/:\$ORIGIN[^:]*:?//g')"

echo gdb "./build/bin/xrt" "$@"
gdb --args ./build/bin/xrt "$@"