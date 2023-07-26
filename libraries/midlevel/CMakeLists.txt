# This file is made to be included by the CMakeLists.txt in xrt
project(xpumidlevel VERSION 0.1.0)

set(XRT_API_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/api/include)

set(
    XPU_MID_LEVEL_SOURCES

    ${PROJECT_SOURCE_DIR}/api/src/callbackTable.c

    ${PROJECT_SOURCE_DIR}/functions/SimpleFunction/SimpleFunction.cpp
)

set(
    XPU_MID_LEVEL_INCLUDE_DIR

    ${PROJECT_SOURCE_DIR}/functions/include
)

add_library(xpumidlevel MODULE ${XPU_MID_LEVEL_SOURCES})
target_compile_definitions(xpumidlevel PRIVATE XRT_DYNAMIC_MID_LEVEL)
target_include_directories(xpumidlevel PRIVATE ${XRT_API_INCLUDE_DIR})

add_library(xpumidlevel-static STATIC ${XPU_MID_LEVEL_SOURCES}  $<TARGET_OBJECTS:xrtcore>)
target_include_directories(xpumidlevel-static PRIVATE ${XRT_API_INCLUDE_DIR})

if(XRT_TESTS)
    add_subdirectory(tests/test0)
    add_subdirectory(tests/test1)
    add_subdirectory(tests/test2)
    add_subdirectory(tests/test3)
    add_subdirectory(tests/test4)
    add_subdirectory(tests/test5)

    configure_file(tests/all_tests.sh ${CMAKE_BINARY_DIR}/bin/all_tests.sh COPYONLY)
endif()
