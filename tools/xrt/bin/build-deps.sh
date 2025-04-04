#!/bin/bash

set -ex

NCURSES_VERSION=6.5
READLINE_VERSION=8.2.13
OPENSSL_VERSION=3.4.0

usage() { echo "Usage: $0 [-p <profile directory>]" 1>&2; }

p=Debug

while getopts ":p:" o
do
    case "${o}" in
        p)
            p="${OPTARG}"
            ;;
        *)
            usage
            exit 1
            ;;
    esac
done

OUTPUT_DIR="${XPU_SW_PATH}/tools/xrt/build/${p}"
PREFIX="${OUTPUT_DIR}/deps-prefix"

mkdir -p "${PREFIX}"

mkdir -p "${OUTPUT_DIR}/deps-build"
cd "${OUTPUT_DIR}/deps-build"

# Ncurses
if [[ ! -f "${PREFIX}/lib/pkgconfig/ncurses.pc" ]]
then
    NCURSES_SRC="ncurses-${NCURSES_VERSION}"
    NCURSES_TAR="${NCURSES_SRC}.tar.gz"

    curl -OL "https://ftp.gnu.org/pub/gnu/ncurses/${NCURSES_TAR}"
    tar xvf "${NCURSES_TAR}"
    rm "${NCURSES_TAR}"
    mkdir -p "${NCURSES_SRC}/build"
    pushd "${NCURSES_SRC}/build"

    ../configure                                            \
        --prefix="${PREFIX}"                                \
        --build="$(${BUILD_CC:-${CC:-gcc}} -dumpmachine)" \
        --host="$(${CC:-gcc} -dumpmachine)"               \
        --with-pkg-config-libdir="${PREFIX}/lib/pkgconfig"  \
        --with-strip-program="${STRIP:-strip}"              \
        --enable-pc-files                                   \
        --disable-cxx-bindings                              \
        --disable-widec                                     \
        --without-manpages

    make -j"$(nproc)"
    make DESTDIR="${PREFIX}" install

    popd
fi

# Readline
if [[ ! -f "${PREFIX}/lib/pkgconfig/readline.pc" ]]
then
    READLINE_SRC="readline-${READLINE_VERSION}"
    READLINE_TAR="${READLINE_SRC}.tar.gz"

    curl -OL "https://ftp.gnu.org/pub/gnu/readline/${READLINE_TAR}"
    tar xvf "${READLINE_TAR}"
    rm "${READLINE_TAR}"
    mkdir -p "${READLINE_SRC}/build"
    pushd "${READLINE_SRC}/build"

    ../configure                                     \
        --prefix="${PREFIX}"                         \
        --build="$("${BUILD_CC:-gcc}" -dumpmachine)" \
        --host="$(${CC:-gcc} -dumpmachine)"        \
        --with-strip-program="${STRIP:-strip}"       \
        --disable-shared                             \
        --with-curses

    make -j"$(nproc)"
    make install

    popd
fi

# OpenSSL
if [[ ! -f "${PREFIX}/lib/pkgconfig/openssl.pc" ]]
then
    if [[ -z "${OPENSSL_MACHINE}" ]]
    then
        case "$(${CC:-gcc} -dumpmachine)"
        in
            x86_64-linux-gnu)
                OPENSSL_MACHINE=linux-x86_64
                ;;
            x86_64-*-linux-gnu)
                OPENSSL_MACHINE=linux-x86_64
                ;;
            arm-linux-gnueabi)
                OPENSSL_MACHINE=linux-generic32
                ;;
            arm-*-linux-gnueabi)
                OPENSSL_MACHINE=linux-generic32
                ;;
            arm-linux-gnueabihf)
                OPENSSL_MACHINE=linux-generic32
                ;;
            arm-*-linux-gnueabihf)
                OPENSSL_MACHINE=linux-generic32
                ;;
            *)
                echo "Unkown CC triplet $("${CC:-gcc}" -dumpmachine); please specify OPENSSL_MACHINE"
                exit 1
                ;;
        esac
    fi

    OPENSSL_SRC="openssl-${OPENSSL_VERSION}"
    OPENSSL_TAR="${OPENSSL_SRC}.tar.gz"

    curl -OL "https://github.com/openssl/openssl/releases/download/${OPENSSL_SRC}/${OPENSSL_TAR}"
    tar xvf "${OPENSSL_TAR}"
    rm "${OPENSSL_TAR}"
    pushd "${OPENSSL_SRC}"

    CC="${CC:-gcc}"
    CC_PROGRAM="${CC%% *}"

    sed -i "1s/.*/#!\/usr\/bin\/perl/" Configure

    ./Configure \
        "${OPENSSL_MACHINE}" \
        no-shared                                                    \
        --cross-compile-prefix="$(dirname "$(which "${CC_PROGRAM}")")/" \
        --prefix="${PREFIX}"                                         \
        --openssldir="${PREFIX}"                                     \
        --libdir=lib

    make -j"$(nproc)"
    make install_sw

    popd
fi
