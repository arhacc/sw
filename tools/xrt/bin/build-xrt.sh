#!/bin/sh

if [ $# -ne 1 ]
then
        echo "Please provide target triple" >&2
        exit 1
fi

target="$1"
simplehost="$(echo "${target}" | grep -oE '^[^-]*')"
depsdir="$(pwd)/build/deps/${target}"
wd="$(pwd)"
zig="${depsdir}/../zig/zig"

CC="${zig} cc -target ${target}" CXX="${zig} c++ -target ${target}" \
	cmake \
		-B "build/xrt/${target}" \
		-S . \
		-G Ninja \
		-D XRT_PROVIDED_DEPS_DIR="$(pwd)/build/deps/${target}" \
		-D CMAKE_EXPORT_COMPILE_COMMANDS=ON &&

cmake --build "build/xrt/${target}"
