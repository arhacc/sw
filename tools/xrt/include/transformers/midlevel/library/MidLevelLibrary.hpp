//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <memory>
#include <vector>

#include <dynload.h>

class MidLevelFunction;
class Manager;

class MidLevelLibrary {
    DLLib* lib;
    std::string filename;
    std::vector<std::unique_ptr<MidLevelFunction>> functions;

  public:
    MidLevelLibrary(const char* _filename, Manager* manager);
    ~MidLevelLibrary();

    inline std::vector<std::unique_ptr<MidLevelFunction>>& getFunctions() {
        return functions;
    }
};
