//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include "manager/libmanager/FunctionInfo.hpp"
#include <any>
#include <manager/libmanager/InternalLibraryLoader.h>
#include <targets/common//CodeGen.h>


std::vector<uint32_t> InternalLibraryLoader::stickyHaltFunctionCode = {
    INSTR_chalt, INSTR_nop,
};
FunctionInfo InternalLibraryLoader::stickyHaltFunction = {
    .length = static_cast<uint32_t>(stickyHaltFunctionCode.size()),
    .name = "__xpu_builtin_hlt_at_zero",
    .address = 0xFFFF'FFFF,
    .code = stickyHaltFunctionCode.data(),
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
InternalLibraryLoader::InternalLibraryLoader()
    : stickyFunctions{stickyHaltFunction} {
    //    directTransformer = _directTransformer;
    //    std::cout << "Loading internal lib..." << std::endl;
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
