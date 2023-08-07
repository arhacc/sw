//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/CodeGen.h>
#include <targets/sim/SimTarget.h>

#include <cinttypes>
#include <cstdint>

//-------------------------------------------------------------------------------------
void SimTarget::writeInstruction(uint32_t _instruction) {
    programFile.push_back(_instruction);
}

//-------------------------------------------------------------------------------------
void SimTarget::writeInstruction(uint8_t _instructionByte, uint32_t _argument) {
    writeInstruction(makeInstruction(arch, _instructionByte, _argument));
}

//-------------------------------------------------------------------------------------
SimTarget::SimTarget(const Arch& _arch) : arch(_arch) {
    fmt::println("Starting SimTarget...");

#if 0
    try {
        std::string design_libname = "../build/xsim.dir/xsim/xsimk.so";

        xpuTestBench = new XpuTestBench(
            design_libname, "librdi_simulator_kernel.so", 1, "clock", "resetn");
        xpuTestBench->tb_init();
        xpuTestBench->do_reset(10);
        xpuTestBench->wait_clock_cycles(30);
        xpuTestBench->AXI_Lite_write(200, 300);
        xpuTestBench->AXI_Lite_write(400, 500);
        xpuTestBench->run_ncycles(100);
    } catch (std::exception& _e) {
        std::cout << "Could not load XSI simulation shared library!" << std::endl;
    }
#endif
}

//-------------------------------------------------------------------------------------
void SimTarget::reset() {}

//-------------------------------------------------------------------------------------
void SimTarget::runRuntime(uint32_t _address, uint32_t _argc, uint32_t* _args) {
    printf("Running code at 0x%016" PRIx32 "\n", _address);

    writeInstruction(arch.INSTRB_prun, _address);
    writeInstruction(arch.INSTR_nop);

    for (uint32_t _i = 0; _i < _argc; _i++) {
        writeInstruction(_args[_i]);
        writeInstruction(arch.INSTR_nop);
    }
}

//-------------------------------------------------------------------------------------
void SimTarget::runDebug(
    uint32_t _address, uint32_t* _args, uint32_t _breakpointAddress) {}

//-------------------------------------------------------------------------------------
uint32_t SimTarget::readRegister(uint32_t _address) {
    return 0;
}

//-------------------------------------------------------------------------------------
void SimTarget::writeRegister(uint32_t _address, uint32_t _register) {}

//-------------------------------------------------------------------------------------
void SimTarget::writeCode(uint32_t _address, uint32_t* _code, uint32_t _length) {
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
void SimTarget::readControllerData(
    uint32_t _address,
    uint32_t* _data,
    uint32_t _lineStart,
    uint32_t _lineStop,
    uint32_t _columnStart,
    uint32_t _columnStop) {}

//-------------------------------------------------------------------------------------
void SimTarget::writeControllerData(
    uint32_t _address,
    uint32_t* _data,
    uint32_t _lineStart,
    uint32_t _lineStop,
    uint32_t _columnStart,
    uint32_t _columnStop) {}

//-------------------------------------------------------------------------------------
void SimTarget::readMatrixArray(
    uint32_t _accMemStart,
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns,
    bool _accRequireResultReady) {}

//-------------------------------------------------------------------------------------
void SimTarget::writeMatrixArray(
    uint32_t _accMemStart,
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns) {}

//-------------------------------------------------------------------------------------
void SimTarget::dump(const std::string& _address) {}

//-------------------------------------------------------------------------------------
