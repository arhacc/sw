//-------------------------------------------------------------------------------------
//
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once

#include <cstdint>
#include <string>
#include <filesystem>
#include <vector>

#include <transformers/onnx/OnnxExecutionContext.hpp>

class OnnxTensor {
public:

  enum class Type : uint16_t {
    Undefined = 0,
    Float = 1,
    Uint8 = 2,
    Int8 = 3,
    Uint16 = 4,
    Int16 = 5,
    Int32 = 6,
    Int64 = 7,
    String = 8,
    Bool = 9,
    Float16 = 10,
    Double = 11,
    Uint32 = 12,
    Uint64 = 13,
    Complex64 = 14,
    Complex128 = 15,
    Bfloat16 = 16,
    Float8e4m3fn = 17,
    Float8e4m3fnuz = 18,
    Float8e5m2 = 19,
    Float8e5m2fnuz = 20,
    Uint4 = 21,
    Int4 = 22,
  };

  typedef std::vector<uint64_t> Dimensions;

private:
  std::string name;
  Dimensions dim;
  Type type;

public:
  OnnxTensor(std::string_view _name);

  const std::string& getName() const;
  void setTypeDim(Type _type, Dimensions _dim);

  Type getType() const;
  Dimensions getDimensions() const;

  void* getDataForMidLevel(OnnxExecutionContext& _executionContext);
  void parseData(OnnxExecutionContext& _executionContext, const std::filesystem::path& _path);
  void writeData(OnnxExecutionContext& _executionContext, const std::filesystem::path& _path);
};
