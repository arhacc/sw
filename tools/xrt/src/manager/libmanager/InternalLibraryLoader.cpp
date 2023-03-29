//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <manager/libmanager/InternalLibraryLoader.h>

//-------------------------------------------------------------------------------------
void do_adjusted_input() {
    std::cout << "[do_adjusted_input]: " << std::endl;
}

//-------------------------------------------------------------------------------------
void do_convolution_output() {
    std::cout << "[do_convolution_output]: " << std::endl;
}

//-------------------------------------------------------------------------------------
InternalLibraryLoader::InternalLibraryLoader() {
    //    directTransformer = _directTransformer;
    //    std::cout << "Loading internal lib..." << std::endl;
    functionMap["adjusted_input"] = do_adjusted_input;
    functionMap["convolution_output"] = do_convolution_output;
}

//-------------------------------------------------------------------------------------
FunctionInfo *InternalLibraryLoader::resolve(const std::string &_name) {
    auto _iterator = functionMap.find(_name);
    if (_iterator == functionMap.end()) {
        return nullptr;
    }

    return (FunctionInfo *) (&_iterator->second);//std::any_cast <void (*) ()> (_iterator->second) ();
}


//-------------------------------------------------------------------------------------
