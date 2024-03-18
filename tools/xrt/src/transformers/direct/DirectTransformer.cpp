//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/arch/Arch.hpp>
#include <common/debug/Debug.hpp>
#include <common/types/Matrix.hpp>
#include <manager/Manager.hpp>
#include <transformers/direct/DirectTransformer.hpp>

#include <cstdint>
#include <vector>

#include "common/log/Logger.hpp"
#include "fmt/core.h"
#include <indicators/cursor_control.hpp>
#include <indicators/indeterminate_progress_bar.hpp>
#include <indicators/termcolor.hpp>

//-------------------------------------------------------------------------------------
DirectTransformer::DirectTransformer(Manager* _manager, const Arch& _arch)
    : manager(_manager), debugAccImage(std::make_shared<AcceleratorImage>(_arch)) {
    manager->runLowLevel("prim_initialize");
    manager->runLowLevel("test_debug_fill");
}

//-------------------------------------------------------------------------------------
DirectTransformer::~DirectTransformer() {}

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

    indicators::IndeterminateProgressBar bar{
        indicators::option::BarWidth{40},
        indicators::option::Start{"["},
        indicators::option::Fill{"·"},
        indicators::option::Lead{"<==>"},
        indicators::option::End{"]"},
        indicators::option::PostfixText{"Running Function"},
        indicators::option::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}};

    indicators::show_console_cursor(false);

    int ticker = 0;

    do {
        if (ticker++ % 4 == 0) {
            bar.tick();
        }

        if (hitBreakpoint) {
            bar.mark_as_completed();
            indicators::show_console_cursor(true);

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

    bar.mark_as_completed();
    indicators::show_console_cursor(true);

    return 0;
}

//-------------------------------------------------------------------------------------
std::vector<uint32_t>
DirectTransformer::debugGetArrayData(uint32_t _firstCell, uint32_t _lastCell, uint32_t _firstRow, uint32_t _lastRow) {
    uint32_t _numRows  = _lastRow - _firstRow + 1;
    uint32_t _numCells = _lastCell - _firstCell + 1;

    std::vector<uint32_t> _result(_numCells * _numRows);

    fmt::println("size {}", debugAccImage->arrayMem.size());

    bool _mustReread = false;
    if (_firstRow < debugAccImage->arrayMemValidRows.first) {
        debugAccImage->arrayMemValidRows.first = _firstRow;
        _mustReread                            = true;
    }
    if (_lastRow > debugAccImage->arrayMemValidRows.second) {
        debugAccImage->arrayMemValidRows.second = _lastRow;
        _mustReread                             = true;
    }

    if (_mustReread) {
        logWork.print(fmt::format("Rereading Array Memory\n"));
        debugAccImage->rereadArrayMem();
        logWork.print(fmt::format("Rereading Done\n"));
    }

    for (uint32_t _cellIndex = 0; _cellIndex < _numCells; ++_cellIndex) {
        for (uint32_t _rowIndex = 0; _rowIndex < _numRows; ++_rowIndex) {
            _result.at(_cellIndex * _numRows + _rowIndex) =
                debugAccImage->arrayMem.at(_firstRow + _rowIndex).at(_firstCell + _cellIndex);
        }
    }

    logWork.print(fmt::format("debugGetArrayData:\n"));
    for (auto val : _result) {
        logWork.print(fmt::format("\t{}\n", val));
    }

    return _result;
}

//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
void DirectTransformer::debugPutArrayData(
    uint32_t _firstCell, uint32_t _lastCell, uint32_t _firstRow, uint32_t _lastRow, std::span<const uint32_t> _data) {
    uint32_t _numRows  = _lastRow - _firstRow + 1;
    uint32_t _numCells = _lastCell - _firstCell + 1;

    for (uint32_t _cellIndex = 0; _cellIndex < _numCells; ++_cellIndex) {
        for (uint32_t _rowIndex = 0; _rowIndex < _numRows; ++_rowIndex) {
            debugAccImage->arrayMem.at(_firstRow + _rowIndex).at(_firstCell + _cellIndex) =
                _data[_cellIndex * _numRows + _rowIndex];
        }
    }
}

//-------------------------------------------------------------------------------------
std::vector<uint32_t> DirectTransformer::debugGetArrayRegs(uint32_t _firstCell, uint32_t _lastCell) {
    uint32_t _numCells = _lastCell - _firstCell + 1;

    std::vector<uint32_t> _result;

    for (uint32_t _cellIndex = 0; _cellIndex < _numCells; ++_cellIndex) {
        _result.push_back(debugAccImage->arrayAcc.at(_firstCell + _cellIndex));
        _result.push_back(debugAccImage->arrayAddrReg.at(0).at(_firstCell + _cellIndex));
        _result.push_back(debugAccImage->arrayBool.at(_firstCell + _cellIndex));
        _result.push_back(debugAccImage->arrayGlobalShiftReg.at(_firstCell + _cellIndex));
        _result.push_back(debugAccImage->arrayIORegData.at(_firstCell + _cellIndex));
        _result.push_back(0xDEADBEEF);
    }

    logWork.print(fmt::format("debugGetArrayRegs:\n"));
    for (auto val : _result) {
        logWork.print(fmt::format("\t{}\n", val));
    }

    return _result;
}

//-------------------------------------------------------------------------------------
unsigned DirectTransformer::debugSetBreakpoint(std::string_view _functionName, uint32_t _lineNumber) {
    return manager->registerBreakpoint(
        _functionName, _lineNumber, [this](std::shared_ptr<AcceleratorImage> _acc, unsigned _breakpointID) -> bool {
            return handleDebugHitCallback(std::move(_acc), _breakpointID);
        });
}

//-------------------------------------------------------------------------------------
bool DirectTransformer::handleDebugHitCallback(std::shared_ptr<AcceleratorImage> _acc, unsigned _breakpointID) {
    debugAccImage = std::move(_acc);

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
