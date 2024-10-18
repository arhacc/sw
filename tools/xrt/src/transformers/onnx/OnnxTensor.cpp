//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <transformers/onnx/OnnxTensor.hpp>
#include <endian/little_endian.hpp>
#include <common/types/Matrix.hpp>
#include <fstream>
#include <common/log/Logger.hpp>

OnnxTensor::OnnxTensor(std::string_view _name)
  : name(_name), type(Type::Undefined)
{}

const std::string& OnnxTensor::getName() const {
  return name;
}

void OnnxTensor::setTypeDim(Type _type, Dimensions _dim) {
  if (type != Type::Undefined && (type != _type || _dim.size() != dim.size() || !std::equal(dim.begin(), dim.end(), _dim.begin()))) {
    throw std::runtime_error("OnnxTensor::setTypeDim called twice for the same tensor with incompatible values");
  }

  type = _type;
  dim = _dim;
}

OnnxTensor::Type OnnxTensor::getType() const {
  return type;
}

OnnxTensor::Dimensions OnnxTensor::getDimensions() const {
  return dim;
}

void *OnnxTensor::getDataForMidLevel(OnnxExecutionContext& _executionContext) {
  auto _matrix = _executionContext.getTensorData(name);
  if (*_matrix == nullptr) {
    _executionContext.allocateTensorData(name, dim);
    _matrix = _executionContext.getTensorData(name);
  }

  assert(*_matrix != nullptr);

  return reinterpret_cast<void *>(_matrix);
}

void OnnxTensor::parseData(OnnxExecutionContext& _executionContext, const std::filesystem::path& _path) {
  /*
  msgpack::zone z;
  auto _matrix = *_executionContext.getTensorData(name);
  if (_matrix == nullptr) {
    _executionContext.allocateTensorData(name, dim);
    _matrix = *_executionContext.getTensorData(name);
  }

  std::ifstream _tensorFile(_path, std::ios::binary);
  std::vector<char> _tensorFileContents((std::istreambuf_iterator<char>(_tensorFile)),
                                         std::istreambuf_iterator<char>());

  auto _mpObjHandle = msgpack::unpack(_tensorFileContents.data(), _tensorFileContents.size());
  auto _mpTensor = _mpObjHandle.get();

  if (_mpTensor.type != msgpack::type::MAP) {
    throw std::runtime_error(".xpu_tensor file does not follow spec: not a map");
  }

  auto _mpTensorMap = _mpTensor.via.map;

  auto _mpTensorAttr = [&](std::string_view _attrName) -> msgpack::object& {
    std::span<msgpack::object_kv> _mpMapKv{_mpTensorMap.ptr, _mpTensorMap.size};

    auto _ret = std::find_if(_mpMapKv.begin(), _mpMapKv.end(), [=](msgpack::object_kv &_kv) {
        if (_kv.key.type != msgpack::type::STR) {
          throw std::runtime_error(".xpu_tensor file does not follow spec: map key not a string");
        }

        auto _key = _kv.key.via.str;
        std::string_view _keyStr{_key.ptr, _key.size};

        return _keyStr == _attrName;
      });

    if (_ret == _mpMapKv.end()) {
      throw std::runtime_error(fmt::format(".xpu_tensor file does not follow spec: attribute {} not found", _attrName));
    }

    return _ret->val;
  };

  auto _mpTensorType = _mpTensorAttr("type");
  if (_mpTensorType.type != msgpack::type::STR) {
    throw std::runtime_error(".xpu_tensor file does not follow spec: \"type\" is not a string");
  }
  std::string_view _mpTensorTypeStr{_mpTensorType.via.str.ptr, _mpTensorType.via.str.size};

  if (_mpTensorTypeStr != "UINT32") {
    throw std::runtime_error(".xpu_tensor: only UINT32 implemented so far");
  }

  auto _mpTensorShape = _mpTensorAttr("shape");
  if (_mpTensorShape.type != msgpack::type::ARRAY) {
    throw std::runtime_error(".xpu_tensor file does not follow spec: \"shape\" is not an array");
  }
  std::span<msgpack::object> _mpTensorShapeArray
      {_mpTensorShape.via.array.ptr, _mpTensorShape.via.array.size};
  
  dim.clear();

  for (auto _mpTensorShapeDim : _mpTensorShapeArray) {
    if (_mpTensorShapeDim.type != msgpack::type::POSITIVE_INTEGER) {
      throw std::runtime_error(".xpu_tensor file does not follow spec: \"shape\" element is not a positive integer");
    }
    
    dim.push_back(_mpTensorShapeDim.via.u64);
  }

  if (dim.size() != 2) {
    throw std::runtime_error(".xpu_tensor: only matrixes supported so far");
  }

  auto _mpTensorData = _mpTensorAttr("data");
  if (_mpTensorData.type != msgpack::type::BIN) {
    throw std::runtime_error(".xpu_tensor file does not follow spec: \"data\" is not a binary blob");
  }

  uint64_t _totalElements = 1;
  for (auto _dim : dim) {
    _totalElements *= _dim;
  }

  std::span<uint8_t> _mpTensorDataBin = {(uint8_t *)_mpTensorData.via.bin.ptr, _mpTensorData.via.bin.size};
  if (_mpTensorDataBin.size() != _totalElements * sizeof(uint32_t)) {
    throw std::runtime_error(fmt::format(".xpu_tensor file does not follow spec: mismatch between reported size in \"shape\" (suggests {} elements) and \"data\" size ({} bytes)", _totalElements, _mpTensorDataBin.size()));
  }

  // TODO: generalize
  size_t _dataIt = 0;
  for (size_t i = 0; i < _matrix->numRows(); i++) {
    for (size_t j = 0; j < _matrix->numColumns(); j++) {
      _matrix->at(i, j) = endian::little_endian::get<uint32_t>(_mpTensorDataBin.data() + _dataIt);
      _dataIt += sizeof(uint32_t);
    }
  }

  //
  //logWork.println<InfoMedium>("Loaded tensor {} from file {}", name, _path.string());*/
}

struct OnnxTensorMsgPack {
  std::string type;
  std::vector<uint64_t> shape;
  std::vector<uint8_t> data;

  //MSGPACK_DEFINE_MAP(type, shape, data)
};

void OnnxTensor::writeData(OnnxExecutionContext& _executionContext, const std::filesystem::path &_path) {
  /*auto _matrix = *_executionContext.getTensorData(name);
  if (_matrix == nullptr) {
    _executionContext.allocateTensorData(name, dim);
    _matrix = *_executionContext.getTensorData(name);
  }

  OnnxTensorMsgPack _onnxTensorMsgPack;

  _onnxTensorMsgPack.type = "UINT32";
  _onnxTensorMsgPack.shape = dim;
  _onnxTensorMsgPack.data.resize(_matrix->numRows() * _matrix->numColumns() * sizeof(uint32_t));

  size_t _dataIt = 0;

  for (size_t i = 0; i < _matrix->numRows(); i++) {
    for (size_t j = 0; j < _matrix->numColumns(); j++) {
      endian::little_endian::put(_matrix->at(i, j), _onnxTensorMsgPack.data.data() + _dataIt);
      _dataIt += sizeof(uint32_t);
    }
  }

   // Serialize the data
  std::stringstream buffer;
  msgpack::pack(buffer, _onnxTensorMsgPack);

  // Get the serialized data
  std::string serializedData = buffer.str();

  // Write the serialized data to a file
  std::ofstream _outFile(_path, std::ios::binary);
  if (!_outFile) {
    logWork.println<Error>("Failed to open the file {} for writing!", _path.string());
  }
  _outFile.write(serializedData.data(), serializedData.size());
  _outFile.close();*/
}

