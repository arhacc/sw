//-------------------------------------------------------------------------------------
//
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

class Manager;
class Matrix;
class MatrixView;
class MidLevelTransformer;

class OnnxExecutionContext {
  std::shared_ptr<MidLevelTransformer> midLevelTransformer;

  std::unordered_map<std::string, std::shared_ptr<Matrix>> tensorData; // TODO: generalize
  std::unordered_map<std::string, std::shared_ptr<MatrixView>> tensorViewData; // TODO: generalize

public:
  OnnxExecutionContext(std::shared_ptr<MidLevelTransformer> _midLevelTransformer);

  std::shared_ptr<MidLevelTransformer> getMidLevelTransformer();

  void allocateTensorData(std::string_view _tensorName, std::vector<uint64_t> _dimensions);
  std::shared_ptr<MatrixView>* getTensorData(std::string_view _tensorName);
};

