//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <common/Constants.hpp>

#define TO_STR(X)       #X
#define MACRO_TO_STR(X) TO_STR(X)

#ifndef XRT_VERSION
std::string cVersionString = "devel";
#else
std::string cVersionString = MACRO_TO_STR(X);
#endif
