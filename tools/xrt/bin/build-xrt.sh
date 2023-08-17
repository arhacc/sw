#!/bin/bash

. `dirname "$0"`/build-functions.sh &&

check-wd &&
set-variables "$@" &&

if [[ ! -f "${depsdir}/.all-good" ]]
then
	echo "Run ./bin/build-deps.sh first" >&2
	exit 1
fi &&

CC="${zig} cc -target ${target}" CXX="${zig} c++ -target ${target}" \
	cmake \
		-B "${builddir}/xrt/${target}" \
		-S . \
		-G Ninja \
		-D XRT_PROVIDED_DEPS_DIR="${depsdir}" \
		-D CMAKE_EXPORT_COMPILE_COMMANDS=ON &&

cmake --build "${builddir}/xrt/${target}"
