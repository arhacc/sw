//-------------------------------------------------------------------------------------
//
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <transformers/onnx/OnnxExecutionContext.hpp>
#include <transformers/midlevel/MidLevelTransformer.hpp>

#include <common/types/Matrix.hpp>

#include <cassert>


OnnxExecutionContext::OnnxExecutionContext(std::shared_ptr<MidLevelTransformer> _midLevelTransformer)
    : midLevelTransformer(std::move(_midLevelTransformer))
{}

std::shared_ptr<MidLevelTransformer> OnnxExecutionContext::getMidLevelTransformer() {
  return midLevelTransformer;
}

void OnnxExecutionContext::allocateTensorData(std::string_view _tensorName, std::vector<uint64_t> _dimensions) {
  // TODO: generalize
  assert(_dimensions.size() == 2);
  
  tensorData[std::string(_tensorName)] = std::make_shared<Matrix>(_dimensions[0], _dimensions[1]);
  tensorViewData[std::string(_tensorName)] = std::make_shared<MatrixView>(tensorData[std::string(_tensorName)]);
}

std::shared_ptr<MatrixView>* OnnxExecutionContext::getTensorData(std::string_view _tensorName) {
  return &tensorViewData[std::string(_tensorName)];
}

