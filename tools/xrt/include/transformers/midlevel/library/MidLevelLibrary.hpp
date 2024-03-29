//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#include <vector>
#include <memory>

#include <dynload.h>

class MidLevelFunction;

class MidLevelLibrary {
    DLLib *lib;
    std::string filename;
    std::vector<std::unique_ptr<MidLevelFunction>> functions;

public:
    MidLevelLibrary(const char *_filename);
    ~MidLevelLibrary();

    inline
    std::vector<std::unique_ptr<MidLevelFunction>>& getFunctions() {
        return functions;
    }
};
