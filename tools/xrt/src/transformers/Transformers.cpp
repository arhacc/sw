//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/Constants.hpp>
#include <common/Utils.hpp>
#include <common/arch/Arch.hpp>
#include <manager/Manager.hpp>
#include <transformers/Transformers.hpp>
#include <transformers/direct/DirectTransformer.hpp>
#include <transformers/json/JsonTransformer.hpp>
#include <transformers/midlevel/MidLevelTransformer.hpp>
#include <transformers/onnx/OnnxTransformer.hpp>

#include <cstdint>
#include <filesystem>
#include <memory>
#include <vector>

//-------------------------------------------------------------------------------------
Transformers::Transformers(Manager* _manager, std::shared_ptr<Arch> _arch)
    : arch(_arch),
      directTransformer(new DirectTransformer(_manager, *arch)),
      jsonTransformer(new JsonTransformer(directTransformer)),
      onnxTransformer(new OnnxTransformer(directTransformer)),
      midLevelTransformer(new MidLevelTransformer(directTransformer)) {}

//-------------------------------------------------------------------------------------
Transformers::~Transformers() {
    delete directTransformer;
    delete jsonTransformer;
    delete onnxTransformer;
}

//-------------------------------------------------------------------------------------
void Transformers::load(const std::string& _path) {
    std::cout << "Transformers::loadFile: " << _path << std::endl;

    FileType _fileType = getFileTypeFromPath(_path);
    switch (_fileType) {
        case FileType::Hex: {
            directTransformer->load(_path);
            break;
        }

        case FileType::Json: {
            jsonTransformer->load(_path);
            break;
        }

        case FileType::Obj: {
            break;
        }

        case FileType::Onnx: {
            onnxTransformer->load(_path);
            break;
        }

        case FileType::C:
        case FileType::Cpp: {
            directTransformer->load(_path);

            break;
        }

        default: {
            throw std::runtime_error("Unknown file type: " + _path);
        }
    }
}

//-------------------------------------------------------------------------------------
int Transformers::run(const std::string& _path) {
    std::cout << "Transformers::runFunction: " << _path << std::endl;

    return directTransformer->run(_path);
}

//-------------------------------------------------------------------------------------
std::vector<uint32_t>
Transformers::debugGetArrayData(uint32_t _firstCell, uint32_t _lastCell, uint32_t _firstRow, uint32_t _lastRow) {
    return directTransformer->debugGetArrayData(_firstCell, _lastCell, _firstRow, _lastRow);
}

//-------------------------------------------------------------------------------------
std::vector<uint32_t> Transformers::debugGetArrayRegs(uint32_t _firstCell, uint32_t _lastCell) {
    return directTransformer->debugGetArrayRegs(_firstCell, _lastCell);
}

//-------------------------------------------------------------------------------------
void Transformers::debugPutArrayData(
    uint32_t _firstCell, uint32_t _lastCell, uint32_t _firstRow, uint32_t _lastRow, std::span<const uint32_t> _data) {
    directTransformer->debugPutArrayData(_firstCell, _lastCell, _firstRow, _lastRow, _data);
}

//-------------------------------------------------------------------------------------
unsigned Transformers::debugSetBreakpoint(std::string_view _functionName, uint32_t _lineNumber) {
    return directTransformer->debugSetBreakpoint(_functionName, _lineNumber);
}

//-------------------------------------------------------------------------------------
void Transformers::debugContinue() {
    directTransformer->debugContinue();
}

//-------------------------------------------------------------------------------------
unsigned Transformers::getActiveBreakpointID() {
    return directTransformer->getActiveBreakpointID();
}

//-------------------------------------------------------------------------------------
