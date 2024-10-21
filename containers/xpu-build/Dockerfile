FROM ubuntu:22.04

RUN DEBIAN_FRONTEND=noninteractive apt update \
  && DEBIAN_FRONTEND=noninteractive apt upgrade -y \
  && DEBIAN_FRONTEND=noninteractive apt install -y \
    bison \
    build-essential \
    cmake \
    flex \
    gcc-12-arm-linux-gnueabihf \
    g++-12-arm-linux-gnueabihf \
    git \
    libncurses5-dev \
    libtinfo5 \
    locales \
    ninja-build \
    openjdk-21-jdk \
    parallel \
    patchelf \
    pipx \
    python3 \
    python3-pip \
    zip \
  && DEBIAN_FRONTEND=noninteractive apt autoremove -y \
  && DEBIAN_FRONTEND=noninteractive apt autoclean

RUN locale-gen en_US.UTF-8 && update-locale # Vivado wants this

RUN pipx ensurepath && pipx install conan && pipx install onnx2json

RUN ~/.local/bin/conan profile detect --force

COPY profiles/* /root/.conan2/profiles

COPY gradle.properties /root/.gradle/gradle.properties

COPY build.sh /
RUN chmod +x /build.sh
COPY test.sh /
RUN chmod +x /test.sh
