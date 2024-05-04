//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/CodeGen.hpp>
#include <common/arch/Arch.hpp>
#include <manager/libmanager/InternalLibraryLoader.hpp>

#include <any>
#include <cstdint>
#include <memory>
#include <vector>

//-------------------------------------------------------------------------------------
std::vector<uint32_t> InternalLibraryLoader::stickyHaltFunctionCode(const Arch& _arch) {
    return {
        _arch.INSTR_chalt,
        _arch.INSTR_nop,
    };
}

//-------------------------------------------------------------------------------------
std::unique_ptr<LowLevelFunctionInfo> InternalLibraryLoader::stickyHaltFunction(const Arch& _arch) {
    return std::make_unique<LowLevelFunctionInfo>(LowLevelFunctionInfo{
        .code    = stickyHaltFunctionCode(_arch),
        .name    = "__xpu_builtin_hlt_at_zero",
        .address = 0xFFFF'FFFF,
    });
};

//-------------------------------------------------------------------------------------
std::vector<std::unique_ptr<LowLevelFunctionInfo>> InternalLibraryLoader::stickyFunctionsToLoad(const Arch& _arch) {
    std::vector<std::unique_ptr<LowLevelFunctionInfo>> stickyFunctions;

    stickyFunctions.push_back(stickyHaltFunction(_arch));

    std::cout << "Loading internal library" << std::endl;
    return stickyFunctions;
}

//-------------------------------------------------------------------------------------
