//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <manager/libmanager/InternalLibraryLoader.h>
//#include <targets/fpga/FpgaTarget.h>

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
    std::cout << "Loading internal lib..." << std::endl;
    functionMap["adjusted_input"] = do_adjusted_input;
    functionMap["convolution_output"] = do_convolution_output;
}

//-------------------------------------------------------------------------------------
FunctionInfo* InternalLibraryLoader::resolve(std::string _name) {
    std::unordered_map<std::string, std::any>::const_iterator _iterator = functionMap.find(_name);
    if(_iterator == functionMap.end()){
        return NULL;
//        FunctionInfo* _functionInfo = jsonLibraryLoader -> getFunction(_name);
//        if(_functionInfo != NULL){
//            return _functionInfo;
 //       }
    } else {
        return (FunctionInfo*)(&_iterator -> second);//std::any_cast <void (*) ()> (_iterator->second) ();
    }
//    return NULL;
}


//-------------------------------------------------------------------------------------
