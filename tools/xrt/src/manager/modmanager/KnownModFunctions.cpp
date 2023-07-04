//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <manager/modmanager/ModFunctionInfo.hpp>
#include <vector>


//-------------------------------------------------------------------------------------
std::vector<ModFunctionInfo> getKnownModFunctions() {
    return {
        ModFunctionInfo{
            .name = "matrixMultiply",
            .args = std::vector<ModFunctionArgument>{
                ModFunctionArgument{
                    .type = ModFunctionArgumentType::Matrix,
                    .primitive = {
                        .type = ModFunctionArgumentType::UInteger,
                        .size = 4, // 32 bits
                    },
                },
                ModFunctionArgument{
                    .type = ModFunctionArgumentType::Matrix,
                    .primitive = {
                        .type = ModFunctionArgumentType::UInteger,
                        .size = 4, // 32 bits
                    },
                },
            },
        },
    };
}


//-------------------------------------------------------------------------------------
