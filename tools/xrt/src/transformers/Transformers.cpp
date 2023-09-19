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

#include <cstdint>
#include <filesystem>

//-------------------------------------------------------------------------------------
Transformers::Transformers(Manager* _manager)
    : directTransformer(new DirectTransformer(_manager)),
      jsonTransformer(new JsonTransformer(directTransformer)),
      onnxTransformer(new OnnxTransformer(directTransformer)) {}

//-------------------------------------------------------------------------------------
Transformers::~Transformers() {
    delete directTransformer;
    delete jsonTransformer;
    delete onnxTransformer;
}

//-------------------------------------------------------------------------------------
void Transformers::load(const std::string& _path) {
    std::cout << "Transformers::loadFile: " << _path << std::endl;

    int _fileType = getFileTypeFromGeneralPath(_path);
    switch (_fileType) {
        case XPU_FILE_HEX: {
            directTransformer->load(_path);
            break;
        }

        case XPU_FILE_JSON: {
            jsonTransformer->load(_path);
            break;
        }

        case XPU_FILE_OBJ: {
            break;
        }

        case XPU_FILE_ONNX: {
            onnxTransformer->load(_path);
            break;
        }

        case XPU_FILE_C:
        case XPU_FILE_CPP: {
            directTransformer->load(_path);

            break;
        }

        default: {
            throw std::runtime_error("Unknown file type: " + _path);
        }
    }
}

//-------------------------------------------------------------------------------------
void Transformers::run(const std::string& _path) {
    std::cout << "Transformers::runFunction: " << _path << std::endl;

    directTransformer->run(_path);
}

//-------------------------------------------------------------------------------------
std::vector<uint32_t> Transformers::debugGetArrayData(
    uint32_t _firstCell, uint32_t _lastCell, uint32_t _firstRow, uint32_t _lastRow) {
    return directTransformer->debugGetArrayData(
        _firstCell, _lastCell, _firstRow, _lastRow);
}

//-------------------------------------------------------------------------------------
void Transformers::debugPutArrayData(
    uint32_t _firstCell,
    uint32_t _lastCell,
    uint32_t _firstRow,
    uint32_t _lastRow,
    std::span<const uint32_t> _data) {
    directTransformer->debugPutArrayData(
        _firstCell, _lastCell, _firstRow, _lastRow, _data);
}

//-------------------------------------------------------------------------------------
