//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/types/Matrix.hpp>
#include <manager/Manager.hpp>
#include <transformers/direct/DirectTransformer.hpp>

#include <cstdint>

//-------------------------------------------------------------------------------------
DirectTransformer::DirectTransformer(Manager* _manager)
    : manager(_manager), debugMemoryImage(new Matrix(1024, 16)) { // TODO: replace with constants from Arch structure

    for (int i = 0; i < 1024; i++) {
        for (int j = 0; j < 16; j++) {
            debugMemoryImage->at(i, j) = 15;
        }
    }

    manager->runLowLevel("prim_initialize");

    pushDebugArrayDataMeoryImage();

    updateDebugArrayDataMemoryImage();

    for (int i = 0; i < 1024; i++) {
        for (int j = 0; j < 16; j++) {
            fmt::print("{:4} ", debugMemoryImage->at(i, j));
        }
        fmt::print("\n");
    }
}

DirectTransformer::~DirectTransformer() {
    delete debugMemoryImage;
}

//-------------------------------------------------------------------------------------
void DirectTransformer::load(const std::string& _path) {
    //  printf("DirectTransformer.loadCode @%d, length=%d\n", _address, _length);
    manager->load(_path);
}

//-------------------------------------------------------------------------------------
void DirectTransformer::run(const std::string& _name) {
    //  printf("DirectTransformer.loadCode @%d, length=%d\n", _address, _length);
    // manager->run(_name);
}

//-------------------------------------------------------------------------------------
void DirectTransformer::updateDebugArrayDataMemoryImage() {
    // TODO: update efficiently, remove hardcoded values
    for (size_t _i = 0; _i < 1024 / 128; ++_i) {
        manager->readMatrixArray(128 * _i, {debugMemoryImage, 128 * _i, 0, 128, 16}, false);
    }
}

//-------------------------------------------------------------------------------------
void DirectTransformer::pushDebugArrayDataMeoryImage() {
    manager->runLowLevel("prim_wait_matrices", {1024 / 128});

    for (size_t _i = 0; _i < 1024 / 128; ++_i) {
        manager->writeMatrixArray(128 * _i, {debugMemoryImage, 128 * _i, 0, 128, 16}); // TODO: change to be ok
    }
}

//-------------------------------------------------------------------------------------
std::vector<uint32_t>
DirectTransformer::debugGetArrayData(uint32_t _firstCell, uint32_t _lastCell, uint32_t _firstRow, uint32_t _lastRow) {
    uint32_t _numRows  = _lastRow - _firstRow + 1;
    uint32_t _numCells = _lastCell - _firstCell + 1;

    std::vector<uint32_t> _result(_numCells * _numRows);

    std::fill(_result.begin(), _result.end(), 7);

    updateDebugArrayDataMemoryImage();

    for (uint32_t _cellIndex = 0; _cellIndex < _numCells; ++_cellIndex) {
        for (uint32_t _rowIndex = 0; _rowIndex < _numRows; ++_rowIndex) {
            _result.at(_cellIndex * _numRows + _rowIndex) =
                debugMemoryImage->at((_firstRow + _rowIndex), (_firstCell + _cellIndex));
        }
    }

    return _result;
}

//-------------------------------------------------------------------------------------
void DirectTransformer::debugPutArrayData(
    uint32_t _firstCell, uint32_t _lastCell, uint32_t _firstRow, uint32_t _lastRow, std::span<const uint32_t> _data) {
    uint32_t _numRows  = _lastRow - _firstRow + 1;
    uint32_t _numCells = _lastCell - _firstCell + 1;

    updateDebugArrayDataMemoryImage();

    for (uint32_t _cellIndex = 0; _cellIndex < _numCells; ++_cellIndex) {
        for (uint32_t _rowIndex = 0; _rowIndex < _numRows; ++_rowIndex) {
            debugMemoryImage->at((_firstRow + _rowIndex), (_firstCell + _cellIndex)) =
                _data[_cellIndex * _numRows + _rowIndex];
        }
    }

    pushDebugArrayDataMeoryImage();
}

//-------------------------------------------------------------------------------------
