//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <cstdint>
#include <string>

#define XRT_LOGO "XRT v0.1"

#define TO_STR(X)       #X
#define MACRO_TO_STR(X) TO_STR(X)

constexpr const char* generateBuildInfo() {
    return

#if defined(__clang__)
    "Clang (" MACRO_TO_STR(__clang_major__) "." MACRO_TO_STR(__clang_minor__) "." MACRO_TO_STR(__clang_patchlevel__) ")"
#elif defined(__GNUC__)
    "GCC (" MACRO_TO_STR(__GNUC__) "." MACRO_TO_STR(__GNUC_MINOR__) ")"
#else
#error "Not a known compiler"
#endif

" with "

#if defined(__GLIBCXX__)
    "libstdc++ (" MACRO_TO_STR(__GLIBCXX__) ")"
#elif defined(_LIBCPP_VERSION)
    "libc++ (" MACRO_TO_STR(_LIBCPP_VERSION) " ABI " MACRO_TO_STR(_LIBCPP_ABI_VERSION) ")"
#endif
            ;
}

extern std::string cVersionString;

constexpr bool cClearMemoryAtStartup          = false;
constexpr uint32_t cClearMemoryAtStartupValue = 0;
//-------------------------------------------------------------------------------------
