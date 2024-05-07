//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#include <common/Utils.hpp>
#include <common/types/Matrix.hpp>

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>

Matrix::Matrix(size_t _numRows, size_t _numColumns) : numRows_(_numRows), numColumns_(_numColumns) {
    data = std::make_shared<std::vector<uint32_t>>(_numRows * _numColumns * sizeof(uint32_t));
    std::fill(data->begin(), data->end(), 0);
}


uint32_t& Matrix::at(size_t i, size_t j) {
    assert(i < numRows_);
    assert(j < numColumns_);

    return (*data)[i * numColumns_ + j];
}

const uint32_t& Matrix::at(size_t i, size_t j) const {
    assert(i < numRows_);
    assert(j < numColumns_);

    return (*data)[i * numColumns_ + j];
}

void Matrix::resize(size_t _newNumRows, size_t _newNumColumns) {
    auto _newData = std::make_shared<std::vector<uint32_t>>(_newNumRows * _newNumColumns * sizeof(uint32_t));
    std::fill(_newData->begin(), _newData->end(), 0);

    for (size_t i = 0; i < std::min(_newNumRows, numRows_); i++) {
        for (size_t j = 0; j < std::min(_newNumRows, numRows_); j++) {
            (*_newData)[i * _newNumColumns + j] = (*data)[i * numColumns_ + j];
        }
    }

    data        = std::move(_newData);
    numRows_    = _newNumRows;
    numColumns_ = _newNumColumns;
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
    const Matrix& _matrix, size_t _startLine, size_t _startColumn, size_t _numRows, size_t _numColumns)
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
    const Matrix* _matrix, size_t _startLine, size_t _startColumn, size_t _numRows, size_t _numColumns)
    : data(_matrix->data),
      totalRows_(_matrix->numRows_),
      totalColumns_(_matrix->numColumns_),
      startLine_(_startLine),
      startColumn_(_startColumn),
      numRows_(_numRows),
      numColumns_(_numColumns) {
    // TODO: Add checks for validity
}

MatrixView::MatrixView(
    std::shared_ptr<const MatrixView> _matrixView,
    size_t startLine,
    size_t startColumn,
    size_t numRows,
    size_t numColumns)
    : data(_matrixView->data),
      totalRows_(_matrixView->totalRows_),
      totalColumns_(_matrixView->totalColumns_),
      startLine_(startLine + _matrixView->startLine_),
      startColumn_(startColumn + _matrixView->startColumn_),
      numRows_(numRows),
      numColumns_(numColumns) {
    // TODO: Add checks for validity
}

uint32_t& MatrixView::at(size_t i, size_t j) {
    assert(i < numRows_);
    assert(j < numColumns_);

    auto _data = data.lock();

    if (_data == nullptr) {
        throw std::runtime_error("Use of MatrixView after Matrix destruction");
    }

    return (*_data)[(i + startLine_) * totalColumns_ + j + startColumn_];
}

const uint32_t& MatrixView::at(size_t i, size_t j) const {
    assert(i < numRows_);
    assert(j < numColumns_);

    auto _data = data.lock();

    if (_data == nullptr) {
        throw std::runtime_error("Use of MatrixView after Matrix destruction");
    }

    return (*_data)[(i + startLine_) * totalColumns_ + j + startColumn_];
}

void printMatrixView(fmt::ostream& out, const MatrixView* _matrixView) {
    unsigned int _numDigits = 0;

    for (uint32_t _i = 0; _i < _matrixView->numRows(); _i++) {
        for (uint32_t _j = 0; _j < _matrixView->numColumns(); _j++) {
            _numDigits = std::max(_numDigits, numDigits(_matrixView->at(_i, _j)));
        }
    }

    for (uint32_t _i = 0; _i < _matrixView->numRows(); _i++) {
        for (uint32_t _j = 0; _j < _matrixView->numColumns(); _j++) {
            out.print("{:>{}}", _matrixView->at(_i, _j), _numDigits);

            if (_j != _matrixView->numColumns() - 1) {
                out.print(" ");
            } else {
                out.print("\n");
            }
        }
    }

    out.print("\n");

    out.flush();
}
