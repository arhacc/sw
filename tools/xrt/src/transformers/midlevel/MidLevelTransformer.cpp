//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#include <common/Utils.hpp>
#include <transformers/midlevel/MidLevelTransformer.hpp>
#include <transformers/midlevel/library/MidLevelLibrary.hpp>
#include <transformers/midlevel/library/MidLevelFunction.hpp>

#include <algorithm>
#include <filesystem>

//-------------------------------------------------------------------------------------
MidLevelTransformer::MidLevelTransformer() {
    initLoadStandardLibraries();
}

//-------------------------------------------------------------------------------------
void MidLevelTransformer::initLoadStandardLibraries() {
    std::filesystem::path _path = getPath(ResourceDirectory::MidLevelLibraries);

    std::ranges::for_each(std::filesystem::directory_iterator{_path}, 
        [this](const auto& _dirEntry) {
            if (!_dirEntry.is_directory() && endsWith(_dirEntry.path().string(), ".so")) {
                libraries.emplace_back(_dirEntry.path().string());

                for(const auto& _function : libraries.back()->getFunctions()) {

                }
            }
        });
}

//-------------------------------------------------------------------------------------
