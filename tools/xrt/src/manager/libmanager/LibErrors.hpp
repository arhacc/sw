//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/XrtException.hpp>

class LibNotFoundError : public XrtException {
  public:
    LibNotFoundError(const std::string& _name);
    ~LibNotFoundError() override = default;
};

class FunctionNotFoundError : public XrtException {
  public:
    FunctionNotFoundError(std::string_view _name);
    ~FunctionNotFoundError() override = default;
};
