//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <manager/Manager.h>
#include <transformers/Transformers.h>
#include <transformers/common/Transformer.cpp>
#include <transformers/direct/DirectTransformer.cpp>
#include <transformers/onnx/OnnxTransformer.cpp>
//-------------------------------------------------------------------------------------
Transformers::Transformers(Manager* _manager) {
	directTransformer = new DirectTransformer(_manager);
	onnxTransformer = new OnnxTransformer(directTransformer);
//	_targets->writeCode(uint32_t _address, uint32_t* _code, uint32_t _length);

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
void Transformers::dump(std::string _address) {
  directTransformer -> dump(_address);
}

//-------------------------------------------------------------------------------------
