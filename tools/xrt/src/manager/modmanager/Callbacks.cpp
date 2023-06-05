//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include "common/arch/Arch.hpp"
#include "common/cache/Cache.h"
#include "targets/Targets.h"
#include <manager/libmanager/FunctionInfo.hpp>
#include <manager/Manager.h>
#include <manager/modmanager/Callbacks.h>
#include <vector>

//-------------------------------------------------------------------------------------
#ifndef XRT_DYNAMIC_LOW_LEVEL
extern "C"
void *xpu_init(bool _enableFpgaTarget, bool _enableSimTarget, bool _enableGoldenModelTarget) {
    const Arch &_arch = parseArchFile();
    Targets *_targets = new Targets(_arch, {}, _enableFpgaTarget, _enableSimTarget, _enableGoldenModelTarget);
    Manager *_manager = new Manager(_targets, new Cache, _arch);

    return reinterpret_cast<void *>(_manager);
}
#endif

//-------------------------------------------------------------------------------------
extern "C"
void xpu_load(void *_ctx, const char *_path) {
    Manager *_manager = static_cast<Manager *>(_ctx);

    try {
        _manager->load(std::string(_path));
    } catch(std::exception& e) {
        std::cout << "Exception in load callback: " << e.what() << std::endl;
    } catch(...) {
        std::cout << "Unidentified exception in load callback" << std::endl;
    }
}

//-------------------------------------------------------------------------------------
extern "C"
void xpu_runRuntime(void *_ctx, void *_functionPtr, uint32_t _argc, uint32_t *_argv) {
    Manager *_manager = static_cast<Manager *>(_ctx);
    auto _functionInfo = static_cast<FunctionInfo*>(_functionPtr);

    try {
        _manager->runRuntime(_functionInfo, _argc, _argv);
    } catch(std::exception& e) {
        std::cout << "Exception in runRuntime callback: " << e.what() << std::endl;
    } catch(...) {
        std::cout << "Unidentified exception in runRuntime callback" << std::endl;
    }
}

//-------------------------------------------------------------------------------------
extern "C"
void *xpu_lowLevel(void *_ctx, const char *_path) {
    Manager *_manager = static_cast<Manager *>(_ctx);

    try {
        FunctionInfo* _function = _manager->lowLevel(std::string(_path));

        return static_cast<void *>(_function);
    } catch(std::exception& e) {
        std::cout << "Exception in lowLevel callback: " << e.what() << std::endl;

        return nullptr;
    } catch(...) {
        std::cout << "Unidentified exception in lowLevel callback" << std::endl;

        return nullptr;
    }
}

//-------------------------------------------------------------------------------------
extern "C"
void xpu_readMatrixArray(void *_ctx, uint32_t _accMemStart,
                             uint32_t _numLine, uint32_t _numColumn,
                             int      _accRequireResultReady,
                             uint32_t *_ramMatrix,
                             uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                             uint32_t _ramStartLine, uint32_t _ramStartColumn) {
    Manager *_manager = static_cast<Manager *>(_ctx);

    try {
        _manager->readMatrixArray(_accMemStart, _numLine, _numColumn, _accRequireResultReady, _ramMatrix, _ramTotalLines, _ramTotalColumns, _ramStartLine, _ramStartColumn);
    } catch(std::exception& e) {
        std::cout << "Exception in readMatrixArray callback: " << e.what() << std::endl;
    } catch(...) {
        std::cout << "Unidentified exception in readMatrixArray callback" << std::endl;
    }
}

//-------------------------------------------------------------------------------------
extern "C"
void xpu_writeMatrixArray(void *_ctx, uint32_t *_ramMatrix,
                              uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                              uint32_t _ramStartLine, uint32_t _ramStartColumn,
                              uint32_t _numLine, uint32_t _numColumn,
                              uint32_t _accMemStart) {
    Manager *_manager = static_cast<Manager *>(_ctx);

    try {
        _manager->writeMatrixArray(_ramMatrix, _ramTotalLines, _ramTotalColumns, _ramStartLine, _ramStartColumn, _numLine, _numColumn, _accMemStart);
    } catch(std::exception& e) {
        std::cout << "Exception in writeMatrixArray callback: " << e.what() << std::endl;
    } catch(...) {
        std::cout << "Unidentified exception in writeMatrixArray callback" << std::endl;
    }
}

//-------------------------------------------------------------------------------------
