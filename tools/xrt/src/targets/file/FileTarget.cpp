//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------

#include <cstdint>
#include <fmt/os.h>
#include <stdexcept>
#include <sys/types.h>
#include <targets/file/FileTarget.h>

#include <cstdio>
#include <cinttypes>
#include <iomanip>
#include <common/CodeGen.h>
#include <fmt/printf.h>

//-------------------------------------------------------------------------------------
FileTarget::FileTarget(std::string_view _path, const Arch& _arch) :
    arch(_arch),
    controllerFile(fmt::output_file(std::string(_path) + ".ctrl")),
    dataFile(fmt::output_file(std::string(_path) + ".ctrl"))
{}

//-------------------------------------------------------------------------------------
void FileTarget::writeInstruction(uint32_t _instruction) {
    if (!ctrl_col)
        controllerFile.print(" ");
    controllerFile.print("{:08X}", _instruction);
    if (!ctrl_col)
        controllerFile.print("\n");
    
    ctrl_col = !ctrl_col;
}

//-------------------------------------------------------------------------------------
void FileTarget::writeInstruction(uint8_t _instructionByte, uint32_t _argument) {
    writeInstruction(makeInstruction(arch, _instructionByte, _argument));
}

//-------------------------------------------------------------------------------------
void FileTarget::writeCode(uint32_t _address, uint32_t *_code, uint32_t _length) {
    printf("Writing code at 0x%08" PRIx32 " ", _address);
    printf("length = %5" PRId32 " (0x%016" PRIx32 ")\n", _length, _length);

    writeInstruction(arch.INSTRB_pload, _address);
    writeInstruction(arch.INSTR_nop);

    for (uint32_t _i = 0; _i < _length; ++_i) {
        writeInstruction(_code[_i]);
    }

    writeInstruction(arch.INSTRB_prun, 0);
    writeInstruction(arch.INSTR_nop);
}

//-------------------------------------------------------------------------------------
void FileTarget::runRuntime(uint32_t _address, uint32_t _argc, uint32_t *_args) {
    printf("Running code at 0x%016" PRIx32 "\n", _address);

    writeInstruction(arch.INSTRB_prun, _address);
    writeInstruction(arch.INSTR_nop);

    while (_args && *_args) {
        writeInstruction(*_args++);
        writeInstruction(arch.INSTR_nop);
    }
}

//-------------------------------------------------------------------------------------
void FileTarget::readMatrixArray(uint32_t _accMemStart,
                                 uint32_t *_ramMatrix,
                                 uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                                 uint32_t _ramStartLine, uint32_t _ramStartColumn,
                                 uint32_t _numLines, uint32_t _numColumns,
                                 bool     _accRequireResultReady) {
    fmt::println("FileTarget: Reading matrix");
    
    if (_accRequireResultReady) {
        fmt::println(" (waiting for result)");
    } else {
        fmt::println(" (not waiting for result)");
    }

    writeInstruction(_accRequireResultReady 
                    ? arch.INSTR_get_matrix_array_w_result_ready
                    : arch.INSTR_get_matrix_array_wo_result_ready);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _accMemStart);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _numLines);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(_numColumns);
    writeInstruction(arch.INSTR_nop);

    getMatrixArray(
        _ramMatrix,
        _ramTotalLines, _ramTotalColumns,
        _ramStartLine, _ramStartColumn,
        _numLines, _numColumns
    );
}


//-------------------------------------------------------------------------------------
void FileTarget::getMatrixArray(uint32_t *_ramMatrix,
                                uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                                uint32_t _ramStartLine, uint32_t _ramStartColumn,
                                uint32_t _numLines, uint32_t _numColumns) {
    fmt::println("WARNING: FileTarget: Getting matrix array");

    for (uint32_t _i = 0; _i < _numLines; ++_i) {
        for (uint32_t _j = 0; _j < _numColumns; ++_j) {
            _ramMatrix[_i * _ramTotalColumns + _j] = 0;
        }
    }
}

//-------------------------------------------------------------------------------------
void FileTarget::writeMatrixArray(uint32_t _accMemStart,
                                  uint32_t *_ramMatrix,
                                  uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                                  uint32_t _ramStartLine, uint32_t _ramStartColumn,
                                  uint32_t _numLines, uint32_t _numColumns) {
    
    fmt::println("FileTarget: Writing matrix");

    writeInstruction(arch.INSTR_send_matrix_array);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _accMemStart);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(0, _numLines);
    writeInstruction(arch.INSTR_nop);
    writeInstruction(_numColumns);
    writeInstruction(arch.INSTR_nop);

    sendMatrixArray(
        _ramMatrix,
        _ramTotalLines, _ramTotalColumns,
        _ramStartLine, _ramStartColumn,
        _numLines, _numColumns
        );
}

//-------------------------------------------------------------------------------------
void FileTarget::sendMatrixArray(uint32_t *_ramMatrix,
                                uint32_t _ramTotalLines, uint32_t _ramTotalColumns,
                                uint32_t _ramStartLine, uint32_t _ramStartColumn,
                                uint32_t _numLines, uint32_t _numColumns) {

    for (uint32_t _i = 0; _i < _numLines; ++_i) {
        for (uint32_t _j = 0; _j < _numColumns; ++_j) {
            dataFile.print("{:08X}", _ramMatrix[_i * _ramTotalColumns + _j]);

            if (_j != _numColumns - 1) {
                dataFile.print(" ");
            } else {
                dataFile.print("\n");
            }
        }
    }
}

//-------------------------------------------------------------------------------------
uint32_t FileTarget::readRegister(uint32_t _address) {
    fmt::println("WARNING: Reading register from file target");

    return 0;
}

//-------------------------------------------------------------------------------------
// UNIMPLEMENTED
//-------------------------------------------------------------------------------------
void FileTarget::reset() {
}

//-------------------------------------------------------------------------------------
void FileTarget::runDebug(uint32_t _address, uint32_t *_args, uint32_t _breakpointAddress) {

}

//-------------------------------------------------------------------------------------
void FileTarget::writeRegister(uint32_t _address, uint32_t _register) {

}

//-------------------------------------------------------------------------------------
void FileTarget::readControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
}

//-------------------------------------------------------------------------------------
void FileTarget::writeControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {

}

//-------------------------------------------------------------------------------------
void FileTarget::dump(const std::string &_address) {

}

//-------------------------------------------------------------------------------------
