#!/bin/bash

ZIG_VERSION='0.11.0-dev.4407+4d7dd1689'
LIBRESSL_VERSION=3.8.0
OPENSSL_VERSION=3.1.1
DYNCALL_VERSION=1.4
NCURSES_VERSION=6.4
READLINE_VERSION=8.2
PROTOBUF_VERSION=3.21.12 # DO NOT UPDATE
ONNX_VERSION=1.14.0

. `dirname "$0"`/build-functions.sh

install-zig() {
	cd "${tmpdir}" &&

	if [[ ! -f "${zig}" ]]
	then
		if [[ -d "${zigdir}" ]]
		then
			rm -r "${zigdir}"
		fi &&

		wget "https://ziglang.org/builds/zig-linux-x86_64-${ZIG_VERSION}.tar.xz" &&
		tar xf "zig-linux-x86_64-${ZIG_VERSION}.tar.xz" &&
		mv "zig-linux-x86_64-${ZIG_VERSION}" "${zigdir}"
	fi
}

install-libressl() {
	cd "${tmpdir}" &&

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
	cmake --install build
}

install-openssl() {
	cd "${tmpdir}" &&

	wget https://github.com/openssl/openssl/releases/download/openssl-${OPENSSL_VERSION}/openssl-${OPENSSL_VERSION}.tar.gz &&
	tar xfz openssl-${OPENSSL_VERSION}.tar.gz &&
	cd openssl-${OPENSSL_VERSION} &&

	if [ "${simplehost}" = x86_64 ]
	then
		openssltarget=linux-x86_64
	elif [ "${simplehost}" = arm ]
	then
		openssltarget=linux-generic32
	else
		echo "Unkown openssl target" >&2
		exit 1
	fi &&

	CC="${zig} cc -target ${target}" CXX="${zig} c++ -target ${target}" \
		./Configure "${openssltarget}" no-shared \
			--prefix="${depsdir}" \
			--openssldir="${depsdir}" \
			--libdir=lib &&

	make -j$(nproc) &&
	make install_sw install_ssldirs
}

install-dyncall() {
	cd "${tmpdir}" &&

	wget "https://www.dyncall.org/r${DYNCALL_VERSION}/dyncall-${DYNCALL_VERSION}.tar.gz" &&
	tar xfz dyncall-${DYNCALL_VERSION}.tar.gz &&
	cd dyncall-${DYNCALL_VERSION}

	if [ "${simplehost}" = arm ]
	then
		extracpuhint="-mcpu=cortex_a9"
	else
		extracpuhint=""
	fi &&

	./configure --prefix="${depsdir}" &&
	CC="${zig} cc -target ${target} ${extracpuhint}" CXX="${zig} c++ -target ${target} ${extracpuhint}" \
		make &&
	make install
}

install-ncurses() {
	cd "${tmpdir}" &&

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
	make install
}

install-readline() {
	cd "${tmpdir}" &&

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
	make install
}

install-local-protobuf() {
	cd "${tmpdir}" &&

	wget -O protobuf-${PROTOBUF_VERSION}.tar.gz https://github.com/protocolbuffers/protobuf/archive/refs/tags/v${PROTOBUF_VERSION}.tar.gz &&
	tar xfz protobuf-${PROTOBUF_VERSION}.tar.gz &&
	cd protobuf-${PROTOBUF_VERSION} &&

	CC="${zig} cc" CXX="${zig} c++" \
		cmake \
			-B local-build \
			-S . \
			-G Ninja \
			-D CMAKE_INSTALL_PREFIX="${depsdir}/../local-protobuf" \
			-D CMAKE_LIBRARY_PATH="${depsdir}/../local-protobuf/lib" \
			-D CMAKE_CXX_STANDARD=14 \
			-D protobuf_BUILD_TESTS=OFF \
			-D protobuf_BUILD_SHARED_LIBS=OFF &&

	cmake --build local-build &&
	cmake --install local-build
}

install-onnx() {
	cd "${tmpdir}" &&

	mkdir onnx &&
	cd onnx &&

	wget -O protobuf-${PROTOBUF_VERSION}.tar.gz https://github.com/protocolbuffers/protobuf/archive/refs/tags/v${PROTOBUF_VERSION}.tar.gz &&
	tar xfz protobuf-${PROTOBUF_VERSION}.tar.gz &&
	wget -O onnx-${ONNX_VERSION}.tar.gz https://github.com/onnx/onnx/archive/refs/tags/v${ONNX_VERSION}.tar.gz &&
	tar xf onnx-${ONNX_VERSION}.tar.gz &&

	echo \
"cmake_minimum_required(VERSION 3.5)
project(Project)

add_subdirectory(protobuf-${PROTOBUF_VERSION})
add_subdirectory(onnx-${ONNX_VERSION})" >CMakeLists.txt

	PATH="${depsdir}/../local-protobuf/bin:$PATH" CC="${zig} cc -target ${target}" CXX="${zig} c++ -target ${target}" \
		cmake \
		    -B build \
		    -S . \
		    -G Ninja \
		    -D CMAKE_INSTALL_PREFIX="${depsdir}" \
		    -D CMAKE_LIBRARY_PTAH="${depsdir}" \
			-D CMAKE_CXX_STANDARD=14 \
			-D ONNX_CUSTOM_PROTOC_EXECUTABLE="${depsdir}/../local-protobuf/bin/protoc" \
		    -D ONNX_USE_PROTOBUF_SHARED_LIBS=OFF \
			-D protobuf_BUILD_TESTS=OFF \
			-D protobuf_BUILD_SHARED_LIBS=OFF &&

	cmake --build build &&
	cmake --install build
}

check-wd &&
set-variables "$@" &&

if [[ -d "${tmpdir}" ]]
then
	rm -rf "${tmpdir}"
fi &&

mkdir -p "${tmpdir}" &&

cd "${tmpdir}" &&

install-zig &&

if have-to-create-dir "${protobufdir}"
then
	install-local-protobuf &&

	done-creating-dir "${protobufdir}"
fi &&

if have-to-create-dir "${depsdir}"
then
	mkdir -p "${depsdir}" &&

	install-openssl &&
	# install-libressl &&
	install-dyncall &&
	install-ncurses &&
	install-readline &&
	install-onnx &&

	done-creating-dir "${depsdir}"
else
	echo "Nothing to do."
	echo "If you want to rebuild dependencies from scratch, delete ${depsdir}"
fi &&

cd "${wd}" &&

rm -r "${tmpdir}"
