#include <common/types/Tensor.hpp>

#include <cassert>
#include <cstdint>
#include <functional>
#include <numeric>
#include <stdexcept>

Tensor::Tensor() : data(nullptr) {}

Tensor::Tensor(std::span<const size_t> _dimensions) : dimensions(_dimensions.begin(), _dimensions.end()) {
    if (_dimensions.size() == 0) {
        data = nullptr;
        return;
    }

    size_t _totalSize = std::reduce(_dimensions.begin(), _dimensions.end(), 1, std::multiplies<>());
    data              = new uint32_t[_totalSize];
}

Tensor::~Tensor() {
    delete[] data;
}

uint32_t& Tensor::at(std::span<const size_t> coords) {
    if (coords.size() != dimensions.size()) {
        throw std::runtime_error("tensor access with wrong dimensions");
    }

    size_t _index = 0;
    for (size_t _dim = 0; _dim < dimensions.size(); _dim++) {
        size_t _multiplyer = 1;
        for (size_t _i = _dim + 1; _i < dimensions.size(); _i++) {
            _multiplyer *= dimensions[_i];
        }

        assert(coords[_dim] <= dimensions[_dim]);
        _index += coords[_dim] * _multiplyer;
    }

    return data[_index];
}

const uint32_t& Tensor::at(std::span<const size_t> coords) const {
    if (coords.size() != dimensions.size()) {
        throw std::runtime_error("tensor access with wrong dimensions");
    }

    size_t _index = 0;
    for (size_t _dim = 0; _dim < dimensions.size(); _dim++) {
        size_t _multiplyer = 1;
        for (size_t _i = _dim + 1; _i < dimensions.size(); _i++) {
            _multiplyer *= dimensions[_i];
        }

        assert(coords[_dim] <= dimensions[_dim]);
        _index += coords[_dim] * _multiplyer;
    }

    return data[_index];
}
