//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <transformers/common/Transformer.hpp>

#include <atomic>
#include <span>
#include <string>
#include <vector>

// forward declaration
class Manager;
class Matrix;
struct AcceleratorImage;

//-------------------------------------------------------------------------------------
class DirectTransformer : public Transformer {
    Manager* manager;

    Matrix* debugMemoryImage;
    std::atomic_bool hitBreakpoint;
    std::atomic_uint hitBreakpointID;

    void updateDebugArrayDataMemoryImage();
    void pushDebugArrayDataMeoryImage();

  public:
    explicit DirectTransformer(Manager* _manager);

    ~DirectTransformer() override;

    void load(const std::string& _path);

    int run(const std::string& _name);

    void uploadFunction(const std::string& _name, int32_t _address);

    void writeCode(uint32_t _address, uint32_t* _code, uint32_t _length);

    void readControllerData(
        uint32_t _address,
        uint32_t* _data,
        uint32_t _lineStart,
        uint32_t _lineStop,
        uint32_t _columnStart,
        uint32_t _columnStop);

    void writeControllerData(
        uint32_t _address,
        uint32_t* _data,
        uint32_t _lineStart,
        uint32_t _lineStop,
        uint32_t _columnStart,
        uint32_t _columnStop);

    void readArrayData(
        uint32_t _address,
        uint32_t* _data,
        uint32_t _lineStart,
        uint32_t _lineStop,
        uint32_t _columnStart,
        uint32_t _columnStop);

    void writeArrayData(
        uint32_t _address,
        uint32_t* _data,
        uint32_t _lineStart,
        uint32_t _lineStop,
        uint32_t _columnStart,
        uint32_t _columnStop);

    void dump(const std::string& _address);

    std::vector<uint32_t>
    debugGetArrayData(uint32_t _firstCell, uint32_t _lastCell, uint32_t _firstRow, uint32_t _lastRow);

    void debugPutArrayData(
        uint32_t _firstCell,
        uint32_t _lastCell,
        uint32_t _firstRow,
        uint32_t _lastRow,
        std::span<const uint32_t> _data);

    unsigned debugSetBreakpoint(std::string_view _functionName, uint32_t _lineNumber);

    bool handleDebugHitCallback(AcceleratorImage&, unsigned);

    void debugContinue();

    int waitForFunctionEnd();

    unsigned getActiveBreakpointID();
};
//-------------------------------------------------------------------------------------
