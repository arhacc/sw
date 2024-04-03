cmake_minimum_required(VERSION 3.21)
project(xrt VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-gdwarf-4 -Wall -ansi -pedantic -Wno-pointer-arith -Wsign-compare -fPIC")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-attributes -Wno-psabi -Wno-changes-meaning")
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
else(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-dollar-in-identifier-extension -Wno-c99-extensions")
endif()

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

# If CMAKE_BUILD_TYPE is not set, CMake will generally default to a Debug build (what we want).
# ONNX overwrites this to Release, so we need to overwrite it first.
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Build type not set - defaulting to Debug")
  set(
    CMAKE_BUILD_TYPE "Debug"
    CACHE
      STRING
      "Choose the type of build from: Debug Release RelWithDebInfo MinSizeRel Coverage."
    FORCE)
endif()

option(XRT_SKIP_MIDLEVEL "Do not build the midlevel library" OFF)
option(XRT_SKIP_XRT "Do not build the xrt executable" OFF)

#------------------ Enable CPM package manager -------------------------------------------------------------------------

include(cmake/CPM.cmake)

#------------------ fmt ------------------------------------------------------------------------------------------------

CPMAddPackage(
    NAME fmt
    GITHUB_REPOSITORY fmtlib/fmt
    GIT_TAG 10.2.1
)

list(APPEND XRT_DEPS_LIBS fmt::fmt)

#------------------ JSON -----------------------------------------------------------------------------------------------

CPMAddPackage(
    NAME json
    GITHUB_REPOSITORY nlohmann/json
    GIT_TAG v3.11.3
)

list(APPEND XRT_DEPS_LIBS nlohmann_json::nlohmann_json)

#------------------ reproc ---------------------------------------------------------------------------------------------

set(REPROC++ ON)

CPMAddPackage(
    NAME reproc
    GITHUB_REPOSITORY DaanDeMeyer/reproc
    GIT_TAG v14.2.4
)

list(APPEND XRT_DEPS_LIBS reproc++)

#------------------ magic_enum -----------------------------------------------------------------------------------------

CPMAddPackage(
    NAME magic_enum
    GITHUB_REPOSITORY Neargye/magic_enum
    GIT_TAG v0.9.5
)

list(APPEND XRT_DEPS_LIBS magic_enum::magic_enum)

CPMAddPackage(
    NAME endian
    GITHUB_REPOSITORY steinwurf/endian
    GIT_TAG 13.0.0
)

list(APPEND XRT_DEPS_LIBS steinwurf::endian)

set(SOCKPP_BUILD_SHARED OFF)
set(SOCKPP_BUILD_STATIC ON)

CPMAddPackage(
    NAME sockpp
    GITHUB_REPOSITORY fpagliughi/sockpp
    GIT_TAG v1.0.0
)

list(APPEND XRT_DEPS_LIBS sockpp-static)

#------------------ Conan packages -------------------------------------------------------------------------------------

#------------------ OpenSSL --------------------------------------------------------------------------------------------


find_package(OpenSSL REQUIRED)

list(APPEND XRT_DEPS_LIBS OpenSSL::Crypto)

#------------------ readline -------------------------------------------------------------------------------------------

find_package(readline REQUIRED)

list(APPEND XRT_DEPS_LIBS readline::readline)

#------------------ ONNX -----------------------------------------------------------------------------------------------

find_package(ONNX REQUIRED)

list(APPEND XRT_DEPS_LIBS onnx::onnx)

find_package(TBB REQUIRED)
list(APPEND XRT_DEPS_LIBS onetbb::onetbb)

find_package(asio REQUIRED)
list(APPEND XRT_DEPS_LIBS asio::asio)


#------------------ Vendor packages ------------------------------------------------------------------------------------

#------------------ indicators -----------------------------------------------------------------------------------------

list(APPEND XRT_DEPS_INCLUDE_DIR vendor/indicators)

# TO DO: FIND HOW TO BUNDLE THESE.

#------------------ System packages ------------------------------------------------------------------------------------

#------------------ dyncall --------------------------------------------------------------------------------------------

list(APPEND XRT_DEPS_LIBS dyncall_s dynload_s)

#------------------ Vivado ---------------------------------------------------------------------------------------------

if(DEFINED ENV{XILINX_VIVADO})
    message("-- XILINX_VIVADO defined as environment variable: " $ENV{XILINX_VIVADO})
    set(VIVADO_INCLUDE_DIR $ENV{XILINX_VIVADO}/data/xsim/include/)
else()
    file(GLOB_RECURSE VIVADO_XSI /opt/Xilinx/Vivado/*/xsi.h /tools/Xilinx/Vivado/*/xsi.h)
    if(VIVADO_XSI)
        cmake_path(GET VIVADO_XSI PARENT_PATH VIVADO_INCLUDE_DIR)
    else()
        message( FATAL_ERROR "Can't find xsi.h. Make sure you either declare XILINX_VIVADO environment variable or install Vivado în /opt/ or /tools/, CMake will exit." )
    endif()
endif()

list(APPEND XRT_DEPS_INCLUDE_DIR ${VIVADO_INCLUDE_DIR})


#------------------ All local sources ----------------------------------------------------------------------------------
file(
    GLOB_RECURSE
    XRT_EXE_SOURCE_FILES
    
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/sources/*.cpp
    ${PROJECT_SOURCE_DIR}/src/transformers/*.cpp
)

file(
    GLOB_RECURSE
    XRT_CORE_SOURCE_FILES
    
    ${PROJECT_SOURCE_DIR}/src/common/*.cpp
    ${PROJECT_SOURCE_DIR}/src/manager/*.cpp
    ${PROJECT_SOURCE_DIR}/src/targets/*.cpp
    ${PROJECT_SOURCE_DIR}/src/exported/*.cpp

)

#------------------ xrtcore library -----------------------------------------------------------------------------------


add_library(xrtcore SHARED ${XRT_CORE_SOURCE_FILES})

target_include_directories(xrtcore PUBLIC
    ${XRT_DEPS_INCLUDE_DIR}

    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(xrtcore PUBLIC
    ${XRT_DEPS_LIBS}
)

# target_compile_definitions(xrtcore PUBLIC ONNX_NAMESPACE=onnx ONNX_ML=1)


#if(DEFINED XRT_FULL_IO_LOG)
if(True)
    target_compile_definitions(xrtcore PUBLIC XRT_FULL_IO_LOG)
endif()

if(DEFINED XRT_VERSION)
    target_compile_definitions(xrtcore PRIVATE XRT_VERSION=${XRT_VERSION})
endif()

set_property(TARGET xrtcore PROPERTY POSITION_INDEPENDENT_CODE ON)

#------------------ midlevel library -----------------------------------------------------------------------------------

set(XRT_API_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include/exported/midlevel ${PROJECT_SOURCE_DIR}/include)

if(NOT DEFINED ENV{XPU_LIBRARIES_PATH})
    message(INFO "XPU_LIBRARIES_PATH is not defined - skipping midlevel")
else()
    if(NOT XRT_SKIP_MIDLEVEL)
        add_subdirectory($ENV{XPU_LIBRARIES_PATH}/mid_level mid_level)
    endif()
endif()

#------------------ Executable -----------------------------------------------------------------------------------------
if(NOT XRT_SKIP_XRT)
    add_executable(xrt ${XRT_EXE_SOURCE_FILES})

    target_link_libraries(xrt xrtcore)

    set_property(TARGET xrt PROPERTY BUILD_RPATH_USE_ORIGIN TRUE)
endif()

#------------------ Install script -------------------------------------------------------------------------------------
configure_file(cmake/install-xrt.sh ${CMAKE_BINARY_DIR}/install-xrt.sh COPYONLY)
