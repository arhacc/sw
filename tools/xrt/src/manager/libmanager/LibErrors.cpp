//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <manager/libmanager/LibErrors.hpp>

#include <fmt/format.h>

//-------------------------------------------------------------------------------------
LibNotFoundError::LibNotFoundError(const std::string& _name)
    : XrtException(
        fmt::format("library {} not found", _name), XrtErrorNumber::LIBRARY_NOT_FOUND) {}

//-------------------------------------------------------------------------------------
FunctionNotFoundError::FunctionNotFoundError(const std::string& _name)
    : XrtException(
        fmt::format("function {} not found", _name), XrtErrorNumber::FUNCTION_NOT_FOUND) {
}

//-------------------------------------------------------------------------------------
