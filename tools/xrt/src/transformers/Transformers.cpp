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

//-------------------------------------------------------------------------------------
Transformers::Transformers(Manager *_manager) : directTransformer(new DirectTransformer(_manager)),
        jsonTransformer(new JsonTransformer(directTransformer)),
        onnxTransformer(new OnnxTransformer(directTransformer)) {
    //	_targets->writeCode(uint32_t _address, uint32_t* _code, uint32_t _length);

}

//-------------------------------------------------------------------------------------
Transformers::~Transformers() {
    delete directTransformer;
    delete jsonTransformer;
    delete onnxTransformer;
}

//-------------------------------------------------------------------------------------
void Transformers::load(const std::string &_path) {
    int _fileType = getFileTypeFromGeneralPath(_path);
    switch (_fileType) {
        case XPU_FILE_HEX: {
            directTransformer->load(_path);
            break;
        }

        case XPU_FILE_JSON: {
            jsonTransformer->load(_path);
            //jsonTransformer->run("main");
            break;
        }

        case XPU_FILE_OBJ: {
            break;
        }

        case XPU_FILE_ONNX: {
            onnxTransformer->load(_path);
            //onnxTransformer->run("main");
            break;
        }

        default: {
            throw std::runtime_error("Unknown file type: " + _path);
        }
    }
}

//-------------------------------------------------------------------------------------
void Transformers::uploadFunction(const std::string &_name, uint32_t _address) {
    directTransformer->uploadFunction(_name, _address);
}

//-------------------------------------------------------------------------------------
void Transformers::run(const std::string &_path) {
    std::cout << "Transformers::runFile: " << _path << std::endl;
    
    directTransformer->run(_path);
}

//-------------------------------------------------------------------------------------
void Transformers::dump(const std::string &_address) {
    directTransformer->dump(_address);

    throw std::runtime_error("unimplemented Transformers::dump");
}

//-------------------------------------------------------------------------------------
