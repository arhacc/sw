//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <stdio.h>
#include <targets/common/Target.h>

//-------------------------------------------------------------------------------------
Target::Target() {
}

//-------------------------------------------------------------------------------------
Target::~Target() {
}

//-------------------------------------------------------------------------------------
void Target::reset(){
}

//-------------------------------------------------------------------------------------
void Target::runRuntime(uint32_t _address, uint32_t* _args) {
}

//-------------------------------------------------------------------------------------
void Target::runDebug(uint32_t _address, uint32_t* _args, uint32_t _breakpointAddress){

}

//-------------------------------------------------------------------------------------
void Target::readRegister(uint32_t _address, uint32_t _register){

}

//-------------------------------------------------------------------------------------
void Target::writeRegister(uint32_t _address, uint32_t _register){

}

//-------------------------------------------------------------------------------------
void Target::writeCode(uint32_t _address, uint32_t* _code, uint32_t _length){
	printf("Target.loadCode @%d, length=%d\n", _address, _length);
}

//-------------------------------------------------------------------------------------
void Target::readControllerData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
}

//-------------------------------------------------------------------------------------
void Target::writeControllerData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
}

//-------------------------------------------------------------------------------------
void Target::readArrayData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
}

//-------------------------------------------------------------------------------------
void Target::writeArrayData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
}

//-------------------------------------------------------------------------------------
void Target::dump(std::string _address){

}

//-------------------------------------------------------------------------------------
