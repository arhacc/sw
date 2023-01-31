//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <targets/Targets.h>
#include <transformers/Transformers.h>
#include <transformers/common/Transformer.cpp>
#include <transformers/direct/DirectTransformer.cpp>
#include <transformers/onnx/OnnxTransformer.cpp>
//-------------------------------------------------------------------------------------
Transformers::Transformers(Targets* _targets) {
	directTransformer = new DirectTransformer(_targets);
	onnxTransformer = new OnnxTransformer(directTransformer);
}

//-------------------------------------------------------------------------------------
Transformers::~Transformers() {
	delete(directTransformer);
	delete(onnxTransformer);
}

//-------------------------------------------------------------------------------------
void Transformers::runFile(std::string _path) {
  std::cout << "Transformers::runFile: " << _path << std::endl;
  onnxTransformer -> load(_path);
  onnxTransformer -> process();
}

//-------------------------------------------------------------------------------------
