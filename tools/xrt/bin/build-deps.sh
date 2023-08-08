#!/bin/bash

ZIG_VERSION='0.11.0-dev.4407+4d7dd1689'
LIBRESSL_VERSION=3.8.0
DYNCALL_VERSION=1.4
NCURSES_VERSION=6.4
READLINE_VERSION=8.2
OPENSSL_VERSION=3.1.1
PROTOBUF_VERSION=3.21.12 # DO NOT UPDATE
ONNX_VERSION=1.14.0

if [[ `basename "$(pwd)"` != xrt ]]
then
	echo "This script needs to be run from the xrt directory (as ./bin/build-deps.sh)" >&2
	exit 1
fi

if [ $# -ne 1 ]
then
	echo "Please provide target triple" >&2
	echo "(you probably want arm-linux-gnueabihf for Pynq or x86_64-linux-gnu)" >&2
	exit 1
fi

target="$1"
simplehost="$(echo "${target}" | grep -oE '^[^-]*')"
depsdir="$(pwd)/build/deps/${target}"
wd="$(pwd)"
zig="${depsdir}/../zig/zig"

export AR="${zig} ar"

install-zig() {
	if [[ ! -f "${zig}" ]]
	then
		wget "https://ziglang.org/builds/zig-linux-x86_64-${ZIG_VERSION}.tar.xz" &&
		tar xf "zig-linux-x86_64-${ZIG_VERSION}.tar.xz" &&
		mv "zig-linux-x86_64-${ZIG_VERSION}" "${depsdir}/../zig"
	fi
}

install-libressl() {
	wget "https://ftp.openbsd.org/pub/OpenBSD/LibreSSL/libressl-${LIBRESSL_VERSION}.tar.gz" &&
	tar xf "libressl-${LIBRESSL_VERSION}.tar.gz" &&
	cd "libressl-${LIBRESSL_VERSION}" &&

	CC="${zig} cc -target ${target}" CXX="${zig} c++ -target ${target}" \
		cmake \
			-B build \
			-S . \
			-G Ninja \
			-D "CMAKE_INSTALL_PREFIX=${depsdir}" \
			-D BUILD_SHARED_LIBS=OFF \
			-D ENABLE_ASM=OFF &&

	cmake --build build &&
	cmake --install build &&

	cd ..
}

install-openssl() {
	wget https://github.com/openssl/openssl/releases/download/openssl-${OPENSSL_VERSION}/openssl-${OPENSSL_VERSION}.tar.gz &&
	tar xfz openssl-${OPENSSL_VERSION}.tar.gz &&
	cd openssl-${OPENSSL_VERSION} &&

	if [ "${simplehost}" = x86_64 ]
	then
		openssltarget=linux-x86_64
	elif [ "${simplehost}" = arm ]
	then
		openssltarget=linux-generic32
	fi

	CC="${zig} cc -target ${target}" CXX="${zig} c++ -target ${target}" \
		./Configure "${openssltarget}" no-shared \
			--prefix="${depsdir}" \
			--openssldir="${depsdir}" \
			--libdir=lib &&

	make -j$(nproc) &&
	make install_sw install_ssldirs &&

	cd ..
}

install-dyncall() {
	wget "https://www.dyncall.org/r${DYNCALL_VERSION}/dyncall-${DYNCALL_VERSION}.tar.gz" &&
	tar xfz dyncall-${DYNCALL_VERSION}.tar.gz &&
	cd dyncall-${DYNCALL_VERSION}

	if [ "${simplehost}" = arm ]
	then
		extracpuhint="-mcpu=cortex_a9"
	else
		extracpuhint=""
	fi

	./configure --prefix="${depsdir}" &&
	CC="${zig} cc -target ${target} ${extracpuhint}" CXX="${zig} c++ -target ${target} ${extracpuhint}" \
		make &&
	make install &&

	cd ..
}

install-ncurses() {
	wget ftp://ftp.gnu.org/gnu/ncurses/ncurses-${NCURSES_VERSION}.tar.gz &&
	tar xfz ncurses-${NCURSES_VERSION}.tar.gz &&
	cd ncurses-${NCURSES_VERSION} &&

	CC="${zig} cc -target ${target}" CXX="${zig} c++ -target ${target}" \
		./configure \
			--prefix="${depsdir}" \
			--host="${simplehost}" \
			--enable-static \
			--disable-shared \
			--disable-stripping \
			--without-ada \
			--without-manpages &&
	
	make -j"$(nproc)" &&
	make install &&

	cd ..
}

install-readline() {
	wget ftp://ftp.gnu.org/gnu/readline/readline-${READLINE_VERSION}.tar.gz &&
	tar xfz readline-${READLINE_VERSION}.tar.gz &&
	cd readline-${READLINE_VERSION} &&

	CC="${zig} cc -target ${target}" CXX="${zig} c++ -target ${target}" \
		./configure \
			--prefix="${depsdir}" \
			--host="${simplehost}" \
			--enable-static \
			--disable-shared \
			--without-ada \
			--without-manpages &&

	make -j"$(nproc)" &&
	make install &&

	cd ..
}

install-protobuf() {
	wget -O protobuf-${PROTOBUF_VERSION}.tar.gz https://github.com/protocolbuffers/protobuf/archive/refs/tags/v${PROTOBUF_VERSION}.tar.gz &&
	tar xfz protobuf-${PROTOBUF_VERSION}.tar.gz &&
	cd protobuf-${PROTOBUF_VERSION} &&

	CC="${zig} cc -target ${target}" CXX="${zig} c++ -target ${target}" \
		cmake \
			-B build \
			-S . \
			-G Ninja \
			-D CMAKE_INSTALL_PREFIX="${depsdir}" \
			-D CMAKE_LIBRARY_PATH="${depsdir}/lib" \
			-D CMAKE_CXX_STANDARD=14 \
			-D protobuf_BUILD_TESTS=OFF \
			-D protobuf_BUILD_SHARED_LIBS=OFF &&

	cmake --build build &&
	cmake --install build &&

	cd ..
}

install-onnx() {
	wget -O onnx-${ONNX_VERSION}.tar.gz https://github.com/onnx/onnx/archive/refs/tags/v${ONNX_VERSION}.tar.gz &&
	tar xf onnx-${ONNX_VERSION}.tar.gz &&
	cd onnx-${ONNX_VERSION} &&

	CC="${zig} cc -target ${target}" CXX="${zig} c++ -target ${target}" \
		cmake \
		    -B build \
		    -S . \
		    -G Ninja \
		    -D CMAKE_INSTALL_PREFIX="${depsdir}" \
		    -D CMAKE_LIBRARY_PTAH="${depsdir}" \
		    -D ONNX_USE_PROTOBUF_SHARED_LIBS=OFF &&

	cmake --build build &&
	cmake --install build &&

	cd ..
}

if [[ -d "${wd}/build/tmp-dl" ]]
then
	rm -rf "${wd}/build/tmp-dl"
fi

mkdir -p "${wd}/build/tmp-dl" &&

cd build/tmp-dl &&

mkdir -p "${depsdir}/.." &&
install-zig &&

if [[ ! -f "${depsdir}/.all-good" ]]
then
	if [[ -d "${depsdir}" ]]
	then
		rm -rf "${depsdir}"
	fi

	mkdir -p "${depsdir}"

	install-openssl &&
	#install-libressl &&
	install-dyncall &&
	install-ncurses &&
	install-readline &&
	install-protobuf &&
	install-onnx &&
	touch "${depsdir}/.all-good"
else
	echo "Nothing to do."
	echo "If you want to rebuild dependencies from scratch, delete ${depsdir}"
fi

cd "${wd}/build" &&

rm -r tmp-dl
