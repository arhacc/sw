//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <transformers/common/Transformer.h>
#include <transformers/direct/DirectTransformer.h>
#include <transformers/onnx/OnnxTransformer.h>
#include <transformers/onnx/XpuL1OnnxRuntime.h>
#include <transformers/onnx/XpuL1OnnxRuntime.cpp>
#include <transformers/onnx/XpuL2Resolver.cpp>
#include <transformers/onnx/XpuL3Library.cpp>

//-------------------------------------------------------------------------------------
OnnxTransformer::OnnxTransformer(DirectTransformer* _directTransformer) {
  std::string _filename = "";
  xpuL1OnnxRuntime = new XpuL1OnnxRuntime(_directTransformer);
//  load(_filename);
//  process();
}

//-------------------------------------------------------------------------------------
OnnxTransformer::~OnnxTransformer() {
}

//-------------------------------------------------------------------------------------
void OnnxTransformer::load(std::string _filename) {
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

  onnx::ModelProto model;
  model.ParseFromArray(buffer.data(), _size); // parse protobuf

  ONNX_NAMESPACE::shape_inference::InferShapes(model);
  graph = model.graph();
}

//-------------------------------------------------------------------------------------
void OnnxTransformer::process() {
  std::cout << "Processing ONNX graph... " << std::endl;

  for (size_t i = 0; i < graph.value_info_size(); i++) {
    const onnx::ValueInfoProto info = graph.value_info(i);
    std::string name = info.name();
    auto shape = info.type().tensor_type().shape();
    if (shape.dim_size() > 0) {
      int size = shape.dim_size();
      std::cout << name << " : " << shape.dim(0).dim_param();
      for (int i = 1; i < size; i++) {
        std::cout << ", " << shape.dim(i).dim_value();
//        xpuDriver -> writeData()
      }
      std::cout << std::endl;
      xpuL1OnnxRuntime -> run(name);
    }
  }

}
//-------------------------------------------------------------------------------------
