//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <string>
#include <transformers/common/Transformer.h>

//-------------------------------------------------------------------------------------
class DirectTransformer : public Transformer {

public:
  DirectTransformer(Targets* _targets);
  ~DirectTransformer();
  void writeCode(uint32_t _address, uint32_t* _code, uint32_t _length);
  void readData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop);
  void writeData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop);
  void dump(std::string _address);

private:
  Targets* targets;
};
//-------------------------------------------------------------------------------------
