//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <common/types/Tensor.hpp>

#include <cstddef>
#include <cstdint>
#include <memory>

#include "common/allocator/SAllocator.hpp"
#include <fmt/os.h>

class Matrix {
    friend class MatrixView;

    volatile int32_t *data;

    size_t numRows_;
    size_t numColumns_;

  public:
    Matrix(size_t _numRows, size_t _numColumns);
    ~Matrix();

    Matrix(const Matrix&)            = delete;
    Matrix(Matrix&&)                 = default;
    Matrix& operator=(const Matrix&) = delete;
    Matrix& operator=(Matrix&&)      = default;

    volatile int32_t& at(size_t i, size_t j);
    const volatile int32_t& at(size_t i, size_t j) const;

    void resize(size_t _numRows, size_t _numColumns);

    inline size_t numRows() const {
        return numRows_;
    }

    inline size_t numColumns() const {
        return numColumns_;
    }
};

class MatrixView {
    volatile int32_t* data;

    size_t totalRows_;
    size_t totalColumns_;
    size_t startLine_;
    size_t startColumn_;
    size_t numRows_;
    size_t numColumns_;

  public:
    MatrixView(std::shared_ptr<Matrix> _matrix);
    MatrixView(Matrix& _matrix);
    MatrixView(const Matrix& _matrix, size_t startLine, size_t startColumn, size_t numRows, size_t numColumns);
    MatrixView(Matrix* _matrix);
    MatrixView(const Matrix* _matrix, size_t startLine, size_t startColumn, size_t numRows, size_t numColumns);
    MatrixView(
        std::shared_ptr<const MatrixView> _matrix,
        size_t _startLine,
        size_t _startColumn,
        size_t _numRows,
        size_t _numColumns);
    MatrixView(std::shared_ptr<const Tensor> _tensor);
    ~MatrixView() = default;

    volatile int32_t& at(size_t i, size_t j);
    volatile const int32_t& at(size_t i, size_t j) const;

    inline size_t numRows() const {
        return numRows_;
    }

    inline size_t numColumns() const {
        return numColumns_;
    }

    inline size_t totalRows() const {
        return totalRows_;
    }

    inline size_t totalColumns() const {
        return totalColumns_;
    }

    inline std::uintptr_t physicalAddress() const {
        return gsAllocator->getPhysicalAddress(data);
    }
};

void printMatrixView(fmt::ostream& out, const MatrixView* _matrixView);
