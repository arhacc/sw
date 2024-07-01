//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <sources/common/Source.hpp>

#include <common/resources/ResourceIdentifier.hpp>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>
#include <span>
#include <vector>

#include "transformers/common/resourcefetcher/ResourceFetcher.hpp"
#include <unistd.h>

// forward declaration
class Source;
class Transformers;

//-------------------------------------------------------------------------------------
class MuxSource : public Source {
    Transformers* transformers;
    std::mutex mux;

  public:
    MuxSource(Transformers* _transformers);
    ~MuxSource() override = default;

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

    unsigned debugContinue();

    unsigned getActiveBreakpointID();

    void registerFetcher(std::unique_ptr<ResourceFetcher> _fetcher);
};
//-------------------------------------------------------------------------------------
