#!/bin/bash

p() {
	if [[ -t 1 ]]
	then
		printf '\n\033[35m%s\033[0m\n\n' "$@"
	else
		printf '%s\n' "$@"
	fi
}

have-to-create-dir() {
    dir="$1"

    if [[ ! -f "${dir}/.all-good" ]]
    then
        if [[ -d "${dir}" ]]
        then
            rm -r "${dir}"
        fi &&

        mkdir -p "${dir}" &&

        cd "${dir}" &&

        return 0
    else
        return 1
    fi
    
    exit 1
}

done-creating-dir() {
    if [[ $# -eq 1 ]]
    then
        cd "$1"
    fi &&

    touch .all-good
}

check-wd() {
    if [[ `basename "$(pwd)"` != xrt ]]
    then
        echo "This script needs to be run from the xrt directory (as ./bin/$1)" >&2
        exit 1
    fi
}

set-variables() {
    if [[ $# -ne 1 ]]
    then
        echo "Please provide target triple" >&2
        echo "(you probably want arm-linux-gnueabihf for Pynq or x86_64-linux-gnu)" >&2
        exit 1
    fi

    target="$1"
    simplehost="$(echo "${target}" | grep -oE '^[^-]*')"
    builddir="$(pwd)/cross-build"
    depsdir="${builddir}/deps/${target}"
    tmpdir="${builddir}/tmp-dl"
    wd="$(pwd)"
    protobufdir="${builddir}/local-protobuf"
    zigdir="${builddir}/zig"
    zig="${zigdir}/zig"

    export AR="${zig} ar"
}
