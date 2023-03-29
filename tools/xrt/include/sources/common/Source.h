//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <string>

//-------------------------------------------------------------------------------------
class Source {
    static constexpr int TOKENCOUNT = 10;
    static constexpr int TOKENSIZE = 100;
public:
    Source();

    virtual ~Source();

    void strTokenizer(std::string _input);

    void run(std::string _name);

protected:
    char **argv;
};
//-------------------------------------------------------------------------------------


