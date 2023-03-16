//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <transformers/common/Transformer.h>
#include <transformers/json/JsonTransformer.h>

//-------------------------------------------------------------------------------------
JsonTransformer::JsonTransformer(DirectTransformer* _directTransformer) {
  std::string _filename = "";
  directTransformer = _directTransformer;
//  load(_filename);
//  process();
}

//-------------------------------------------------------------------------------------
JsonTransformer::~JsonTransformer() {
}

//-------------------------------------------------------------------------------------
void JsonTransformer::load(std::string _path) {
  std::cout << "Loading " << _path << " ..." << std::endl;
  directTransformer -> load(_path);
}

//-------------------------------------------------------------------------------------
void JsonTransformer::run(std::string _name) {

}

//-------------------------------------------------------------------------------------
