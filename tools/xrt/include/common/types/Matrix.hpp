//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------
#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include <fmt/os.h>

class Matrix {
    friend class MatrixView;

    uint32_t* data;

    size_t numRows_;
    size_t numColumns_;

  public:
    Matrix(size_t _numRows, size_t _numColumns);
    ~Matrix();

    Matrix(const Matrix&)            = delete;
    Matrix& operator=(const Matrix&) = delete;

    uint32_t& at(size_t i, size_t j);
    const uint32_t& at(size_t i, size_t j) const;

    void resize(size_t _numRows, size_t _numColumns);

    inline size_t numRows() const {
        return numRows_;
    }

    inline size_t numColumns() const {
        return numColumns_;
    }
};

class MatrixView {
    uint32_t* data;
    size_t totalRows_;
    size_t totalColumns_;
    size_t startLine_;
    size_t startColumn_;
    size_t numRows_;
    size_t numColumns_;

  public:
    MatrixView(Matrix& _matrix);
    MatrixView(
        const Matrix& _matrix,
        size_t startLine,
        size_t startColumn,
        size_t numRows,
        size_t numColumns);
    MatrixView(Matrix* _matrix);
    MatrixView(
        const Matrix* _matrix,
        size_t startLine,
        size_t startColumn,
        size_t numRows,
        size_t numColumns);
    MatrixView(
        const MatrixView* _matrix,
        size_t startLine,
        size_t startColumn,
        size_t numRows,
        size_t numColumns);
    ~MatrixView() = default;

    uint32_t& at(size_t i, size_t j);
    const uint32_t& at(size_t i, size_t j) const;

    inline size_t numRows() const {
        return numRows_;
    }

    inline size_t numColumns() const {
        return numColumns_;
    }
};

void printMatrixView(fmt::ostream& out, const MatrixView* _matrixView);
