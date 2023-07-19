//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <cstdint>
#include <manager/Manager.h>
#include <transformers/direct/DirectTransformer.h>

//-------------------------------------------------------------------------------------
DirectTransformer::DirectTransformer(Manager *_manager)
    : manager(_manager), debugArrayDataMemoryImage(4 * 1024 * 16) { // TODO: replace with constants from Arch structure

}

//-------------------------------------------------------------------------------------
void DirectTransformer::load(const std::string &_path) {
    //  printf("DirectTransformer.loadCode @%d, length=%d\n", _address, _length);
    manager->load(_path);
}

//-------------------------------------------------------------------------------------
void DirectTransformer::run(const std::string &_name) {
    //  printf("DirectTransformer.loadCode @%d, length=%d\n", _address, _length);
    manager->run(_name);
}

//-------------------------------------------------------------------------------------
std::vector<uint32_t> DirectTransformer::debugGetArrayData(uint32_t _firstCell, uint32_t _lastCell, uint32_t _firstRow, uint32_t _lastRow) {
    uint32_t _numRows = _lastRow - _firstRow + 1;
    uint32_t _numCells = _lastCell - _firstCell + 1;

    std::vector<uint32_t> _result(_numCells * _numRows);

    // TODO: update efficiently, remove hardcoded values
    for (size_t _i = 0; _i < 1024 / 128; ++_i) {
        manager->readMatrixArray(
            128 * _i, debugArrayDataMemoryImage.data(),
            1024, 16,
            128 * _i, 0,
            128, 16,
            false
        );
    }

    for (uint32_t _cellIndex = 0; _cellIndex < _numCells; ++_cellIndex) {
        for (uint32_t _rowIndex = 0; _rowIndex < _numRows; ++_rowIndex) {
            _result.at(_cellIndex * _numRows + _rowIndex)
                = debugArrayDataMemoryImage.at((_firstRow + _rowIndex) * 16 + (_firstCell + _cellIndex));
        }
    }

    return _result;
}

//-------------------------------------------------------------------------------------
