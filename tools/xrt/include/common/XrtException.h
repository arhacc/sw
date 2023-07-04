//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <exception>
#include <string>

enum class XrtErrorNumber : int {
    RESERVED = 0,
    GENERIC_ERROR = 1,

    LIBRARY_NOT_FOUND = 2,
};

class XrtException : public std::exception {
    std::string message;
    XrtErrorNumber errNumber;
public:
    XrtException(const std::string &_message, XrtErrorNumber _errNumber);
    XrtException(std::string &&_message, XrtErrorNumber _errNumber);
    
    ~XrtException() override = default;

    const char* what() const noexcept override;
    XrtErrorNumber errorNumber() const noexcept;
    int errorNumberInt() const noexcept;
};
