//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#include <common/Utils.hpp>
#include <transformers/direct/DirectTransformer.hpp>
#include <transformers/midlevel/MidLevelTransformer.hpp>
#include <transformers/midlevel/library/MidLevelFunction.hpp>
#include <transformers/midlevel/library/MidLevelLibrary.hpp>

#include <algorithm>
#include <filesystem>
#include <memory>

//-------------------------------------------------------------------------------------
MidLevelTransformer::MidLevelTransformer(const DirectTransformer* _directTransformer) {
    manager = _directTransformer->getManager();

    initLoadStandardLibraries();
}

//-------------------------------------------------------------------------------------
void MidLevelTransformer::initLoadStandardLibraries() {
    std::filesystem::path _path = getPath(ResourceDirectory::MidLevelLibraries);

    std::ranges::for_each(std::filesystem::directory_iterator{_path}, [this](const auto& _dirEntry) {
        if (!_dirEntry.is_directory() && endsWith(_dirEntry.path().string(), ".so")) {
            loadLibrary(_dirEntry.path());
        }
    });
}

//-------------------------------------------------------------------------------------
void MidLevelTransformer::loadLibrary(const std::filesystem::path& _path) {
    libraries.push_back(std::make_unique<MidLevelLibrary>(_path.c_str(), manager));

    for (const auto& _function : libraries.back()->getFunctions()) {
        functions.insert({std::string(_function->getName()), _function.get()});
    }
}

//-------------------------------------------------------------------------------------
