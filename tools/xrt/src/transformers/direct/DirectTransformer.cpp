//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <targets/Targets.h>
#include <transformers/common/Transformer.h>
#include <transformers/direct/DirectTransformer.h>

//-------------------------------------------------------------------------------------
DirectTransformer::DirectTransformer(Targets* _targets) {
  targets = _targets;
}

//-------------------------------------------------------------------------------------
DirectTransformer::~DirectTransformer() {
}

//-------------------------------------------------------------------------------------
void DirectTransformer::writeCode(uint32_t _address, uint32_t* _code, uint32_t _length){
//  printf("DirectTransformer.loadCode @%d, length=%d\n", _address, _length);
  targets->writeCode(_address, _code, _length);
}  

//-------------------------------------------------------------------------------------
void DirectTransformer::readControllerData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
  targets->readControllerData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void DirectTransformer::writeControllerData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
  targets->writeControllerData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void DirectTransformer::readArrayData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
  targets->readArrayData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void DirectTransformer::writeArrayData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
  targets->writeArrayData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void DirectTransformer::dump(std::string _address){
  targets->dump(_address);
}

//-------------------------------------------------------------------------------------
