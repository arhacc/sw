//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <common/resources/ResourceIdentifier.hpp>
#include <transformers/common/Transformer.hpp>

#include <atomic>
#include <cstdint>
#include <memory>
#include <span>
#include <string>
#include <vector>

// forward declaration
class Manager;
class Matrix;
class ResourceLoader;
struct Arch;
struct AcceleratorImage;

//-------------------------------------------------------------------------------------
class DirectTransformer : public Transformer {
    Manager* manager;

    std::shared_ptr<ResourceLoader> resourceLoader;

    std::shared_ptr<AcceleratorImage> debugAccImage;
    std::atomic_bool hitBreakpoint;
    std::atomic_uint hitBreakpointID;

  public:
    DirectTransformer(Manager* _manager, const Arch& _arch, std::shared_ptr<ResourceLoader> resourceLoader);

    ~DirectTransformer() override;

    // must be called after the standard library is loaded
    void init();

    int runLowLevel(const ResourceIdentifier& resourceIdentifier);

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

    std::vector<uint32_t> debugGetArrayRegs(uint32_t _firstCell, uint32_t _lastCell);

    unsigned debugSetBreakpoint(std::string_view _functionName, uint32_t _lineNumber);

    bool handleDebugHitCallback(std::shared_ptr<AcceleratorImage>, unsigned);

    void debugContinue();

    int waitForFunctionEnd();

    unsigned getActiveBreakpointID();

    inline Manager* getManager() const {
        return manager;
    }
};
//-------------------------------------------------------------------------------------
