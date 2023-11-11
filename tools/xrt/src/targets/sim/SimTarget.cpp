//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include <common/CodeGen.h>
#include <targets/sim/SimTarget.h>

#include <cassert>
#include <cinttypes>
#include <cstddef>
#include <cstdint>
#include <vector>

#include <fmt/core.h>

//-------------------------------------------------------------------------------------
SimTarget::SimTarget(const Arch& _arch) : arch(_arch) {
    fmt::println("Starting SimTarget...");
    tb = new Tb(
        "./xsim.dir/simulator_axi/xsimk.so",
        "librdi_simulator_kernel.so",
        1,
        "clock",
        "resetn",
        _arch);
    //    simulator->run();

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
/*
    std::cout << "[MAIN] Current root: " << std::filesystem::current_path() << std::endl;
    tb->generateClock(tb->getHalfClockPeriod());

    std::cout << "Start time in ns: " << tb->getTime() << '\n';
    std::cout << "Clock with half-period of " << std::dec << tb->getHalfClockPeriod()
              << " simulation steps\n";


    std::cout << "\nEnd time: " << std::dec << tb->getTime() << " ns" << std::endl;
}

*/
//-------------------------------------------------------------------------------------
