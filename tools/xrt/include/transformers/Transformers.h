//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <manager/Manager.h>
#include <transformers/common/Transformer.h>
#include <transformers/direct/DirectTransformer.h>
#include <transformers/json/JsonTransformer.h>
#include <transformers/onnx/OnnxTransformer.h>

#include <cstdint>
#include <span>
#include <vector>

//-------------------------------------------------------------------------------------
class Transformers {
    DirectTransformer* directTransformer;
    JsonTransformer* jsonTransformer;
    OnnxTransformer* onnxTransformer;

  public:
    Transformers(Manager* _manager);

    ~Transformers();

    void load(const std::string& _path);

    void run(const std::string& _path);

    std::vector<uint32_t> debugGetArrayData(
        uint32_t _firstCell, uint32_t _lastCell, uint32_t _firstRow, uint32_t _lastRow);

    void debugPutArrayData(
        uint32_t _firstCell,
        uint32_t _lastCell,
        uint32_t _firstRow,
        uint32_t _lastRow,
        std::span<const uint32_t> _data);
};
//-------------------------------------------------------------------------------------
