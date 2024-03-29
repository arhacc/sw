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

# Speed up CPM compilation for faster build times (add this to .bashrc)
export CPM_SOURCE_CACHE="${HOME}/.cache/CPM"
```
