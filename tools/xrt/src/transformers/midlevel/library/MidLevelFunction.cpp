//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#include <common/Utils.hpp>
#include <common/log/Logger.hpp>
#include <common/types/Types.hpp>
#include <transformers/midlevel/library/MidLevelFunction.hpp>

#include <optional>
#include <stdexcept>
#include <string_view>

#include <dyncall.h>
#include <dynload.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <magic_enum.hpp>

//-------------------------------------------------------------------------------------
MidLevelFunction::MidLevelFunction(DLLib* _library, const char* _descriptionSymbol, Manager* _manager)
    : manager(_manager) {
    initFromDescriptionSymbol(_library, _descriptionSymbol);
}

//-------------------------------------------------------------------------------------
void MidLevelFunction::call(std::vector<void*> _args) {
    DCCallVM* vm = dcNewCallVM(4096);
    dcMode(vm, DC_CALL_C_DEFAULT);
    dcReset(vm);

    dcArgPointer(vm, (void*) manager);

    for (void* _arg : _args) {
        dcArgPointer(vm, _arg);
    }

    dcCallVoid(vm, address);

    dcFree(vm);
}

//-------------------------------------------------------------------------------------
void MidLevelFunction::initFromDescriptionSymbol(DLLib* _library, const char* _descriptionSymbol) {
    const char* _description = (const char*) dlFindSymbol(_library, _descriptionSymbol);

    if (_description == nullptr) {
        throw std::runtime_error(fmt::format("Symbol {} not found", _descriptionSymbol));
    }

    if (!beginsWith(_descriptionSymbol, cMidLevelDescriptionSymbolPrefix)) {
        throw std::runtime_error(fmt::format(
            "Symbol {} is not a description symbol (expected {} prefix)",
            _descriptionSymbol,
            cMidLevelDescriptionSymbolPrefix));
    }

    const char* _functionName = _descriptionSymbol + strlen(cMidLevelDescriptionSymbolPrefix);

    name = _functionName;
    address = dlFindSymbol(_library, _functionName);


    // TODO: GENERALIZE
    if (name == "mat_mat_mult_dot") {
        onnxOperator = "MatMul";
        fmt::println("Found operator for MatMul");
    } else if (name == "mat_mat_add_hadamard") {
	onnxOperator = "Add";
        fmt::println("Found operator for Add");
    }
	

    initParseDescription(_description);

    logWork.print(fmt::format("Found mid level function {} at address {}\n", toString(), address));
}

//-------------------------------------------------------------------------------------
void MidLevelFunction::initParseDescription(const char* _descriptionCStr) {
    std::string _description = _descriptionCStr;
    std::string _delimiter   = ", ";

    size_t _last = 0;
    size_t _next;
    std::string _token;
    while ((_next = _description.find(_delimiter, _last)) != std::string::npos) {
        params.emplace_back(_description.substr(_last, _next - _last));
        _last = _next + _delimiter.size();
    }
    params.emplace_back(_description.substr(_last));
}

//-------------------------------------------------------------------------------------
MidLevelFunctionParam::MidLevelFunctionParam(std::string_view _s) {
    std::string_view::const_iterator it = _s.begin();

    if (beginsWith(_s, "INOUT")) {
        driection = MidLevelParamDirection::InOut;

        it += 5;
    } else if (beginsWith(_s, "IN")) {
        driection = MidLevelParamDirection::In;

        it += 2;
    } else if (beginsWith(_s, "OUT")) {
        driection = MidLevelParamDirection::Out;

        it += 3;
    } else {
        throw std::runtime_error(fmt::format("Function paramater \"{}\" does not specify IN/OUT/INOUT", _s));
    }

    _s = std::find_if(it, _s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    });

    if (_s == "Matrix") {
        type = RuntimeType::Matrix;
    } else if (_s == "MatrixView") {
        type = RuntimeType::MatrixView;
        // TODO: STANDARDIZE THIS
    } else if (_s == "UInt32" || _s == "uint32_t" || _s == "int32_t") {
        type = RuntimeType::UInt32;
    } else {
        throw std::runtime_error(fmt::format("Function paramater \"{}\" type not known", _s));
    }
}

//-------------------------------------------------------------------------------------

std::string MidLevelFunction::toString() const {
    std::string _s = fmt::format("{}(", name);

    bool first = true;
    for (const auto& _param : params) {
        if (!first) {
            _s += ", ";
        }

        _s += _param.toString();
        first = false;
    }

    _s += ")";

    return _s;
}

//-------------------------------------------------------------------------------------

std::string MidLevelFunctionParam::toString() const {
    std::string _directionStr(magic_enum::enum_name(driection));

    // to uppercase
    std::transform(_directionStr.begin(), _directionStr.end(), _directionStr.begin(), [](char i) -> char {
        return std::toupper(i);
    });

    return fmt::format("{} {}", _directionStr, magic_enum::enum_name(type));
}

//-------------------------------------------------------------------------------------
