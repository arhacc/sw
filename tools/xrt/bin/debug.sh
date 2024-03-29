#!/bin/bash

echo gdb "./build/bin/xrt" "$@"
gdb --args ./build/bin/xrt "$@"