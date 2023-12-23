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
#include <common/log/Logger.hpp>
#include <common/types/Matrix.hpp>
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
    logInit.print("Starting SimTarget...\n");
    tb = new Tb(
        cDesignDirPath / "simulator_axi" / "xsimk.so",
        "librdi_simulator_kernel.so",
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
    tb->reset();
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
void SimTarget::getMatrixArray(MatrixView* _matrixView) {
    logWork.print("SimTarget: Getting matrix array\n");

    size_t _transferLength = _matrixView->numRows() * _matrixView->numColumns();

    std::vector<uint64_t> _data = tb->axiStreamRead(_transferLength / 2);

    size_t _k = 0, _shift = 32;
    for (uint32_t _i = 0; _i < _matrixView->numRows(); _i++) {
        for (uint32_t _j = 0; _j < _matrixView->numColumns(); _j++) {
            _matrixView->at(_i, _j) = _data.at(_k / 2) >> _shift;

            _k++;
            _shift ^= 32;
        }
    }

#ifdef XRT_FULL_IO_LOG
    printMatrixView(logAXIStreamRead.out(), _matrixView);
#endif
}

//-------------------------------------------------------------------------------------
void SimTarget::sendMatrixArray(const MatrixView* _matrixView) {
    logWork.print("SimTarget: Sending matrix array\n");

    std::size_t _transferLength = _matrixView->numRows() * _matrixView->numColumns();
    assert(_transferLength % 2 == 0);

    std::vector<uint64_t> _data(_transferLength / 2, 0);

    std::size_t _k = 0, _shift = 32;
    for (uint32_t _i = 0; _i < _matrixView->numRows(); _i++) {
        for (uint32_t _j = 0; _j < _matrixView->numColumns(); _j++) {
            _data.at(_k / 2) |= static_cast<uint64_t>(_matrixView->at(_i, _j)) << _shift;

            _k++;
            _shift ^= 32;
        }
    }

    tb->axiStreamWrite(_data);

#ifdef XRT_FULL_IO_LOG
    printMatrixView(logAXIStreamWrite.out(), _matrixView);
#endif
}

//-------------------------------------------------------------------------------------
