//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include <transformers/midlevel/library/MidLevelFunction.hpp>

#include <dyncall.h>

//-------------------------------------------------------------------------------------
MidLevelFunction::MidLevelFunction(
    void* _address, std::optional<MidLevelFunctionReturn> _return, std::vector<MidLevelFunctionParam>&& _params)
    : address(_address), return_(_return), params(std::move(_params)){};

//-------------------------------------------------------------------------------------
void MidLevelFunction::invoke(Manager* _manager, std::vector<void*> _args) {
    DCCallVM* vm = dcNewCallVM(4096);
    dcMode(vm, DC_CALL_C_DEFAULT);
    dcReset(vm);

    dcArgPointer(vm, (void*) _manager);

    for (void* _arg : _args) {
        dcArgPointer(vm, _arg);
    }

    dcCallVoid(vm, address);

    dcFree(vm);
}

//-------------------------------------------------------------------------------------
