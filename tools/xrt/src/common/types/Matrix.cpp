//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#include <common/types/Matrix.hpp>

#include <cassert>
#include <cstdint>
#include <cstring>

Matrix::Matrix(size_t _numRows, size_t _numColumns)
    : numRows_(_numRows), numColumns_(_numColumns) {
    data = new uint32_t[_numRows * _numColumns];

    std::memset(data, 0, _numRows * _numColumns);
}

Matrix::~Matrix() {
    delete[] data;
}

uint32_t& Matrix::at(size_t i, size_t j) {
    assert(i < numRows_);
    assert(j < numColumns_);

    return data[i * numColumns_ + j];
}

const uint32_t& Matrix::at(size_t i, size_t j) const {
    assert(i < numColumns_);
    assert(j < numColumns_);

    return data[i * numColumns_ + j];
}

void Matrix::resize(size_t _newNumRows, size_t _newNumColumns) {
    uint32_t* newData = new uint32_t[_newNumRows * _newNumColumns];
    std::memset(newData, 0, _newNumRows * _newNumColumns);

    for (size_t i = 0; i < std::min(_newNumRows, numRows_); i++) {
        for (size_t j = 0; j < std::min(_newNumRows, numRows_); j++) {
            newData[i * _newNumColumns + j] = data[i * numColumns_ + j];
        }
    }
}

MatrixView::MatrixView(Matrix& _matrix)
    : data(_matrix.data),
      totalRows_(_matrix.numRows_),
      totalColumns_(_matrix.numColumns_),
      startLine_(0),
      startColumn_(0),
      numRows_(_matrix.numRows_),
      numColumns_(_matrix.numColumns_) {}

MatrixView::MatrixView(
    Matrix& _matrix,
    size_t _startLine,
    size_t _startColumn,
    size_t _numRows,
    size_t _numColumns)
    : data(_matrix.data),
      totalRows_(_matrix.numRows_),
      totalColumns_(_matrix.numColumns_),
      startLine_(_startLine),
      startColumn_(_startColumn),
      numRows_(_numRows),
      numColumns_(_numColumns) {}

MatrixView::MatrixView(Matrix* _matrix)
    : data(_matrix->data),
      totalRows_(_matrix->numRows_),
      totalColumns_(_matrix->numColumns_),
      startLine_(0),
      startColumn_(0),
      numRows_(_matrix->numRows_),
      numColumns_(_matrix->numColumns_) {}

MatrixView::MatrixView(
    Matrix* _matrix,
    size_t _startLine,
    size_t _startColumn,
    size_t _numRows,
    size_t _numColumns)
    : data(_matrix->data),
      totalRows_(_matrix->numRows_),
      totalColumns_(_matrix->numColumns_),
      startLine_(_startLine),
      startColumn_(_startColumn),
      numRows_(_numRows),
      numColumns_(_numColumns) {}

uint32_t& MatrixView::at(size_t i, size_t j) {
    assert(i < numRows_);
    assert(j < numColumns_);

    return data[(i + startLine_) * totalColumns_ + j + startColumn_];
}

const uint32_t& MatrixView::at(size_t i, size_t j) const {
    assert(i < numRows_);
    assert(j < numColumns_);

    return data[(i + startLine_) * totalColumns_ + j + startColumn_];
}
