FROM ubuntu:23.04

COPY docker-install.sh /install.sh
COPY ct-ng.config /ct-ng.config

RUN chmod +x /install.sh
RUN /install.sh
RUN mkdir /srcdir
RUN mkdir /vivado

ENV  CC=/root/x-tools/arm-unknown-linux-gnueabihf/bin/arm-unknown-linux-gnueabihf-gcc
ENV CXX=/root/x-tools/arm-unknown-linux-gnueabihf/bin/arm-unknown-linux-gnueabihf-g++
ENV CXXFLAGS=-Wno-psabi
ENV VIVADO_HOME=/vivado

ENV OPENSSL_ROOT_DIR=/root/x-tools/arm-unknown-linux-gnueabihf/arm-unknown-linux-gnueabihf
ENV ONNX_ROOT=/root/x-tools/arm-unknown-linux-gnueabihf/arm-unknown-linux-gnueabihf
ENV FMT_HOME=/root/x-tools/arm-unknown-linux-gnueabihf/arm-unknown-linux-gnueabihf
ENV PROTOBUF_ROOT=/root/x-tools/arm-unknown-linux-gnueabihf/arm-unknown-linux-gnueabihf
ENV CMAKE_LIBRARY_PATH=/root/x-tools/arm-unknown-linux-gnueabihf/arm-unknown-linux-gnueabihf/lib


WORKDIR /srcdir

CMD true
