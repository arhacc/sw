//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#pragma once
#include <stdio.h>
#include <targets/goldenmodel/GoldenModelTarget.h>

//-------------------------------------------------------------------------------------
GoldenModelTarget::GoldenModelTarget() {
}

//-------------------------------------------------------------------------------------
GoldenModelTarget::~GoldenModelTarget() {
}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::reset(){
}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::runRuntime(uint32_t _address, uint32_t* _args) {
}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::runDebug(uint32_t _address, uint32_t* _args, uint32_t _breakpointAddress){

}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::readRegister(uint32_t _address, uint32_t _register){

}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::writeRegister(uint32_t _address, uint32_t _register){

}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::writeCode(uint32_t _address, uint32_t* _code, uint32_t _length){
	printf("Target.loadCode @%d, length=%d\n", _address, _length);
}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::readData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::writeData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
}

//-------------------------------------------------------------------------------------
void GoldenModelTarget::dump(std::string _address){

}

//-------------------------------------------------------------------------------------
