//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

class Tensor {
    friend class MatrixView;

    uint32_t* data;
    std::vector<size_t> dimensions;

  public:
    Tensor();
    Tensor(std::span<const size_t> dimensions);
    ~Tensor();

    uint32_t& at(std::span<const size_t> coords);
    const uint32_t& at(std::span<const size_t> coords) const;

    inline size_t numDimensions() const {
        return dimensions.size();
    }

    inline size_t getDimensionSize(size_t dim) const {
        return dimensions.at(dim);
    }
};
