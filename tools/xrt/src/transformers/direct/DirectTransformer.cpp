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

//-------------------------------------------------------------------------------------
