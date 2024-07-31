//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <transformers/Transformers.hpp>

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>
#include "transformers/common/resourcefetcher/ResourceFetcher.hpp"

//-------------------------------------------------------------------------------------
Transformers::Transformers(Manager* _manager, std::shared_ptr<Arch> _arch)
    : arch(_arch),
      resourceLoader(std::make_shared<ResourceLoader>(*arch)),
      directTransformer(std::make_unique<DirectTransformer>(_manager, *arch, resourceLoader)),
      jsonTransformer(std::make_unique<JsonTransformer>(directTransformer.get())),
      midLevelTransformer(std::make_shared<MidLevelTransformer>(directTransformer.get())),
      onnxTransformer(std::make_unique<OnnxTransformer>(resourceLoader, midLevelTransformer)) {
    resourceLoader->setManager(*_manager);
    resourceLoader->setMidlevelTransformer(*midLevelTransformer);
    resourceLoader->setOnnxTransformer(*onnxTransformer);

    resourceLoader->loadStdlib();

    directTransformer->init();
}

//-------------------------------------------------------------------------------------
int Transformers::run(
    const ResourceIdentifier& _resourceIdentifier,
    const std::unordered_map<std::string, ResourceIdentifier>& _inputs,
    std::unordered_map<std::string, ResourceIdentifier>& _outputs
) {
    switch (_resourceIdentifier.fileType) {
        case ResourceIdentifier::FileType::Hex: {
            return directTransformer->runLowLevel(_resourceIdentifier);
        }
        case ResourceIdentifier::FileType::Onnx: {
            onnxTransformer->run(_resourceIdentifier, _inputs, _outputs);
            return 0;
        }
        case ResourceIdentifier::FileType::So: {
            throw std::runtime_error("Not yet implemented run so");
        }
        case ResourceIdentifier::FileType::Tensor: {
            throw std::runtime_error("Can not run a tensor");
        }
        default: {
            throw std::runtime_error("Transformers::run: bad resource type");
        }
    }
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
std::vector<uint32_t>
Transformers::debugGetControllerData(uint32_t _firstRow, uint32_t _lastRow) {
    return directTransformer->debugGetControllerData(_firstRow, _lastRow);
}

//-------------------------------------------------------------------------------------
std::vector<uint32_t> Transformers::debugGetControllerRegs() {
    return directTransformer->debugGetControllerRegs();
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
unsigned Transformers::debugContinue() {
    return directTransformer->debugContinue();
}

//-------------------------------------------------------------------------------------
// unsigned Transformers::getActiveBreakpointID() {
//     return directTransformer->getActiveBreakpointID();
// }

//-------------------------------------------------------------------------------------
void Transformers::registerFetcher(std::unique_ptr<ResourceFetcher> _resourceFetcher) {
    resourceLoader->registerFetcher(std::move(_resourceFetcher));
}

//-------------------------------------------------------------------------------------
