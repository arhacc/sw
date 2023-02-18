//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
/*
https://en.wikipedia.org/wiki/Intel_HEX
*/
//-------------------------------------------------------------------------------------
#pragma once
#include <map>
#include <unordered_map>
#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdint>
#include <algorithm>
#include <string>
#include <ostream>
#include <sstream>
#include <iomanip>
#include <fstream>

//-------------------------------------------------------------------------------------
class Driver {

public:
  Driver(Targets* _targets);
  ~Driver();

  void reset();
  void runRuntime(uint32_t _address, uint32_t* _args);
  void runDebug(uint32_t _address, uint32_t* _args, uint32_t _breakpointAddress);

  void readRegister(uint32_t _address, uint32_t _register);
  void writeRegister(uint32_t _address, uint32_t _register);

  void writeCode(uint32_t _address, uint32_t* _code, uint32_t _length);

  void readControllerData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop);
  void writeControllerData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop);

  void readArrayData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop);
  void writeArrayData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop);

  void dump(std::string _address);

private:
  Targets* targets;
  
};
//-------------------------------------------------------------------------------------
