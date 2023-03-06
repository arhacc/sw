cmake_minimum_required(VERSION 3.21)
project(xrt VERSION 0.1.0)


set(CMAKE_CXX_STANDARD 20)
#cmake_policy(SET CMP0135 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0135 NEW)
set(XPU_SIMULATOR ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_CXX_FLAGS "-std=c++2a -g -Wall -ansi -pedantic -Wno-pointer-arith -Wsign-compare -fpermissive -pthread")
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ~/.xpu/bin/${CMAKE_BUILD_TYPE})

include(CTest)
enable_testing()

set(ONNX_NAMESPACE "onnx")
#set(ONNX_ROOT ${ONNX_ROOT})
set(PROTOBUF_ROOT "/usr/")
#set(VITIS_HOME ${VITIS_HOME})
#set(FMT_HOME ${FMT_HOME})

find_library(PROTOBUF_LIBRARIES
    NAMES protobuf
    HINTS ${PROTOBUF_ROOT}/lib
)

find_path(PROTOBUF_INCLUDE_DIR
    NAMES google/protobuf/api.pb.h
    HINTS ${PROTOBUF_ROOT}/include
)

foreach(libname onnx onnx_proto)
    unset(libpath CACHE)
    find_library(ONNX_LIBRARIE_${libname}
        NAMES ${libname}
        HINTS $ENV{ONNX_ROOT}/lib)
    list(APPEND ONNX_LIBRARIES ${ONNX_LIBRARIE_${libname}})
endforeach()

find_path(ONNX_INCLUDE_DIR
    NAMES onnx/onnx.pb.h
    HINTS $ENV{ONNX_ROOT}/include
)

include(FetchContent)

include_directories(
    $ENV{BINUTILS_INCLUDE}
)

link_directories(
    $ENV{BINUTILS_LIB}
    $ENV{FMT_HOME}/lib/
    $ENV{VIVADO_HOME}/data/lib/lnx64.o/
)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

add_executable(xrt
    ${CMAKE_SOURCE_DIR}/src/targets/Targets.cpp
    ${CMAKE_SOURCE_DIR}/src/manager/Manager.cpp
    ${CMAKE_SOURCE_DIR}/src/transformers/Transformers.cpp
    ${CMAKE_SOURCE_DIR}/src/sources/Sources.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

target_compile_definitions(xrt 
    PRIVATE 
    ONNX_NAMESPACE=${ONNX_NAMESPACE}
)

target_include_directories(xrt 
    PUBLIC
    ${PROTOBUF_INCLUDE_DIR}
    ${ONNX_INCLUDE_DIR}
    ${CMAKE_SOURCE_DIR}/include/
    ${CMAKE_SOURCE_DIR}/src/
    $ENV{VIVADO_HOME}/data/xsim/include/
#    $ENV{VITIS_HOME}/examples/xsim/verilog/xsi/counter/
    $ENV{FMT_HOME}/include/
    /opt/homebrew/anaconda3/include/
    /usr/include/
    .
)

target_link_libraries(xrt 
    PUBLIC
    ${PROTOBUF_LIBRARIES}
    ${ONNX_LIBRARIES}
    nlohmann_json::nlohmann_json
    dl
    readline
    fmt
#    $ENV{FMT_HOME}/lib/
#    /opt/homebrew/anaconda3/include/lib/
)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})




