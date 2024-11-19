## Build xrt

### Install the dependencies

```bash
# Install dependencies
sudo apt update
sudo apt install build-essential cmake ninja-build libssl-dev libreadline-dev

# Make sure Xilinx Vivado enviornment is set (add this to .bashrc)
source /opt/Xilinx/Vivado/2022.2/settings64.sh
```

### Compile

```bash
./bin/build.sh [options]
```

Options for `build.sh` are:

* `-p [profile directory]`: Selects the directory subdirectory of the build directory to use. Default is `debug`.
* `-r [release type]`: Select cmake release type. Generally will either be `Debug` or `Release`. Check the [official documentation](https://cmake.org/cmake/help/latest/variable/CMAKE_BUILD_TYPE.html). Default is `Debug`.
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
