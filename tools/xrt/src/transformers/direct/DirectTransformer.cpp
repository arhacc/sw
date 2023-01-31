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
void DirectTransformer::readData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
  targets->readData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void DirectTransformer::writeData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
  targets->writeData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
