//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <string>
#include <vector>

//-------------------------------------------------------------------------------------
class Source {
  public:
    Source() = default;

    virtual ~Source() = default;

    void strTokenizer(std::string _input);

    void run(std::string _name);

  protected:
    std::vector<std::string> argv;
};
//-------------------------------------------------------------------------------------
