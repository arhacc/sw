//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/debug/Debug.hpp>
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
}

//-------------------------------------------------------------------------------------
DirectTransformer::~DirectTransformer() {
    delete debugMemoryImage;
}

//-------------------------------------------------------------------------------------
void DirectTransformer::load(const std::string& _path) {
    manager->load(_path);
}

//-------------------------------------------------------------------------------------
int DirectTransformer::run(const std::string& _name) {
    manager->runLowLevel(_name);

    return waitForFunctionEnd();
}

//-------------------------------------------------------------------------------------
unsigned DirectTransformer::getActiveBreakpointID() {
    return hitBreakpointID;
}

// TODO: This should be a midlevel function
//-------------------------------------------------------------------------------------
int DirectTransformer::waitForFunctionEnd() {
    uint32_t status_reg;
    uint32_t accelerator_program_state;
    uint32_t dte_state;
    uint32_t prog_fifo_empty;
    uint32_t data_in_fifo_empty;
    uint32_t data_out_fifo_empty;
    uint32_t debug_on;

    do {
        if (hitBreakpoint) {
            return 1;
        }

        status_reg = manager->readRegister(manager->constant(ArchConstant::IO_INTF_AXILITE_READ_REGS_STATUS_REG_ADDR));

        accelerator_program_state =
            status_reg >> manager->constant(ArchConstant::XPU_STATUS_REG_ACCELERATOR_PROGRAM_STATE_LOWER);
        accelerator_program_state =
            accelerator_program_state
            & ((1 << manager->constant(ArchConstant::XPU_STATUS_REG_ACCELERATOR_PROGRAM_STATE_NR_BITS)) - 1);

        dte_state = status_reg >> manager->constant(ArchConstant::XPU_STATUS_REG_DTE_STATE_LOWER);
        dte_state = dte_state & ((1 << manager->constant(ArchConstant::XPU_STATUS_REG_DTE_STATE_NR_BITS)) - 1);

        prog_fifo_empty = status_reg >> manager->constant(ArchConstant::XPU_STATUS_REG_PROG_FIFO_EMPTY_LOC_LOWER);
        prog_fifo_empty =
            prog_fifo_empty & ((1 << manager->constant(ArchConstant::XPU_STATUS_REG_PROG_FIFO_EMPTY_NR_BITS)) - 1);

        data_in_fifo_empty = status_reg >> manager->constant(ArchConstant::XPU_STATUS_REG_DATA_IN_FIFO_EMPTY_LOC_LOWER);
        data_in_fifo_empty = data_in_fifo_empty
                             & ((1 << manager->constant(ArchConstant::XPU_STATUS_REG_DATA_IN_FIFO_EMPTY_NR_BITS)) - 1);

        data_out_fifo_empty =
            status_reg >> manager->constant(ArchConstant::XPU_STATUS_REG_DATA_OUT_FIFO_EMPTY_LOC_LOWER);
        data_out_fifo_empty =
            data_out_fifo_empty
            & ((1 << manager->constant(ArchConstant::XPU_STATUS_REG_DATA_OUT_FIFO_EMPTY_NR_BITS)) - 1);

        debug_on = status_reg >> manager->constant(ArchConstant::XPU_STATUS_REG_DEBUG_ON_LOWER);
        debug_on = debug_on & ((1 << manager->constant(ArchConstant::XPU_STATUS_REG_DEBUG_ON_NR_BITS)) - 1);
    } while ((accelerator_program_state != manager->constant(ArchConstant::ACCELERATOR_STATE_HALT))
             || (dte_state != manager->constant(ArchConstant::DTE_STATE_IDLE)) || (prog_fifo_empty != 1)
             || (data_in_fifo_empty != 1) || (data_out_fifo_empty != 1) || (debug_on != 0));

    return 0;
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
unsigned DirectTransformer::debugSetBreakpoint(std::string_view _functionName, uint32_t _lineNumber) {
    return manager->registerBreakpoint(
        _functionName, _lineNumber, [this](AcceleratorImage& _acc, unsigned _breakpointID) -> bool {
            return handleDebugHitCallback(_acc, _breakpointID);
        });
}

//-------------------------------------------------------------------------------------
bool DirectTransformer::handleDebugHitCallback(AcceleratorImage& _acc, unsigned _breakpointID) {
    uint32_t i = 0;
    for (std::vector<uint32_t>& _arrayMemRow : _acc.arrayMem) {
        uint32_t j = 0;
        for (uint32_t& _arrayMemValue : _arrayMemRow) {
            debugMemoryImage->at(i, j++) = _arrayMemValue;
        }

        i++;
    }

    hitBreakpoint   = true;
    hitBreakpointID = manager->hwBreakpoint2UserBreakpointID(_breakpointID);

    return false;
}

//-------------------------------------------------------------------------------------
void DirectTransformer::debugContinue() {
    hitBreakpoint = false;

    manager->continueAfterBreakpoint();
}

//-------------------------------------------------------------------------------------
