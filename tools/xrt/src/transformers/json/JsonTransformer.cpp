//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <transformers/common/Transformer.hpp>
#include <transformers/json/JsonTransformer.hpp>

//-------------------------------------------------------------------------------------
JsonTransformer::JsonTransformer(DirectTransformer* _directTransformer)
    : directTransformer(_directTransformer) {
    std::string _filename;
    //  load(_filename);
    //  process();
}

//-------------------------------------------------------------------------------------
void JsonTransformer::load(const std::string& _path) {
    std::cout << "Loading " << _path << " ..." << std::endl;
    directTransformer->load(_path);
}

//-------------------------------------------------------------------------------------
void JsonTransformer::run(const std::string& _name) {
    directTransformer->run(_name);
}

//-------------------------------------------------------------------------------------
