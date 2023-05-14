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
    } catch(std::exception& e) {
        std::cout << "Exception in load callback: " << e.what() << std::endl;
    } catch(...) {
        std::cout << "Unidentified exception in load callback" << std::endl;
    }
}

extern "C"
void callbackRunRuntime(void *_functionPtr) {
    auto _functionInfo = static_cast<FunctionInfo*>(_functionPtr);

    try {
        callbackManager->runRuntime(_functionInfo);
    } catch(std::exception& e) {
        std::cout << "Exception in runRuntime callback: " << e.what() << std::endl;
    } catch(...) {
        std::cout << "Unidentified exception in runRuntime callback" << std::endl;
    }
}

extern "C"
void *callbackLowLevel(const char *_path) {
    try {
        FunctionInfo* _function = callbackManager->lowLevel(std::string(_path));

        return static_cast<void *>(_function);
    } catch(std::exception& e) {
        std::cout << "Exception in lowLevel callback: " << e.what() << std::endl;

        return nullptr;
    } catch(...) {
        std::cout << "Unidentified exception in lowLevel callback" << std::endl;

        return nullptr;
    }
}
