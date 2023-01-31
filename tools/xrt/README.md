# Build ONNX from Source
```
git clone https://github.com/onnx/onnx.git
cd onnx
mkdir build
cd build
export ONNX_ROOT=<onnx-installation-absolute-path>
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DONNX_NAMESPACE=onnx -DCMAKE_BUILD_TYPE=Release -DONNX_ML=0 -DBUILD_SHARED_LIBS=ON 
-DONNX_USE_PROTOBUF_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=${ONNX_ROOT} ../
make -j$(nproc) && make install
```
  
# Build xrt
```
export JAVA_HOME=$(/usr/libexec/java_home)
export GIT_ROOT="$HOME/projects"
export ONNX_ROOT="$GIT_ROOT/onnx/install"
export BINUTILS_HOME="/opt/homebrew/opt/binutils"
export LDFLAGS="-L$BINUTILS_HOME/lib"
export CPPFLAGS="-I$BINUTILS_HOME/include -I/opt/homebrew/opt/openjdk/include"
export FMT_HOME="/opt/homebrew/Cellar/fmt/9.1.0"
export XILINX_HOME="$HOME/apps/Xilinx"
export VITIS_HOME="$XILINX_HOME/Vitis/2022.2"


cd $GIT_ROOT/xrt
./build.sh
```

# Run xrt
```
cd $GIT_ROOT/xrt
./run.sh
```
