//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <cstdint>
#include <memory>
#include <span>
#include <string>
#include <vector>

// forward declaration
class Manager;
struct Arch;
class JsonTransformer;
class DirectTransformer;
class OnnxTransformer;
class MidLevelTransformer;

//-------------------------------------------------------------------------------------
class Transformers {
    std::shared_ptr<Arch> arch;

    DirectTransformer* directTransformer;
    JsonTransformer* jsonTransformer;
    OnnxTransformer* onnxTransformer;
    MidLevelTransformer* midLevelTransformer;

  public:
    Transformers(Manager* _manager, std::shared_ptr<Arch> _arch);

    ~Transformers();

    void load(const std::string& _path);

    int run(const std::string& _path);

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
