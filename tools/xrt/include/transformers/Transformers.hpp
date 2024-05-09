//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/Constants.hpp>
#include <common/Utils.hpp>
#include <common/arch/Arch.hpp>
#include <common/resources/ResourceIdentifier.hpp>
#include <transformers/common/resourceloader/ResourceLoader.hpp>
#include <manager/Manager.hpp>
#include <transformers/direct/DirectTransformer.hpp>
#include <transformers/json/JsonTransformer.hpp>
#include <transformers/midlevel/MidLevelTransformer.hpp>
#include <transformers/onnx/OnnxTransformer.hpp>

#include <cstdint>
#include <memory>
#include <span>
#include <vector>
#include "transformers/common/resourcefetcher/ResourceFetcher.hpp"

//-------------------------------------------------------------------------------------
class Transformers {
    std::shared_ptr<Arch> arch;
    std::shared_ptr<ResourceLoader> resourceLoader;

    std::unique_ptr<DirectTransformer> directTransformer;
    std::unique_ptr<JsonTransformer> jsonTransformer;
    std::unique_ptr<MidLevelTransformer> midLevelTransformer;
    std::unique_ptr<OnnxTransformer> onnxTransformer;

  public:
    Transformers(Manager* _manager, std::shared_ptr<Arch> _arch);

    ~Transformers() = default;

    int run(const ResourceIdentifier& _path);

    std::vector<uint32_t>
    debugGetArrayData(uint32_t _firstCell, uint32_t _lastCell, uint32_t _firstRow, uint32_t _lastRow);
    std::vector<uint32_t> debugGetArrayRegs(uint32_t _firstCell, uint32_t _lastCell);

    void debugPutArrayData(
        uint32_t _firstCell,
        uint32_t _lastCell,
        uint32_t _firstRow,
        uint32_t _lastRow,
        std::span<const uint32_t> _data);

    unsigned debugSetBreakpoint(std::string_view _functionName, uint32_t _lineNumber);

    void debugContinue();

    unsigned getActiveBreakpointID();

    void registerFetcher(std::unique_ptr<ResourceFetcher> resourceFetchers);
};
//-------------------------------------------------------------------------------------
