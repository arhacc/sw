cmake_minimum_required(VERSION 3.21)
project(xrt VERSION 0.1.0)


set(CMAKE_CXX_STANDARD 20)
set(DOWNLOAD_EXTRACT_TIMESTAMP NEW)
set(XPU_SIMULATOR ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_CXX_FLAGS "-g -Wall -ansi -pedantic -Wno-pointer-arith -Wsign-compare -fpermissive -pthread")

include(CTest)
enable_testing()

set(ONNX_NAMESPACE "onnx")
set(PROTOBUF_ROOT "/usr/")

find_library(PROTOBUF_LIBRARIES
        NAMES protobuf
        HINTS ${PROTOBUF_ROOT}/lib
        )

find_path(PROTOBUF_INCLUDE_DIR
        NAMES google/protobuf/api.pb.h
        HINTS ${PROTOBUF_ROOT}/include
        )

#foreach(libname onnx onnx_proto)
#    unset(libpath CACHE)
#    find_library(ONNX_LIBRARIE_${libname}
#        NAMES ${libname}
#        HINTS $ENV{ONNX_ROOT}/lib)
#    list(APPEND ONNX_LIBRARIES ${ONNX_LIBRARIE_${libname}})
#endforeach()

#find_path(ONNX_INCLUDE_DIR
#    NAMES onnx/onnx.pb.h
#    HINTS $ENV{ONNX_ROOT}/include
#)

#set(ONNX_INCLUDE_DIR /usr/include)


include(FetchContent)

include_directories(
        $ENV{BINUTILS_INCLUDE}
        /usr/include
        $ENV{VIVADO_HOME}/data/xsim/include/
)

link_directories(
        $ENV{BINUTILS_LIB}
        $ENV{FMT_HOME}/lib/
        $ENV{VIVADO_HOME}/data/lib/lnx64.o/
)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

file(GLOB_RECURSE SOURCE_FILES include/*.h src/*.cpp src/*.hpp)

add_executable(xrt ${SOURCE_FILES})

target_compile_definitions(xrt
        PRIVATE
        ONNX_NAMESPACE=${ONNX_NAMESPACE}
        ONNX_ML=1
        )

target_include_directories(xrt
        PUBLIC
        ${PROTOBUF_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/include/
        ${CMAKE_SOURCE_DIR}/src/
        $ENV{VIVADO_HOME}/data/xsim/include/
        /usr/include/
        .
        )

target_link_libraries(xrt
        PUBLIC
        ${PROTOBUF_LIBRARIES}
        ${ONNX_LIBRARIES}
        nlohmann_json::nlohmann_json
        dl
        fmt
        readline
        onnx
        onnx_proto
        #    $ENV{FMT_HOME}/lib/
        #    /opt/homebrew/anaconda3/include/lib/
        )


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})




