cmake_minimum_required(VERSION 3.21)
project(xrt VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-g -Wall -ansi -pedantic -Wno-pointer-arith -Wsign-compare")

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()
#
#------------------ Find Protobuf --------------------------------------------------------------------------------------
if(DEFINED ENV{PROTOBUF_ROOT})
    set(PROTOBUF_ROOT $ENV{PROTOBUF_ROOT})
else()
    set(PROTOBUF_ROOT /usr/)
endif()

find_library(PROTOBUF_LIBRARIES NAMES protobuf HINTS ${PROTOBUF_ROOT}/lib)
find_path(PROTOBUF_INCLUDE_DIR NAMES google/protobuf/api.pb.h HINTS ${PROTOBUF_ROOT}/include)
if(NOT DEFINED PROTOBUF_LIBRARIES)
    message( FATAL_ERROR "Can't find protobuf. Make sure you either declare PROTOBUF_ROOT environment variable or install Protobuf in /usr/, CMake will exit." )
endif()

#------------------ Find Vivado ----------------------------------------------------------------------------------------
if(DEFINED ENV{VIVADO_HOME})
    message("-- VIVADO_HOME defined as environment variable: " $ENV{VIVADO_HOME})
    set(VIVADO_INCLUDE_DIR $ENV{VIVADO_HOME}/data/xsim/include/)
else()
    file(GLOB_RECURSE VIVADO_XSI /opt/Xilinx/Vivado/*/xsi.h /tools/Xilinx/Vivado/*/xsi.h)
    if(VIVADO_XSI)
        cmake_path(GET VIVADO_XSI PARENT_PATH VIVADO_INCLUDE_DIR)
    else()
        message( FATAL_ERROR "Can't find xsi.h. Make sure you either declare VIVADO_HOME environment variable or install Vivado în /opt/ or /tools/, CMake will exit." )
    endif()
endif()

#------------------ Find ONNX & ONNX_PROTO -----------------------------------------------------------------------------
if(DEFINED ENV{ONNX_ROOT})
    set(ONNX_LIB_ROOT $ENV{ONNX_ROOT}/lib)
    set(ONNX_INCLUDE_PATH $ENV{ONNX_ROOT}/include)
else()
    set(ONNX_LIB_ROOT /usr/lib/x86_64-linux-gnu/)
    set(ONNX_INCLUDE_PATH /usr/include)
endif()

find_library(ONNX_LIBRARIES NAMES onnx HINTS ${ONNX_LIB_ROOT})
find_library(ONNX_LIBRARIES_PROTO NAMES onnx_proto HINTS ${ONNX_LIB_ROOT})
find_path(ONNX_INCLUDE_DIR NAMES onnx/onnx_pb.h HINTS ${ONNX_INCLUDE_PATH})
if(NOT DEFINED ONNX_LIBRARIES)
    message( FATAL_ERROR "Can't find onnx. Make sure you either declare ONNX_ROOT environment variable or install Onnx in /usr/include/ & /usr/lib/x86_64-linux-gnu/, CMake will exit." )
endif()

#------------------ Find FMT -------------------------------------------------------------------------------------------

if(DEFINED ENV{FMT_HOME})
    set(FMT_LIB_ROOT $ENV{FMT_HOME}/lib)
    set(FMT_INCLUDE_PATH $ENV{FMT_HOME}/include)
else()
    set(FMT_LIB_ROOT /usr/lib/x86_64-linux-gnu/)
    set(FMT_INCLUDE_PATH /usr/include)
endif()

find_library(FMT_LIBRARY NAMES fmt HINTS ${FMT_LIB_ROOT})
find_path(FMT_INCLUDE_DIR NAMES fmt/core.h HINTS ${FMT_INCLUDE_PATH})
if(NOT DEFINED FMT_LIBRARY)
    message( FATAL_ERROR "Can't find fmt. Make sure you either declare FMT_HOME environment variable or install FMT in /usr/include/ & /usr/lib/x86_64-linux-gnu/, CMake will exit." )
endif()

#------------------ Find OpenSSL----------------------------------------------------------------------------------------
find_path(OPENSSL_INCLUDE_DIR NAMES openssl/md5.h HINTS /opt/homebrew/anaconda3/include/ /usr/include)
if(NOT OPENSSL_INCLUDE_DIR)
    message( FATAL_ERROR "Can't find openssl. Make sure you install OpenSSL in /usr/include/ or /opt/homebrew/anaconda3/include/, CMake will exit." )
endif()

#------------------ JSON -----------------------------------------------------------------------------------------------

include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

#------------------ All local sources ----------------------------------------------------------------------------------
file(GLOB_RECURSE SOURCE_FILES include/*.h src/*.cpp src/*.hpp)

#------------------ Executable -----------------------------------------------------------------------------------------
add_executable(xrt ${SOURCE_FILES})

target_compile_definitions(xrt PRIVATE ONNX_NAMESPACE=onnx ONNX_ML=1)

target_include_directories(xrt PUBLIC
        ${PROTOBUF_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/include/
        ${VIVADO_INCLUDE_DIR}
        ${ONNX_INCLUDE_DIR}
        ${FMT_INCLUDE_DIR}
        ${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(xrt PUBLIC
        ${PROTOBUF_LIBRARIES}
        ${ONNX_LIBRARIES}
        ${ONNX_LIBRARIES_PROTO}
        ${FMT_LIBRARY}
        nlohmann_json::nlohmann_json
        dl
        readline
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})




