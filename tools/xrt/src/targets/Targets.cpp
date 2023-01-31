//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <targets/Targets.h>
#include <targets/common/Target.cpp>
#include <targets/fpga/FpgaTarget.cpp>
#include <targets/sim/SimTarget.cpp>
#include <targets/goldenmodel/GoldenModelTarget.cpp>
//-------------------------------------------------------------------------------------
Targets::Targets(bool _enableFpgaTarget, bool _enableSimTarget, bool _enableGoldenModelTarget) {
  enableFpgaTarget = _enableFpgaTarget;
  enableSimTarget = _enableSimTarget;
  enableGoldenModelTarget = _enableGoldenModelTarget;
  
  if(_enableFpgaTarget){
    fpgaTarget = new FpgaTarget();
  }
  if(_enableSimTarget){
    simTarget = new SimTarget();
  }
  if(_enableGoldenModelTarget){
    goldenModelTarget = new GoldenModelTarget();
  }
}

//-------------------------------------------------------------------------------------
Targets::~Targets() {
    delete(fpgaTarget);
    delete(simTarget); 
    delete(goldenModelTarget);   
}

//-------------------------------------------------------------------------------------
void Targets::reset(){
  if(enableFpgaTarget){
    fpgaTarget -> reset();
  }
  if(enableSimTarget){
    simTarget -> reset();
  }
  if(enableGoldenModelTarget){
    goldenModelTarget -> reset();
  }  
}

//-------------------------------------------------------------------------------------
void Targets::runRuntime(uint32_t _address, uint32_t* _args){
  if(enableFpgaTarget){
    fpgaTarget -> runRuntime(_address, _args);
  }
  if(enableSimTarget){
    simTarget -> runRuntime(_address, _args);
  }
  if(enableGoldenModelTarget){
    goldenModelTarget -> runRuntime(_address, _args);
  }  
}

//-------------------------------------------------------------------------------------
void Targets::runDebug(uint32_t _address, uint32_t* _args, uint32_t _breakpointAddress){
  if(enableFpgaTarget){
    fpgaTarget -> runDebug(_address, _args, _breakpointAddress);
  }
  if(enableSimTarget){
    simTarget -> runDebug(_address, _args, _breakpointAddress);
  }
  if(enableGoldenModelTarget){
    goldenModelTarget -> runDebug(_address, _args, _breakpointAddress);
  }  
}

//-------------------------------------------------------------------------------------
void Targets::readRegister(uint32_t _address, uint32_t _register){
  if(enableFpgaTarget){
    fpgaTarget -> readRegister(_address, _register);
  }
  if(enableSimTarget){
    simTarget -> readRegister(_address, _register);
  }
  if(enableGoldenModelTarget){
    goldenModelTarget -> readRegister(_address, _register);
  }  
}

//-------------------------------------------------------------------------------------
void Targets::writeRegister(uint32_t _address, uint32_t _register){
  if(enableFpgaTarget){
    fpgaTarget -> writeRegister(_address, _register);
  }
  if(enableSimTarget){
    simTarget -> writeRegister(_address, _register);
  }
  if(enableGoldenModelTarget){
    goldenModelTarget -> writeRegister(_address, _register);
  }  
}

//-------------------------------------------------------------------------------------
void Targets::writeCode(uint32_t _address, uint32_t* _code, uint32_t _length){
  if(enableFpgaTarget){
    fpgaTarget -> writeCode(_address, _code, _length);
  }
  if(enableSimTarget){
    simTarget -> writeCode(_address, _code, _length);
  }
  if(enableGoldenModelTarget){
    goldenModelTarget -> writeCode(_address, _code, _length);
  }  
}

//-------------------------------------------------------------------------------------
void Targets::readData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
  if(enableFpgaTarget){
    fpgaTarget -> readData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
  }
  if(enableSimTarget){
    simTarget -> readData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
  }
  if(enableGoldenModelTarget){
    goldenModelTarget -> readData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
  }  
}

//-------------------------------------------------------------------------------------
void Targets::writeData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
//  printf("Targets.loadCode @%d, length=%d\n", _address, _length);
  if(enableFpgaTarget){
    fpgaTarget -> writeData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
  }
  if(enableSimTarget){
    simTarget -> writeData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
  }
  if(enableGoldenModelTarget){
    goldenModelTarget -> writeData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
  }
}

//-------------------------------------------------------------------------------------
