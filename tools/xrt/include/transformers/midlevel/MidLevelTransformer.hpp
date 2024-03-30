//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <transformers/common/Transformer.hpp>

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class MidLevelLibrary;
class MidLevelFunction;
class Manager;
class DirectTransformer;

class MidLevelTransformer : public Transformer {
    Manager* manager;
    std::vector<std::unique_ptr<MidLevelLibrary>> libraries;
    std::unordered_map<std::string, MidLevelFunction*> functions;

    void initLoadStandardLibraries();

    void loadLibrary(const std::filesystem::path& _path);

  public:
    MidLevelTransformer(const DirectTransformer* _directTransformer);
};
