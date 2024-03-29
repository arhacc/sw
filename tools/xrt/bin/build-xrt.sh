#!/bin/bash

set -e

# Get arguments
usage() { echo "Usage: $0 [-p <conan profile>] [-r <cmake release type>]" 1>&2; exit 0; }

p=debug
r=Debug

while getopts ":p:r:" o; do
    case "${o}" in
        p)
            p="${OPTARG}"
            ;;
        r)
            r="${OPTARG}"
            ;;
        *)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

# Build

conan install . --output-folder=build --build=missing --profile="${p}"
cd build
source conanbuild.sh

cmake -B . -S .. -G Ninja -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE="${r}"
cmake --build .
