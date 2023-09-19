//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <common/CodeGen.h>
#include <common/Utils.h>
#include <targets/file/FileTarget.h>

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
        arch.IO_INTF_AXILITE_WRITE_REGS_PROG_FIFO_IN_ADDR, waitingInstruction.value());
    waitingInstruction.reset();
}

//-------------------------------------------------------------------------------------
void FileTarget::writeInstruction(uint32_t _instruction) {
    if (!waitingInstruction.has_value()) {
        waitingInstruction = _instruction;

        return;
    }

    controllerFile.print("program {:08x} {:08x}\n", *waitingInstruction, _instruction);
    waitingInstruction.reset();
}

//-------------------------------------------------------------------------------------
void FileTarget::getMatrixArray(
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns) {
    fmt::println("WARNING: FileTarget: Getting matrix array");

    for (uint32_t _i = 0; _i < _numLines; ++_i) {
        for (uint32_t _j = 0; _j < _numColumns; ++_j) {
            _ramMatrix[_i * _ramTotalColumns + _j] = 0;
        }
    }
}

//-------------------------------------------------------------------------------------
void FileTarget::sendMatrixArray(
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns) {
    unsigned int _numDigits = 0;

    for (uint32_t _i = 0; _i < _numLines; ++_i) {
        for (uint32_t _j = 0; _j < _numColumns; ++_j) {
            uint32_t _index =
                (_ramStartLine + _i) * _ramTotalColumns + _ramStartColumn + _j;
            _numDigits = std::max(_numDigits, numDigits(_ramMatrix[_index]));
        }
    }

    for (uint32_t _i = 0; _i < _numLines; ++_i) {
        for (uint32_t _j = 0; _j < _numColumns; ++_j) {
            uint32_t _index =
                (_ramStartLine + _i) * _ramTotalColumns + _ramStartColumn + _j;
            dataFile.print("{:>{}}", _ramMatrix[_index], _numDigits);

            if (_j != _numColumns - 1) {
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
