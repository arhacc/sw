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
        echo "Tested Options:" >&2
        echo "    x86_64-linux-gnu     -- Linux PC" >&2
        echo "    arm-linux-gnueabihf  -- Pynq Board" >&2
        echo "    aarch64-macos        -- MacOS (ARM)" >&2
        exit 1
    fi

    
    case "$(uname -s) $(uname -m)" in
        "Linux x86_64")
            zighost="linux-x86_64"
            ;;

        "Linux x86")
            zighost="linux-x86"
            ;;

        "Linux aarch64")
            zighost="linux-aarch64"
            ;;

        "Linux armv7*")
            zighost="linux-armv7a"
            ;;

        "Darwin arm64")
            zighost="macos-aarch64"
            ;;

        "Darwin x86_64")
            zighost="macos-x86_64"
            ;;
    esac
    

    target="$1"
    target_machine="$(echo "${target}" | awk -F- '{print $1}')"
    target_os="$(echo "${target}" | awk -F- '{print $2}')"
    builddir="$(pwd)/cross-build"
    depsdir="${builddir}/deps/${target}"
    tmpdir="${builddir}/tmp-dl"
    wd="$(pwd)"
    protobufdir="${builddir}/local-protobuf"
    zigdir="${builddir}/zig"
    zig="${zigdir}/zig"

    export AR="${zig} ar"
}