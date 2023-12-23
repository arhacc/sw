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
#include <common/arch/generated/ArchConstants.hpp>
#include <common/types/Matrix.hpp>
#include <targets/file/FileTarget.hpp>

#include <cinttypes>
#include <cstdint>
#include <cstdio>
#include <iomanip>
#include <stdexcept>

#include <fmt/os.h>
#include <fmt/printf.h>
#include <sys/types.h>

//-------------------------------------------------------------------------------------
FileTarget::FileTarget(std::string_view _path, const Arch& _arch)
    : arch(_arch),
      controllerFile(fmt::output_file(std::string(_path) + ".ctrl")),
      dataFile(fmt::output_file(std::string(_path) + ".data")) {}

//-------------------------------------------------------------------------------------
void FileTarget::dumpWaitingInstruction() {
    writeRegister(
        arch.get(ArchConstant::IO_INTF_AXILITE_WRITE_REGS_PROG_FIFO_IN_ADDR),
        waitingInstruction.value());
    waitingInstruction.reset();
}

//-------------------------------------------------------------------------------------
void FileTarget::writeInstruction(uint32_t _instruction) {
    if (!waitingInstruction.has_value()) {
        waitingInstruction = _instruction;

        return;
    }

    controllerFile.print("program {:08x} {:08x}\n", *waitingInstruction, _instruction);
    controllerFile.flush();
    waitingInstruction.reset();
}

//-------------------------------------------------------------------------------------
void FileTarget::getMatrixArray(MatrixView* _matrixView) {
    fmt::println("WARNING: FileTarget: Getting matrix array");

    for (uint32_t _i = 0; _i < _matrixView->numRows(); _i++) {
        for (uint32_t _j = 0; _j < _matrixView->numColumns(); _j++) {
            _matrixView->at(_i, _j) = 0;
        }
    }
}

//-------------------------------------------------------------------------------------
void FileTarget::sendMatrixArray(const MatrixView* _matrixView) {
    unsigned int _numDigits = 0;

    for (uint32_t _i = 0; _i < _matrixView->numRows(); _i++) {
        for (uint32_t _j = 0; _j < _matrixView->numColumns(); _j++) {
            _numDigits = std::max(_numDigits, numDigits(_matrixView->at(_i, _j)));
        }
    }

    for (uint32_t _i = 0; _i < _matrixView->numRows(); _i++) {
        for (uint32_t _j = 0; _j < _matrixView->numColumns(); _j++) {
            dataFile.print("{:>{}}", _matrixView->at(_i, _j), _numDigits);

            if (_j != _matrixView->numColumns() - 1) {
                dataFile.print(" ");
            } else {
                dataFile.print("\n");
            }
        }
    }

    dataFile.print("\n");

    dataFile.flush();
}

//-------------------------------------------------------------------------------------
uint32_t FileTarget::readRegister(uint32_t _address) {
    fmt::println("WARNING: Reading register from file target returns 0");

    controllerFile.print("read {:08x}\n", _address);
    controllerFile.flush();

    return 0;
}

//-------------------------------------------------------------------------------------
void FileTarget::writeRegister(uint32_t _address, uint32_t _value) {
    controllerFile.print("write {:08x} {:08x}\n", _address, _value);
    controllerFile.flush();
}

//-------------------------------------------------------------------------------------
// UNIMPLEMENTED
//-------------------------------------------------------------------------------------
void FileTarget::reset() {}

//-------------------------------------------------------------------------------------
