# Build xrt

Install the dependencies:

```bash
# Install compiler and python (for conan)
sudo apt update
sudo apt install build-essential git python3 python3-pip

# Install conan
python3 -m pip install --user conan

# RUN THIS ONLY IF conan compains about not being in PATH (add this to .bashrc)
export PATH="${PATH}:${HOME}/.local/bin"

# Configure conan
conan profile detect --force
sed -e 's/Release/Debug/g' ~/.conan2/profiles/default >~/.conan2/profiles/debug

# Make sure Xilinx Vivado enviornment is set (add this to .bashrc)
source /opt/Xilinx/Vivado/2022.2/settings64.sh

# Speed up CPM compilation (add this to .bashrc)
export CPM_SOURCE_CACHE="${HOME}/.cache/CPM"
```

All other dependencies are handled by conan.

Compile:

```bash
# Building for debug
./bin/build-xrt.sh

# Building for release
./bin/build-xrt.sh -p default -r Release
```

# Build xrt locally.

Install the dependencies (assuming Ubuntu 22.04):

```bash
sudo apt install build-essential cmake libssl-dev libncurses-dev libonnx-dev libprotobuf-dev libreadline-dev ninja-build protobuf-compiler
```

Install the dyncall library not provided in the standard packages:

```bash
wget "https://www.dyncall.org/r1.4/dyncall-1.4.tar.gz" &&
tar xfz dyncall-1.4.tar.gz &&
cd dyncall-1.4 &&
./configure --prefix=/usr/local &&
make &&
sudo make install &&
cd .. &&
rm -r dyncall-1.4
```

In the xrt directory run:

```bash
cmake -B build -S . -G Ninja
cmake --build build
```

# Cross build xrt

The new method assumes an x86-64 machine is doing the build.

To build xrt to run on an x86-64 run the following commands from the xrt directory.

```bash
./bin/build-deps.sh x86_64-linux-gnu
./bin/build-xrt.sh x86_64-linux-gnu
```

To build xrt to run on a pynq board, run the following:

```bash
./bin/build-deps.sh arm-linux-gnueabihf
./bin/build-xrt.sh arm-linux-gnueabihf
```

The executables will be found in the new `build/xrt/x86_64-linux-gnu/bin` or
`build/xrt/arm-linux-gnueabihf/bin` directory respectively.
