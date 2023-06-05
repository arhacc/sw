//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <manager/modmanager/ModFunctionInfo.hpp>


//-------------------------------------------------------------------------------------
std::vector<ModFunctionInfo> getKnownModFunctions() {
    return {
        {
            .name = "matrixMultiply",
            .args = {
                {
                    .type = ModFunctionArgumentType::Matrix,
                    .primitive = {
                        .type = ModFunctionArgumentType::UInteger,
                        .size = 4, // 32 bits
                    },
                },
                {
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
