//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include "manager/libmanager/FunctionInfo.hpp"
#include <any>
#include <cstdint>
#include <manager/libmanager/InternalLibraryLoader.h>
#include <common/CodeGen.h>
#include <vector>


//-------------------------------------------------------------------------------------
std::vector<uint32_t> InternalLibraryLoader::stickyHaltFunctionCode(const Arch& _arch) {
    return {
        _arch.INSTR_chalt, _arch.INSTR_nop,
    };
}

//-------------------------------------------------------------------------------------
FunctionInfo InternalLibraryLoader::stickyHaltFunction(const Arch& _arch, std::vector<std::vector<uint32_t>>& _stickyFunctionsCode) {
    _stickyFunctionsCode.push_back(stickyHaltFunctionCode(_arch));

    auto &_stickyHaltFunctionCode = *(_stickyFunctionsCode.end() - 1);

    return {
        .length = static_cast<uint32_t>(_stickyHaltFunctionCode.size()),
        .name = "__xpu_builtin_hlt_at_zero",
        .address = 0xFFFF'FFFF,
        .code = _stickyHaltFunctionCode.data(),
    };
};

//-------------------------------------------------------------------------------------
void do_adjusted_input() {
    std::cout << "[do_adjusted_input]: " << std::endl;
}

//-------------------------------------------------------------------------------------
void do_convolution_output() {
    std::cout << "[do_convolution_output]: " << std::endl;
}

//-------------------------------------------------------------------------------------
InternalLibraryLoader::InternalLibraryLoader(const Arch& _arch)
    : arch(_arch), stickyFunctions{stickyHaltFunction(_arch, stickyFunctionsCode)} {
    
    std::cout << "Loading internal library" << std::endl;
    
    functionMap["adjusted_input"] = do_adjusted_input;
    functionMap["convolution_output"] = do_convolution_output;

    for (FunctionInfo& _function : stickyFunctions) {
        functionMap[_function.name] = _function;
    }
}

//-------------------------------------------------------------------------------------
FunctionInfo *InternalLibraryLoader::resolve(const std::string &_name) {
    auto _iterator = functionMap.find(_name);
    if (_iterator == functionMap.end()
        || _iterator->second.type() != typeid(FunctionInfo&)) {
        return nullptr;
    }

    return &std::any_cast<FunctionInfo&>(_iterator->second);
}

//-------------------------------------------------------------------------------------
std::vector<FunctionInfo>& InternalLibraryLoader::stickyFunctionsToLoad() {
    return stickyFunctions;
}


//-------------------------------------------------------------------------------------
