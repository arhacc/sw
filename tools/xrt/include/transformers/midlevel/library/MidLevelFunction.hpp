//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <common/types/Types.hpp>

#include <optional>
#include <vector>

class Manager;

enum class MidLevelFunctionParamDirection {
    In,
    Out,
    InOut,
};

struct MidLevelFunctionParam {
    RuntimeType type;
    MidLevelFunctionParamDirection driection;
};

struct MidLevelFunctionReturn {
    RuntimeType type;
};

class MidLevelFunction {
    void* address;
    std::optional<MidLevelFunctionReturn> return_;
    std::vector<MidLevelFunctionParam> params;

  public:
    MidLevelFunction(
        void* _address, std::optional<MidLevelFunctionReturn> _return, std::vector<MidLevelFunctionParam>&& _params);
    ~MidLevelFunction() = default;

    void invoke(Manager* manager, std::vector<void*> _args);
};
