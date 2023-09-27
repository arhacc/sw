//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/XrtException.h>

//-------------------------------------------------------------------------------------
XrtException::XrtException(const std::string& _message, XrtErrorNumber _errNumber)
    : message(_message), errNumber(_errNumber) {}

//-------------------------------------------------------------------------------------
XrtException::XrtException(std::string&& _message, XrtErrorNumber _errNumber)
    : message(std::move(_message)), errNumber(_errNumber) {}

//-------------------------------------------------------------------------------------
const char* XrtException::what() const noexcept {
    return message.c_str();
}

//-------------------------------------------------------------------------------------
XrtErrorNumber XrtException::errorNumber() const noexcept {
    return errNumber;
}

//-------------------------------------------------------------------------------------
int XrtException::errorNumberInt() const noexcept {
    return static_cast<int>(errNumber);
}

//-------------------------------------------------------------------------------------
