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
static void runHex(DirectTransformer *_directTransformer, const std::string &_path) {
    // this is a temporary function; this should proabably be in DirectTransformer (?)
    // or some other class

    std::ifstream _file(_path);

    std::vector<uint32_t> _code;
    uint32_t _instruction;

    while (_file >> std::hex >> _instruction) {
        _code.push_back(_instruction);
    }

    _directTransformer->writeCode(0, _code.data(), _code.size());
}

//-------------------------------------------------------------------------------------
void Transformers::load(const std::string &_path) {
    directTransformer->load(_path);
}

//-------------------------------------------------------------------------------------
void Transformers::run(const std::string &_path) {
    std::cout << "Transformers::runFile: " << _path << std::endl;
    int _fileType = getFileTypeFromGeneralPath(_path);
    switch (_fileType) {
        case XPU_FILE_HEX: {
            runHex(directTransformer, _path);
            break;
        }

        case XPU_FILE_JSON: {
            jsonTransformer->load(_path);
            jsonTransformer->run("main");
            break;
        }

        case XPU_FILE_OBJ: {
            break;
        }

        case XPU_FILE_ONNX: {
            onnxTransformer->load(_path);
            onnxTransformer->run("main");
            break;
        }

        default: {
            throw std::runtime_error("Unknown file type: " + _path);
        }
    }
}

//-------------------------------------------------------------------------------------
void Transformers::dump(const std::string &_address) {
    directTransformer->dump(_address);
}

//-------------------------------------------------------------------------------------
