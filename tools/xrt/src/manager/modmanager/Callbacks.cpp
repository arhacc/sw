//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <manager/libmanager/FunctionInfo.hpp>
#include <manager/Manager.h>
#include <manager/modmanager/Callbacks.h>

Manager *callbackManager = nullptr;

extern "C"
void callbackLoad(const char *_path) {
    try {
        callbackManager->load(std::string(_path));
    } catch(...) {}
}

extern "C"
void callbackRunRuntime(void *_functionPtr) {
    auto _functionInfo = static_cast<FunctionInfo*>(_functionPtr);

    try {
        callbackManager->runRuntime(_functionInfo);
    } catch(...) {}
}

extern "C"
void *callbackLowLevel(const char *_path) {
    try {
        FunctionInfo* _function = callbackManager->lowLevel(std::string(_path));

        return static_cast<void *>(_function);
    } catch(...) {
        return nullptr;
    }
}
