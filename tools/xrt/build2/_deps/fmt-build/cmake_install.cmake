# Install script for directory: /home/vlad/.cache/CPM/fmt/c77b38c6a3f42c882bb968b232e2ea7085d475fd

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Debug")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/vlad/work/xpu/sw/tools/xrt/build2/_deps/fmt-build/libfmtd.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/fmt" TYPE FILE FILES
    "/home/vlad/.cache/CPM/fmt/c77b38c6a3f42c882bb968b232e2ea7085d475fd/include/fmt/args.h"
    "/home/vlad/.cache/CPM/fmt/c77b38c6a3f42c882bb968b232e2ea7085d475fd/include/fmt/chrono.h"
    "/home/vlad/.cache/CPM/fmt/c77b38c6a3f42c882bb968b232e2ea7085d475fd/include/fmt/color.h"
    "/home/vlad/.cache/CPM/fmt/c77b38c6a3f42c882bb968b232e2ea7085d475fd/include/fmt/compile.h"
    "/home/vlad/.cache/CPM/fmt/c77b38c6a3f42c882bb968b232e2ea7085d475fd/include/fmt/core.h"
    "/home/vlad/.cache/CPM/fmt/c77b38c6a3f42c882bb968b232e2ea7085d475fd/include/fmt/format.h"
    "/home/vlad/.cache/CPM/fmt/c77b38c6a3f42c882bb968b232e2ea7085d475fd/include/fmt/format-inl.h"
    "/home/vlad/.cache/CPM/fmt/c77b38c6a3f42c882bb968b232e2ea7085d475fd/include/fmt/os.h"
    "/home/vlad/.cache/CPM/fmt/c77b38c6a3f42c882bb968b232e2ea7085d475fd/include/fmt/ostream.h"
    "/home/vlad/.cache/CPM/fmt/c77b38c6a3f42c882bb968b232e2ea7085d475fd/include/fmt/printf.h"
    "/home/vlad/.cache/CPM/fmt/c77b38c6a3f42c882bb968b232e2ea7085d475fd/include/fmt/ranges.h"
    "/home/vlad/.cache/CPM/fmt/c77b38c6a3f42c882bb968b232e2ea7085d475fd/include/fmt/std.h"
    "/home/vlad/.cache/CPM/fmt/c77b38c6a3f42c882bb968b232e2ea7085d475fd/include/fmt/xchar.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt" TYPE FILE FILES
    "/home/vlad/work/xpu/sw/tools/xrt/build2/_deps/fmt-build/fmt-config.cmake"
    "/home/vlad/work/xpu/sw/tools/xrt/build2/_deps/fmt-build/fmt-config-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt/fmt-targets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt/fmt-targets.cmake"
         "/home/vlad/work/xpu/sw/tools/xrt/build2/_deps/fmt-build/CMakeFiles/Export/lib/cmake/fmt/fmt-targets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt/fmt-targets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt/fmt-targets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt" TYPE FILE FILES "/home/vlad/work/xpu/sw/tools/xrt/build2/_deps/fmt-build/CMakeFiles/Export/lib/cmake/fmt/fmt-targets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/fmt" TYPE FILE FILES "/home/vlad/work/xpu/sw/tools/xrt/build2/_deps/fmt-build/CMakeFiles/Export/lib/cmake/fmt/fmt-targets-debug.cmake")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig" TYPE FILE FILES "/home/vlad/work/xpu/sw/tools/xrt/build2/_deps/fmt-build/fmt.pc")
endif()

