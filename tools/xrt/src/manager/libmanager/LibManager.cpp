//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <any>
#include <map>
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdint>
#include <algorithm>
#include <string>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <manager/libmanager/LibManager.h>
#include <manager/libmanager/InternalLibraryLoader.cpp>
#include <manager/libmanager/JsonLibraryLoader.cpp>

//-------------------------------------------------------------------------------------
void do_adjusted_input() {
    std::cout << "[do_adjusted_input]: " << std::endl;    
}

//-------------------------------------------------------------------------------------
void do_convolution_output() {
    std::cout << "[do_convolution_output]: " << std::endl;
}


//-------------------------------------------------------------------------------------
LibManager::LibManager(MemManager* _memManager) {
    memManager = _memManager;
    internalLibraryLoader = new InternalLibraryLoader();
    jsonLibraryLoader = new JsonLibraryLoader();
    internallyResolvedFunctionMap["adjusted_input"] = do_adjusted_input;
    internallyResolvedFunctionMap["convolution_output"] = do_convolution_output;
}

//-------------------------------------------------------------------------------------
LibManager::~LibManager() {
}

//-------------------------------------------------------------------------------------
void LibManager::resolve(std::string _name) {
    std::unordered_map<std::string, std::any>::const_iterator _iterator = internallyResolvedFunctionMap.find(_name);
    if(_iterator == internallyResolvedFunctionMap.end()){
        FunctionInfo* _functionInfo = jsonLibraryLoader -> getFunction(_name);
        if(_functionInfo == NULL){
            std::cout << "Cannot resolve function: " << _name << std::endl;
//            exit(1);
            return;
        } else {
            jsonLibraryLoader -> writeFunction(_functionInfo);
        }
    } else {
        std::any_cast <void (*) ()> (_iterator->second) ();
    }
}

//-------------------------------------------------------------------------------------
void LibManager::uploadFunction(std::string _name) {

}

//-------------------------------------------------------------------------------------
void LibManager::uploadData(void* _address, uint32_t _length){

}

//-------------------------------------------------------------------------------------
