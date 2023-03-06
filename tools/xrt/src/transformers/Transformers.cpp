//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Globals.h>
#include <common/Utils.h>
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
	int _fileType = getFileTypeFromGeneralPath(_path);
	switch(_fileType){
		case XPU_FILE_HEX: {

			break;
		}

		case XPU_FILE_JSON: {

			break;
		}

		case XPU_FILE_OBJ: {

			break;
		}

		case XPU_FILE_ONNX: {
		  onnxTransformer -> load(_path);
		  onnxTransformer -> process();
			break;
		}

		default: {
  		std::cout << "Unknown file type: " << _path << std::endl;
  		exit(1);
			break;
		}
	}
}

//-------------------------------------------------------------------------------------
void Transformers::dump(std::string _address) {
  directTransformer -> dump(_address);
}

//-------------------------------------------------------------------------------------
