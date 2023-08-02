//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <manager/libmanager/midlevel/ModFunctionInfo.hpp>

#include <vector>

//-------------------------------------------------------------------------------------
std::vector<ModFunctionInfo> getKnownModFunctions() {
    return {
        ModFunctionInfo{
            .name = "xpu_simpleFunction",
            .args = std::vector<ModFunctionArgument>{},
        },
    };
}

//-------------------------------------------------------------------------------------
