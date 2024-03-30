//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <common/types/Types.hpp>

#include <optional>
#include <string>
#include <vector>

#include <dynload.h>

class Manager;

constexpr const char cMidLevelDescriptionSymbolPrefix[] = "_xpu_mid_level_decl_";

enum class MidLevelParamDirection {
    In,
    Out,
    InOut,
};

struct MidLevelFunctionParam {
    RuntimeType type;
    MidLevelParamDirection driection;

    MidLevelFunctionParam(std::string_view _s);

    std::string toString() const;
};

class MidLevelFunction {
    Manager* manager;
    void* address;
    std::string name;
    std::vector<MidLevelFunctionParam> params;

    void initFromDescriptionSymbol(DLLib* _library, const char* _descriptionSymbol);
    void initParseDescription(const char* _description);

  public:
    MidLevelFunction(DLLib* _library, const char* _descriptionSymbol, Manager* _manager);
    ~MidLevelFunction() = default;

    void call(std::vector<void*> _args);

    std::string toString() const;

    inline std::string_view getName() {
        return name;
    }
};
