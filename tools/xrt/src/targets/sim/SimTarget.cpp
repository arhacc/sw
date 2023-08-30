//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/CodeGen.h>
#include <targets/sim/SimTarget.h>
#include <targets/sim/Simulator.h>

#include <cassert>
#include <cinttypes>
#include <cstdint>
#include <vector>

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
uint32_t SimTarget::readRegister(uint32_t _address) {
    return 0;
}

//-------------------------------------------------------------------------------------
void SimTarget::writeRegister(uint32_t _address, uint32_t _register) {}

//-------------------------------------------------------------------------------------
void SimTarget::getMatrixArray(
    uint32_t* _ramMatrix,
    uint32_t _ramTotalLines,
    uint32_t _ramTotalColumns,
    uint32_t _ramStartLine,
    uint32_t _ramStartColumn,
    uint32_t _numLines,
    uint32_t _numColumns) {
    fmt::println("SimTarget: Getting matrix array");

    Simulator _simulator(
        "./xsim.dir/simulator_axi/xsimk.so", "clock", "resetn", programFile, dataFile);

    _simulator.run();

    std::vector<unsigned int> _matrix = _simulator.getMatrix();

    auto _matrixIt = _matrix.begin() + skipGetMatrix;

    for (uint32_t _i = _ramStartLine; _i < _ramStartLine + _numLines; ++_i) {
        for (uint32_t _j = _ramStartColumn; _j < _ramStartColumn + _numColumns; ++_j) {
            _ramMatrix[_i * _ramTotalColumns + _j] = (*_matrixIt++);
            ++skipGetMatrix;
        }
    }

    assert(_matrixIt == _matrix.end());
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
    for (uint32_t _i = 0; _i < _numLines; ++_i) {
        for (uint32_t _j = 0; _j < _numColumns; ++_j) {
            uint32_t _index =
                (_ramStartLine + _i) * _ramTotalColumns + _ramStartColumn + _j;

            dataFile.push_back(_ramMatrix[_index]);
        }
    }
}

//-------------------------------------------------------------------------------------
