cmake_minimum_required(VERSION 3.18)
project(xrt VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "-Wall -ansi -pedantic -Wno-pointer-arith -Wsign-compare -fPIE -fPIC -fuse-ld=bfd")



set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")


include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_support_error)

if(ipo_supported)
    set(INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
else()
    message(STATUS "IPO is not supported: ${ipo_support_error}")
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_STANDARD 23)
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
set(XPU_DEBUG_VERBOSITY_LEVEL "XPU_DEBUG_VERBOSITY_LEVEL_FULL" CACHE STRING "Verbosity level for printing during runs" )

find_package(PkgConfig REQUIRED)

#------------------ Enable CPM package manager -------------------------------------------------------------------------

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/CPM.cmake")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#------------------ fmt ------------------------------------------------------------------------------------------------

CPMAddPackage(
    NAME fmt
    GITHUB_REPOSITORY fmtlib/fmt
    GIT_TAG 11.0.1
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
    GIT_TAG v14.2.5
)

list(APPEND XRT_DEPS_LIBS reproc++)

#------------------ magic_enum -----------------------------------------------------------------------------------------

CPMAddPackage(
    NAME magic_enum
    GITHUB_REPOSITORY Neargye/magic_enum
    GIT_TAG v0.9.6
)

list(APPEND XRT_DEPS_LIBS magic_enum::magic_enum)

#------------------ endian ---------------------------------------------------------------------------------------------

CPMAddPackage(
    NAME endian
    GITHUB_REPOSITORY steinwurf/endian
    GIT_TAG 13.0.0
)

list(APPEND XRT_DEPS_LIBS steinwurf::endian)

#------------------ sockpp ---------------------------------------------------------------------------------------------

set(SOCKPP_BUILD_SHARED OFF)
set(SOCKPP_BUILD_STATIC ON)

CPMAddPackage(
    NAME sockpp
    GITHUB_REPOSITORY fpagliughi/sockpp
    GIT_TAG v1.0.0
)

list(APPEND XRT_DEPS_LIBS sockpp-static)

#------------------ googletest -----------------------------------------------------------------------------------------

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

list(APPEND XRT_DEPS_LIBS GTest::gtest)

#------------------ gsl ------------------------------------------------------------------------------------------------

include(FetchContent)
FetchContent_Declare(GSL
        GIT_REPOSITORY "https://github.com/microsoft/GSL"
        GIT_TAG "v4.2.0"
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(GSL)

list(APPEND XRT_DEPS_LIBS Microsoft.GSL::GSL)

#------------------ System packages -------------------------------------------------------------------------------------

#------------------ OpenSSL --------------------------------------------------------------------------------------------

pkg_check_modules(OpenSSL REQUIRED IMPORTED_TARGET openssl libcrypto)

list(APPEND XRT_DEPS_LIBS PkgConfig::OpenSSL)

#------------------ readline -------------------------------------------------------------------------------------------

pkg_check_modules(Readline REQUIRED IMPORTED_TARGET readline ncurses)

list(APPEND XRT_DEPS_LIBS PkgConfig::Readline)

#------------------ Vendor packages ------------------------------------------------------------------------------------

#------------------ indicators -----------------------------------------------------------------------------------------

list(APPEND XRT_DEPS_INCLUDE_DIR vendor/indicators)

#------------------ dyncall --------------------------------------------------------------------------------------------

add_subdirectory(vendor/dyncall)

list(APPEND XRT_DEPS_LIBS dyncall_s dynload_s dl)
list(APPEND XRT_DEPS_INCLUDE_DIR vendor/dyncall/dyncall vendor/dyncall/dynload)

#------------------ Vivado ---------------------------------------------------------------------------------------------

list(APPEND XRT_DEPS_INCLUDE_DIR vendor/xsi)


#------------------ All local sources ----------------------------------------------------------------------------------
set(XRT_MAIN_FILE ${PROJECT_SOURCE_DIR}/src/main.cpp)

file(
    GLOB_RECURSE
    XRT_EXE_SOURCE_FILES
    
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

    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(xrtcore PUBLIC
    ${XRT_DEPS_LIBS}
)

target_compile_definitions(xrtcore PUBLIC XPU_DEBUG_VERBOSITY_LEVEL=${XPU_DEBUG_VERBOSITY_LEVEL})

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

set(XRT_API_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/src/exported/midlevel ${PROJECT_SOURCE_DIR}/src)

if(NOT DEFINED ENV{XPU_LIBRARIES_PATH})
    message(INFO "XPU_LIBRARIES_PATH is not defined - skipping midlevel")
else()
    if(NOT XRT_SKIP_MIDLEVEL)
        add_subdirectory($ENV{XPU_LIBRARIES_PATH}/mid_level mid_level)
    endif()
endif()

#------------------ Executable -----------------------------------------------------------------------------------------
if(NOT XRT_SKIP_XRT)
  add_executable(xrt ${XRT_MAIN_FILE} ${XRT_EXE_SOURCE_FILES})

  target_link_libraries(xrt PRIVATE xrtcore)

  set_property(TARGET xrt PROPERTY BUILD_RPATH_USE_ORIGIN TRUE)
endif()

#------------------ Test executable -----------------------------------------------------------------------------------------
add_executable(xrt-unit-tests ${XRT_EXE_SOURCE_FILES})

target_link_libraries(xrt-unit-tests PRIVATE xrtcore GTest::gtest_main)

set_property(TARGET xrt-unit-tests PROPERTY BUILD_RPATH_USE_ORIGIN TRUE)


#------------------ Install script -------------------------------------------------------------------------------------
configure_file(cmake/install-xrt.sh ${CMAKE_BINARY_DIR}/install-xrt.sh COPYONLY)
