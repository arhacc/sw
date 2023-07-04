//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include "targets/sim/SimTarget.h"
#include <cstdint>

//-------------------------------------------------------------------------------------
SimTarget::SimTarget() {
    printf("Starting SimTarget...\n");

    //BUG: terminate called after throwing an instance of 'ISIMK::DbgManagerException'
    //disable loading of xsimk.so until the bug is fixed!!!
    return;

    try {
        std::string design_libname = "../build/xsim.dir/xsim/xsimk.so";

        xpuTestBench = new XpuTestBench(design_libname, "librdi_simulator_kernel.so", 1, "clock", "resetn");
        xpuTestBench->tb_init();
        xpuTestBench->do_reset(10);
        xpuTestBench->wait_clock_cycles(30);
        xpuTestBench->AXI_Lite_write(200, 300);
        xpuTestBench->AXI_Lite_write(400, 500);
        xpuTestBench->run_ncycles(100);
    } catch (std::exception &_e) {
        std::cout << "Could not load XSI simulation shared library!" << std::endl;
    }
}

//-------------------------------------------------------------------------------------
void SimTarget::reset() {
}

//-------------------------------------------------------------------------------------
void SimTarget::runRuntime(uint32_t _address, uint32_t _argc, uint32_t *_args) {
}

//-------------------------------------------------------------------------------------
void SimTarget::runDebug(uint32_t _address, uint32_t *_args, uint32_t _breakpointAddress) {

}

//-------------------------------------------------------------------------------------
uint32_t SimTarget::readRegister(uint32_t _address) {
    return 0;
}

//-------------------------------------------------------------------------------------
void SimTarget::writeRegister(uint32_t _address, uint32_t _register) {

}

//-------------------------------------------------------------------------------------
void SimTarget::writeCode(uint32_t _address, uint32_t *_code, uint32_t _length) {
    printf("SimTarget.loadCode @%d, length=%d\n", _address, _length);
}

//-------------------------------------------------------------------------------------
void SimTarget::readControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
}

//-------------------------------------------------------------------------------------
void SimTarget::writeControllerData(uint32_t _address, uint32_t *_data, uint32_t _lineStart, uint32_t _lineStop,
        uint32_t _columnStart, uint32_t _columnStop) {
}

//-------------------------------------------------------------------------------------
void SimTarget::getMatrixArray(uint32_t _accAddress, uint32_t _rawRamAddress, uint32_t _numLines, uint32_t _numColumns, bool _waitResult) {
}

//-------------------------------------------------------------------------------------
void SimTarget::sendMatrixArray(uint32_t _rawRamAddress, uint32_t _accAddress, uint32_t _numLines, uint32_t _numColumns) {
}

//-------------------------------------------------------------------------------------
void SimTarget::dump(const std::string &_address) {

}

//-------------------------------------------------------------------------------------
