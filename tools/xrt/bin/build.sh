#!/bin/bash -f
#export LD_LIBRARY_PATH=/usr/local/bin/:/Users/marius/projects/protobuf/bazel-out/darwin_arm64-fastbuild/bin/:$LD_LIBRARY_PATH
#export LD_LIBRARY_PATH=/Users/marius/projects/protobuf/bazel-out/darwin_arm64-fastbuild/bin/_objs/protobuf/:$LD_LIBRARY_PATH
#rm -rf ../build
mkdir -p ../build/
cd ../build
cmake ..
make -j$(nproc)