//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <transformers/common/Transformer.hpp>

#include <unordered_map>
#include <vector>
#include <memory>

class MidLevelLibrary;
class MidLevelFunction;

class MidLevelTransformer : public Transformer {
  std::vector<std::unique_ptr<MidLevelLibrary>> libraries;
  std::unordered_map<std::string, MidLevelFunction*> functions;

  void initLoadStandardLibraries();

  public:
    MidLevelTransformer();
};
