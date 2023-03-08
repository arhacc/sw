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
//  load(_filename);
//  process();
}

//-------------------------------------------------------------------------------------
JsonTransformer::~JsonTransformer() {
}

//-------------------------------------------------------------------------------------
void JsonTransformer::load(std::string _filename) {
  std::cout << "Loading " << _filename << " ..." << std::endl;

  std::ifstream input(_filename, std::ios::ate | std::ios::binary); // open file and move current
                                           // position in file to the end

  std::streamsize _size = input.tellg(); // get current position in file
  input.seekg(0, std::ios::beg);        // move to start of file
//  std::cout << "size===" << size << std::endl;
  if(_size <= 0){
    std::cout << "Cannot read: " << _filename << std::endl;
//    exit(1);
    return;
  }
  std::vector<char> buffer(_size);
  input.read(buffer.data(), _size); // read raw data
}

//-------------------------------------------------------------------------------------
void JsonTransformer::process() {
  std::cout << "Processing JSON file... " << std::endl;

}
//-------------------------------------------------------------------------------------
