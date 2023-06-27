cmake_minimum_required(VERSION 3.21)
project(xrt VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-gdwarf-4 -Wall -ansi -pedantic -Wno-unused-private-field -Wno-pointer-arith -Wsign-compare")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

#------------------ Options --------------------------------------------------------------------------------------------
option(XRT_DYNAMIC_MID_LEVEL "Build the mid-level library as a dynamically loadable library")
option(XRT_TESTS "Build tests" ON)

#------------------ Enable CPM package manager -------------------------------------------------------------------------
include(cmake/CPM.cmake)

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

#------------------ Find OpenSSL----------------------------------------------------------------------------------------
#find_path(OPENSSL_INCLUDE_DIR NAMES openssl/md5.h HINTS /opt/homebrew/anaconda3/include/ /usr/include /root/x-tools/)
#if(NOT OPENSSL_INCLUDE_DIR)
#    message( FATAL_ERROR "Can't find openssl. Make sure you install OpenSSL in /usr/include/ or /opt/homebrew/anaconda3/include/, CMake will exit." )
#endif()


#------------------ fmt ------------------------------------------------------------------------------------------------

CPMAddPackage(
    NAME fmt
    GITHUB_REPOSITORY fmtlib/fmt
    GIT_TAG 10.0.0
)

#------------------ JSON -----------------------------------------------------------------------------------------------

CPMAddPackage(
    NAME json
    GITHUB_REPOSITORY nlohmann/json
    GIT_TAG v3.11.2
)


#------------------ reproc ---------------------------------------------------------------------------------------------

set(REPROC++ ON)

CPMAddPackage(
    NAME reproc
    GITHUB_REPOSITORY DaanDeMeyer/reproc
    GIT_TAG v14.2.4
)


#------------------ magic_enum -----------------------------------------------------------------------------------------
CPMAddPackage(
    NAME magic_enum
    GITHUB_REPOSITORY Neargye/magic_enum
    GIT_TAG v0.9.2
)

#------------------ All local sources ----------------------------------------------------------------------------------
file(
    GLOB_RECURSE
    XRT_ALL_SOURCE_FILES
    
    ${PROJECT_SOURCE_DIR}/src/*.cpp

)

file(
    GLOB_RECURSE
    XRT_CORE_SOURCE_FILES
    
    ${PROJECT_SOURCE_DIR}/src/common/*.cpp
    ${PROJECT_SOURCE_DIR}/src/manager/*.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/*.cpp

)

add_library(xrtcore OBJECT ${XRT_CORE_SOURCE_FILES})

target_include_directories(xrtcore PRIVATE
    ${PROTOBUF_INCLUDE_DIR}
    ${VIVADO_INCLUDE_DIR}
    ${ONNX_INCLUDE_DIR}
    ${FMT_INCLUDE_DIR}
#    ${OPENSSL_INCLUDE_DIR}

    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(xrtcore PUBLIC
    ${PROTOBUF_LIBRARIES}
    ${ONNX_LIBRARIES}
    ${ONNX_LIBRARIES_PROTO}
    dl
    readline
    dyncall_s
    dynload_s
    ncurses
    protobuf

    reproc++
    magic_enum::magic_enum

    fmt::fmt
    nlohmann_json::nlohmann_json
)


add_subdirectory(../../libraries/midlevel libraries/midlevel)

#------------------ Executable -----------------------------------------------------------------------------------------
add_executable(xrt ${XRT_ALL_SOURCE_FILES})

target_compile_definitions(xrt PRIVATE ONNX_NAMESPACE=onnx ONNX_ML=1)

target_include_directories(xrt PUBLIC
        ${PROTOBUF_INCLUDE_DIR}
        ${VIVADO_INCLUDE_DIR}
        ${ONNX_INCLUDE_DIR}
#        ${OPENSSL_INCLUDE_DIR}

        ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(xrt PUBLIC
        ${PROTOBUF_LIBRARIES}
        ${ONNX_LIBRARIES}
        ${ONNX_LIBRARIES_PROTO}
        fmt::fmt
        nlohmann_json::nlohmann_json
        protobuf
        dl
        readline
        dyncall_s
        dynload_s
        reproc++
        ncurses
        magic_enum::magic_enum
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
