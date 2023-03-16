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
LibManager::LibManager(MemManager* _memManager) {
    memManager = _memManager;
    std::cout << "Loading libraries..." << std::endl;
    internalLibraryLoader = new InternalLibraryLoader();
    jsonLibraryLoader = new JsonLibraryLoader();
    jsonLibraryLoader -> load("../lib/lowlevel.json");
    uploadFunction("result_ready");
    uploadFunction("wait_matrix");
}

//-------------------------------------------------------------------------------------
LibManager::~LibManager() {

}

//-------------------------------------------------------------------------------------
void LibManager::load(std::string _path) {
    jsonLibraryLoader -> load(_path);
}

//-------------------------------------------------------------------------------------
FunctionInfo* LibManager::resolve(std::string _name) {
/*    std::unordered_map<std::string, std::any>::const_iterator _iterator = internallyResolvedFunctionMap.find(_name);
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
    }*/
    FunctionInfo* _functionInfo = internalLibraryLoader->resolve(_name);
    if(_functionInfo != NULL){
        return _functionInfo;
    }
    _functionInfo = jsonLibraryLoader->resolve(_name);
    if(_functionInfo == NULL){
        std::cout << "Error 001: Cannot resolve function: " << _name << std::endl;
        exit(1);
    }
    return _functionInfo;
}

//-------------------------------------------------------------------------------------
uint32_t LibManager::uploadFunction(std::string _name) {
    FunctionInfo* _functionInfo = resolve(_name);
    if(_functionInfo == NULL){
        std::cout << "Error 002: Cannot resolve function: " << _name << std::endl;
        exit(1);
    }
    if(_functionInfo -> address != -1){
        return _functionInfo -> address;
    }
    _functionInfo -> address = uploadCode(_functionInfo -> code, _functionInfo -> length);
}

//-------------------------------------------------------------------------------------
uint32_t LibManager::uploadCode(uint32_t* _code, uint32_t _length){

}

//-------------------------------------------------------------------------------------
uint32_t LibManager::uploadData(uint32_t* _data, uint32_t _length){

}

//-------------------------------------------------------------------------------------
