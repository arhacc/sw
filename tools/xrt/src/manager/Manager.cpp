//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <targets/Targets.h>
#include <manager/Manager.h>
//#include <manager/common/Transformer.cpp>
#include <manager/libmanager/LibManager.cpp>
#include <manager/memmanager/MemManager.cpp>
#include <manager/driver/Driver.cpp>

//-------------------------------------------------------------------------------------
Manager::Manager(Targets* _targets) {
	driver = new Driver(_targets);
	memManager = new MemManager(driver);
	libManager = new LibManager(memManager);
//	_Manager->writeCode(uint32_t _address, uint32_t* _code, uint32_t _length);

}

//-------------------------------------------------------------------------------------
Manager::~Manager() {
	delete(libManager);
	delete(memManager);
	delete(driver);
}


//-------------------------------------------------------------------------------------
void Manager::reset(){
    driver -> reset();
}

//-------------------------------------------------------------------------------------
void Manager::run(std::string _name){
	libManager -> resolve(_name);
}

//-------------------------------------------------------------------------------------
void Manager::runRuntime(uint32_t _address, uint32_t* _args){
    driver -> runRuntime(_address, _args);
}

//-------------------------------------------------------------------------------------
void Manager::runDebug(uint32_t _address, uint32_t* _args, uint32_t _breakpointAddress){
    driver -> runDebug(_address, _args, _breakpointAddress);
}

//-------------------------------------------------------------------------------------
void Manager::readRegister(uint32_t _address, uint32_t _register){
    driver -> readRegister(_address, _register);
}

//-------------------------------------------------------------------------------------
void Manager::writeRegister(uint32_t _address, uint32_t _register){
    driver -> writeRegister(_address, _register);
}

//-------------------------------------------------------------------------------------
void Manager::writeCode(uint32_t _address, uint32_t* _code, uint32_t _length){
    driver -> writeCode(_address, _code, _length);
}

//-------------------------------------------------------------------------------------
void Manager::readControllerData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
    driver -> readControllerData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop); 
}

//-------------------------------------------------------------------------------------
void Manager::writeControllerData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
//  printf("Manager.loadCode @%d, length=%d\n", _address, _length);
    driver -> writeControllerData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void Manager::readArrayData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
    driver -> readArrayData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void Manager::writeArrayData(uint32_t _address, uint32_t* _data, uint32_t _lineStart , uint32_t _lineStop, uint32_t _columnStart, uint32_t _columnStop){
//  printf("Manager.loadCode @%d, length=%d\n", _address, _length);
    driver -> writeArrayData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void Manager::dump(std::string _address){
    driver -> dump(_address);
}


/*
//-------------------------------------------------------------------------------------
void Transformers::runFile(std::string _path) {
  std::cout << "Transformers::runFile: " << _path << std::endl;
  onnxTransformer -> load(_path);
  onnxTransformer -> process();
}

//-------------------------------------------------------------------------------------
void Transformers::dump(std::string _address) {
  directTransformer -> dump(_address);
}
*/
//-------------------------------------------------------------------------------------
