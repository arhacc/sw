//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <manager/Manager.h>
#include <transformers/direct/DirectTransformer.h>

//-------------------------------------------------------------------------------------
DirectTransformer::DirectTransformer(Manager *_manager) : manager(_manager) {

}

//-------------------------------------------------------------------------------------
void DirectTransformer::load(const std::string &_path) {
    //  printf("DirectTransformer.loadCode @%d, length=%d\n", _address, _length);
    manager->load(_path);
}

//-------------------------------------------------------------------------------------
void DirectTransformer::run(const std::string &_name) {
    //  printf("DirectTransformer.loadCode @%d, length=%d\n", _address, _length);
    manager->run(_name);
}

void DirectTransformer::uploadFunction(const std::string& _name, int32_t _address) {
    //  printf("DirectTransformer.loadCode @%d, length=%d\n", _address, _length);
    manager->uploadFunction(_name, _address);
}

//-------------------------------------------------------------------------------------
void DirectTransformer::writeCode(uint32_t _address, uint32_t *_code, uint32_t _length) {
    //  printf("DirectTransformer.loadCode @%d, length=%d\n", _address, _length);
    manager->writeCode(_address, _code, _length);
}

//-------------------------------------------------------------------------------------
void DirectTransformer::readControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    manager->readControllerData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void DirectTransformer::writeControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    manager->writeControllerData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void DirectTransformer::readArrayData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    manager->readArrayData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void DirectTransformer::writeArrayData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
    manager->writeArrayData(_address, _data, _lineStart, _lineStop, _columnStart, _columnStop);
}

//-------------------------------------------------------------------------------------
void DirectTransformer::dump(const std::string &_address) {
    manager->dump(_address);
}

//-------------------------------------------------------------------------------------
