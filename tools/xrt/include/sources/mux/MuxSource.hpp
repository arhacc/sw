//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <sources/common/Source.hpp>

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <mutex>
#include <span>
#include <vector>

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

    void load(const std::filesystem::path& _path);

    int run(std::string_view _func);

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
};
//-------------------------------------------------------------------------------------
