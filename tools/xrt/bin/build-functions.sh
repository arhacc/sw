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
    target=
    version="devel"

    while getopts t:v: x
    do
        case $x in
        t)
            target="$OPTARG"
            ;;
        v)
            version="$OPTARG"
            ;;
        ?)
            printf "Usage: %s: [-t target] [-v version]\n" $0
            ;;
        esac
    done

    
    case "$(uname -s) $(uname -m)" in
        "Linux x86_64")
            zighost="linux-x86_64"
            defaulttarget="x86_64-linux-gnu"
            ;;

        "Linux x86")
            zighost="linux-x86"
            defaulttarget="x86-linux-gnu"
            ;;

        "Linux aarch64")
            zighost="linux-aarch64"
            defaulttarget="aarch64-linux-gnu"
            ;;

        "Linux armv7*")
            zighost="linux-armv7a"
            defaulttarget="NO DEFAULT TARGET FOR ARM 32-bit"
            ;;

        "Darwin arm64")
            zighost="macos-aarch64"
            defaulttarget="aarch64-macos"
            ;;

        "Darwin x86_64")
            zighost="macos-x86_64"
            defaulttarget="x86_64-macos"
            ;;
    esac
    
    if [[ $# -lt 1 ]]
    then
        echo "No target specified. Defaulting to $defaulttarget"
        target="$defaulttarget"
        sleep 1
    else
        target="$1"
    fi
    
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