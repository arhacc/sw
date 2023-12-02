//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/CodeGen.hpp>
#include <common/Utils.hpp>
#include <common/arch/Arch.hpp>
#include <targets/sim/SimTarget.hpp>
#include <targets/sim/Tb.hpp>

#include <cassert>
#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <vector>

#include <fmt/core.h>

const std::filesystem::path SimTarget::cDesignDirPath = getXpuHome() / "lib" / "xsim.dir";

//-------------------------------------------------------------------------------------
SimTarget::SimTarget(const Arch& _arch) : arch(_arch) {
    fmt::println("Starting SimTarget...");
    tb = new Tb(
        cDesignDirPath / "simulator_axi" / "xsimk.so",
        "librdi_simulator_kernel.so",
        1,
        "clock",
        "resetn",
        _arch);
}

//-------------------------------------------------------------------------------------
SimTarget::~SimTarget() {
    delete tb;
}

//-------------------------------------------------------------------------------------
void SimTarget::reset() {
    tb->doResetActive();
    tb->wait_clock_cycle(1);
    tb->doResetInactive();
    tb->wait_clock_cycle(1);
}

//-------------------------------------------------------------------------------------
uint32_t SimTarget::readRegister(uint32_t _address) {
    return tb->axiRead(_address);
}

//-------------------------------------------------------------------------------------
void SimTarget::writeRegister(uint32_t _address, uint32_t _register) {
    tb->axiWrite(_address, _register);
}

//-------------------------------------------------------------------------------------
void SimTarget::writeInstruction(uint32_t _instruction) {
    writeRegister(
        arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_PROG_FIFO_IN_ADDR),
        _instruction);
}

//-------------------------------------------------------------------------------------
void SimTarget::getMatrixArray(
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns) {
    assert(_ramStartLine + _numLines <= _ramTotalLines);
    assert(_ramStartColumn + _numColumns <= _ramTotalColumns);
    fmt::println("SimTarget: Getting matrix array");

    std::size_t _transferLength = _numLines * _numColumns;

    std::vector<uint64_t> _data = tb->axiStreamRead(_transferLength / 2);

    std::size_t _k = 0, _shift = 32;
    for (uint32_t _i = _ramStartLine; _i < _ramStartLine + _numLines; _i++) {
        for (uint32_t _j = _ramStartColumn; _j < _ramStartColumn + _numColumns; _j++) {
            _ramMatrix[_i * _ramTotalColumns + _j] = _data.at(_k / 2) >> _shift;

            _k++;
            _shift ^= 32;
        }
    }
}

//-------------------------------------------------------------------------------------
void SimTarget::sendMatrixArray(
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns) {
    assert(_ramStartLine + _numLines <= _ramTotalLines);
    assert(_ramStartColumn + _numColumns <= _ramTotalColumns);
    fmt::println("SimTarget: Sending matrix array");

    std::size_t _transferLength = _numLines * _numColumns;
    assert(_transferLength % 2 == 0);

    std::vector<uint64_t> _data(_transferLength / 2, 0);

    std::size_t _k = 0, _shift = 32;
    for (uint32_t _i = _ramStartLine; _i < _ramStartLine + _numLines; _i++) {
        for (uint32_t _j = _ramStartColumn; _j < _ramStartColumn + _numColumns; _j++) {
            _data.at(_k / 2) |=
                static_cast<uint64_t>(_ramMatrix[_i * _ramTotalColumns + _j]) << _shift;

            _k++;
            _shift ^= 32;
        }
    }

    tb->axiStreamWrite(_data);
}

//-------------------------------------------------------------------------------------
