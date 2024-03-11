//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include <transformers/midlevel/library/MidLevelFunction.hpp>
#include <optional>
#include <stdexcept>

#include <common/Utils.hpp>
#include "common/types/Types.hpp"
#include "magic_enum.hpp"
#include <dyncall.h>
#include <fmt/format.h>
#include <dynload.h>

//-------------------------------------------------------------------------------------
MidLevelFunctionReturn::MidLevelFunctionReturn(RuntimeType _type) : type(_type)
{}

//-------------------------------------------------------------------------------------
MidLevelFunction::MidLevelFunction(DLLib* _library, const char *_descriptionSymbol) {
    initFromDescriptionSymbol(_library, _descriptionSymbol);
}

//-------------------------------------------------------------------------------------
void MidLevelFunction::call(Manager* _manager, std::vector<void*> _args) {
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
void MidLevelFunction::initFromDescriptionSymbol(DLLib *_library, const char *_descriptionSymbol) {
    const char *_description = (const char *) dlFindSymbol(_library, _descriptionSymbol);

    if (_description == nullptr) {
        throw std::runtime_error(fmt::format("Symbol {} not found", _descriptionSymbol));
    }

    if (!beginsWith(_descriptionSymbol, cMidLevelDescriptionSymbolPrefix)) {
        throw std::runtime_error(fmt::format("Symbol {} is not a description symbol (expected {} prefix)", _descriptionSymbol, cMidLevelDescriptionSymbolPrefix));
    }

    const char *_functionName = _descriptionSymbol + strlen(cMidLevelDescriptionSymbolPrefix);

    name = _functionName;

    initParseDescription(_description);
}

//-------------------------------------------------------------------------------------
void MidLevelFunction::initParseDescription(const char *_descriptionCStr) {
    std::string _description = _descriptionCStr;
    std::string _delimiter = " ,";

    size_t _last = 0;
    size_t _next;
    std::string _token;
    while ((_next = _description.find(_delimiter, _last)) != std::string::npos) {
        params.emplace_back(_description.substr(_last, _next - _last));
        _last = _next + _delimiter.size();
    }

    return_ = MidLevelFunctionReturn(_description.substr(_last));
}

//-------------------------------------------------------------------------------------
MidLevelFunctionReturn::MidLevelFunctionReturn(std::string_view _s) {
    if (_s == "void") {
        type = std::nullopt;
    } else {
        throw std::runtime_error(fmt::format("Unknown type \"{}\" in function return declaration", _s));
    }
}

//-------------------------------------------------------------------------------------
MidLevelFunctionParam::MidLevelFunctionParam(std::string_view _s) {
    if (beginsWith(_s, "INOUT")) {
        driection = MidLevelParamDirection::InOut;
    } else if (beginsWith(_s, "IN")) {
        driection = MidLevelParamDirection::In;
    } else if (beginsWith(_s, "OUT")) {
        driection = MidLevelParamDirection::Out;
    } else {
        throw std::runtime_error(fmt::format("Function paramater \"{}\" does not specify IN/OUT/INOUT", _s));
    }

    _s = std::find_if(_s.begin(), _s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    });

    if (_s == "Matrix") {
        type = RuntimeType::Matrix;
    } else if (_s == "MatrixView") {
        type = RuntimeType::MatrixView;
    } else {
        throw std::runtime_error(fmt::format("Function paramater \"{}\" type not known", _s));
    }
}

//-------------------------------------------------------------------------------------

std::string MidLevelFunction::toString() const {
    std::string _s = fmt::format("{} {}(", return_.toString(), name);

    bool first = true;
    for (const auto& _param : params) {
        if (!first) {
            _s += ", ";
            first = false;
        }

        _s += _param.toString();
    }

    _s += ")";
    
    return _s;
}

//-------------------------------------------------------------------------------------

std::string MidLevelFunctionParam::toString() const {
    std::string _directionStr(magic_enum::enum_name(driection));

    // to uppercase
    std::transform(_directionStr.begin(), _directionStr.end(), _directionStr.begin(), [](char i)->char{return std::toupper(i);});

    return fmt::format("{} {}", _directionStr, magic_enum::enum_name(type));
}

//-------------------------------------------------------------------------------------

std::string MidLevelFunctionReturn::toString() const {
    return (type == std::nullopt) ? "void" : std::string(magic_enum::enum_name(type.value()));
}

//-------------------------------------------------------------------------------------