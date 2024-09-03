//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <transformers/direct/DirectTransformer.hpp>
#include <transformers/onnx/OnnxTransformer.hpp>
#include <transformers/onnx/OnnxGraph.hpp>
#include <transformers/onnx/OnnxNode.hpp>
#include <transformers/onnx/OnnxExecutionContext.hpp>
#include <transformers/onnx/OnnxOutputCache.hpp>
#include <transformers/common/resourceloader/ResourceLoader.hpp>
#include <common/log/Logger.hpp>


#include <transformers/onnx/OnnxTensor.hpp>
#include <common/types/Matrix.hpp>

#include <iostream>

//-------------------------------------------------------------------------------------
OnnxTransformer::OnnxTransformer(
    std::shared_ptr<ResourceLoader> _resourceLoader,
    std::shared_ptr<MidLevelTransformer> _midLevelTransformer
)
  : resourceLoader(std::move(_resourceLoader)),
    midLevelTransformer(std::move(_midLevelTransformer)),
    outputCache(std::make_unique<OnnxOutputCache>())
{

  //OnnxExecutionContext _context(midLevelTransformer);
  //OnnxTensor _t1("t1");
  //_t1.setTypeDim(OnnxTensor::Type::Uint32, {16, 16});
  //_t1.parseData(_context, "result.xpu_tensor");

  //auto _m = *reinterpret_cast<std::shared_ptr<Matrix>*>(_t1.getDataForMidLevel(_context));

  //fmt::println("result is");
  //for (size_t i = 0; i < _m->numRows(); i++) {
  //  for (size_t j = 0; j < _m->numColumns(); j++) {
  //    fmt::print("{} ", _m->at(i, j));
  //  }
  //  fmt::print("\n");
  //}
}

//-------------------------------------------------------------------------------------
OnnxTransformer::~OnnxTransformer() {
}

void OnnxTransformer::test() {
	logWork.print("Started ONNX test\n");
	OnnxExecutionContext _context(midLevelTransformer);

	OnnxTensor _input0("Mat0");
	_input0.setTypeDim(OnnxTensor::Type::Uint32, {32, 32});

	_context.allocateTensorData("Mat0", {32, 32});
	auto _matrixView0 = *_context.getTensorData("Mat0");

	size_t k = 0;
	for (size_t i = 0; i < 32; i++) {
		for (size_t j = 0; j < 32; j++) {
			_matrixView0->at(i, j) = k++;
		}
	}

	OnnxTensor _input1("Mat1");
	_input1.setTypeDim(OnnxTensor::Type::Uint32, {32, 32});

	_context.allocateTensorData("Mat1", {32, 32});
	auto _matrixView1 = *_context.getTensorData("Mat1");

	k = 0;
	for (size_t i = 0; i < 32; i++) {
		for (size_t j = 0; j < 32; j++) {
			_matrixView1->at(i, j) = 1000 + k++;
		}
	}

	_input0.writeData(_context, "Mat0.xpu_tensor");
	_input1.writeData(_context, "Mat1.xpu_tensor");

	logWork.print("Wrote input files\n");

	auto _graph = OnnxGraph::parse("/home/vserbu/work/xpu/sw/tools/xrt/models/MatMatAddTest.onnx");
	
	std::unordered_map<std::string, std::filesystem::path> _inputs;
	std::unordered_map<std::string, std::filesystem::path> _outputs;

	_inputs["Mat0"] = "Mat0.xpu_tensor";
	_inputs["Mat1"] = "Mat1.xpu_tensor";
	
	_outputs["MatRes0"] = "MatRes0.xpu_tensor";

	OnnxExecutionContext _runContext(midLevelTransformer);

	logWork.print("Start running graph\n");

	_graph->run(_runContext, _inputs, _outputs);

	logWork.print("Finished running graph\n");

	auto _outputView = *_runContext.getTensorData("MatRes0");

	logWork.print("Matrix is\n");
	for (size_t i = 0; i < 32; i++) {
		for (size_t j = 0; j < 32; j++) {
			logWork.print(fmt::format("{} ", _outputView->at(i, j)));
		}
		logWork.print("\n");
	}
    //fmt::println("onnx test begin");
    //OnnxExecutionContext _context(midLevelTransformer);
    //auto g = OnnxGraph::parse("/home/vserbu/work/xpu/sw/tools/xrt/models/mul.onnx");

    //std::unordered_map<std::string, std::filesystem::path> _inputs;
    //std::unordered_map<std::string, std::filesystem::path> _outputs;

    //_inputs["A"] = "a.xpu_tensor";
    //_inputs["B"] = "b.xpu_tensor";
    //_outputs["X"] = "result.xpu_tensor";



    //g->run(_context, _inputs, _outputs);
    //fmt::println("onnx test end");
}
//-------------------------------------------------------------------------------------
void OnnxTransformer::loadGraph(const Md5Hash& _hash, const std::filesystem::path& _path) {
  loadedGraphs[_hash] = OnnxGraph::parse(_path);
}

//-------------------------------------------------------------------------------------
void OnnxTransformer::loadTensor(const Md5Hash& _hash, const std::filesystem::path& _path) {
  loadedTensors[_hash] = std::make_unique<std::filesystem::path>(_path);
}

//-------------------------------------------------------------------------------------
void OnnxTransformer::run(
  const ResourceIdentifier& _graph,
  const std::unordered_map<std::string, ResourceIdentifier>& _inputs,
  std::unordered_map<std::string, ResourceIdentifier>& _outputs
) {
  resourceLoader->load(_graph);
  for (const auto& [_, _input] : _inputs) {
    resourceLoader->load(_input);
  }

  std::unordered_map<std::string, std::filesystem::path> _inputPaths;

  for (auto &[_inputName, _inputRi] : _inputs) {
    _inputPaths[_inputName] = *loadedTensors[_inputRi.hash];
  }

  auto _outputPaths = outputCache->prepareOutputPaths(_outputs);

  OnnxExecutionContext _executionContext(midLevelTransformer);
  loadedGraphs[_graph.hash]->run(_executionContext, _inputPaths, _outputPaths);

  outputCache->finalizeOutputs(_outputs, _outputPaths);

  logWork.print("Final ONNX outputs:\n");
  for (const auto& [_outputName, _outputRi] : _outputs) {
    logWork.print(fmt::format("|-| {} -> {}", _outputName, _outputRi.toString()));
  }
}

//-------------------------------------------------------------------------------------
