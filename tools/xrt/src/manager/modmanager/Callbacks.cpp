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

extern "C"
void callbackReadMatrixArray(uint32_t _accMemStart,
                             uint32_t _numLine, uint32_t _numColumn,
                             int      _accRequireResultReady,
                             uint32_t *_ramMatrix,
                             uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                             uint32_t _ramStartLine, uint32_t _ramStartColumn) {

    try {
        callbackManager->readMatrixArray(_accMemStart, _numLine, _numColumn, _accRequireResultReady, _ramMatrix, _ramTotalLines, _ramTotalColumns, _ramStartLine, _ramStartColumn);
    } catch(std::exception& e) {
        std::cout << "Exception in readMatrixArray callback: " << e.what() << std::endl;
    } catch(...) {
        std::cout << "Unidentified exception in readMatrixArray callback" << std::endl;
    }
}

extern "C"
void callbackWriteMatrixArray(uint32_t *_ramMatrix,
                              uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                              uint32_t _ramStartLine, uint32_t _ramStartColumn,
                              uint32_t _numLine, uint32_t _numColumn,
                              uint32_t _accMemStart) {
    try {
        callbackManager->writeMatrixArray(_ramMatrix, _ramTotalLines, _ramTotalColumns, _ramStartLine, _ramStartColumn, _numLine, _numColumn, _accMemStart);
    } catch(std::exception& e) {
        std::cout << "Exception in writeMatrixArray callback: " << e.what() << std::endl;
    } catch(...) {
        std::cout << "Unidentified exception in writeMatrixArray callback" << std::endl;
    }
}
