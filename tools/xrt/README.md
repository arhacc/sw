## Build xrt

### Install the dependencies

```bash
# Install compiler and python (for conan)
sudo apt update
sudo apt install build-essential git patchelf python3 python3-pip

# Install conan
python3 -m pip install --user conan

# RUN THIS ONLY IF conan compains about not being in PATH (add this to .bashrc)
export PATH="${PATH}:${HOME}/.local/bin"

# Configure conan
conan profile detect --force
sed -e 's/Release/Debug/g' ~/.conan2/profiles/default >~/.conan2/profiles/debug

# Make sure Xilinx Vivado enviornment is set (add this to .bashrc)
source /opt/Xilinx/Vivado/2022.2/settings64.sh

# Speed up CPM compilation for faster build times (add this to .bashrc)
export CPM_SOURCE_CACHE="${HOME}/.cache/CPM"
```

### Compile

```bash
./bin/build.sh [options]
```

Options for `build.sh` are:

* `-p [profile]`: Select conan profile to build with; basic ones are `default` for release builds and `debug` for debug builds. *Must be consistent with the `-r` option*. Additional ones can be set up for cross-compilation or for building with a differnt toolchain. Check the [conan documentation](https://docs.conan.io/2/reference/config_files/profiles.html). Default is `debug`.
* `-r [release type]`: Select cmake release type. Generally will either be `Debug` or `Release`. *Must be consistent with the `-p` option*. Default is `Debug`.
* `-M`: Skip building xpu mid level libraries.
* `-X`: Skip building the xrt binary (the xrtcore library will always be built).
* `-S`: Skip building support for running on the vivado xsim simulator.
* `-F`: Skip building support for running on fpga hardware.
* `-G`: Skip building support for running on the software golden model.

## Run xrt

```bash
./bin/run.sh [options]
```

Options for running xrt:

* `-source:[source] [extra-arg]`. Source from which to run. One of: `net`, `batch`, `cmd`, `file`. `cmd` takes no extra argument. `net` taxes an extra argument specifying the port number on which to listen. `batch` and `file` take an extra argument specifying the path to the file to use.
* `-target:[target]`: Target on which to run. One of: `sim`, `fpga`, `gm`.
* `-arch [architecture]`: XPU architecutre on which to run (determined automatically where possible).
* `-log_suffix [suffix]`: Suffix string to add to the path of all log files written.
* `-nowdb`: If running with target as simulation; disable waveform database generation.