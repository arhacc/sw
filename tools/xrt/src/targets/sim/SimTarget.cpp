//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//
//-------------------------------------------------------------------------------------
#include "targets/sim/SimTarget.h"
#include "targets/sim/XpuTestBench.cpp"
#include "targets/sim/Xsi_loader.cpp"

//-------------------------------------------------------------------------------------
SimTarget::SimTarget(){
	printf("Starting SimTarget...\n");

//BUG: terminate called after throwing an instance of 'ISIMK::DbgManagerException'
//disable loading of xsimk.so until the bug is fixed!!!
    return;

    try{
        std::string design_libname = "../build/xsim.dir/xsim/xsimk.so";

        xpuTestBench = new XpuTestBench(design_libname, "librdi_simulator_kernel.so", 1, "clock", "resetn");
        xpuTestBench -> tb_init();
        xpuTestBench -> do_reset(10);
        xpuTestBench -> wait_clock_cycles(30);
        xpuTestBench -> AXI_Lite_write(200,300);
        xpuTestBench -> AXI_Lite_write(400, 500);
        xpuTestBench -> run_ncycles(100);        
    }catch(std::exception _e){
        std::cout << "Could not load XSI simulation shared library!" << std::endl;
    }
}

//-------------------------------------------------------------------------------------
SimTarget::~SimTarget(){
}

//-------------------------------------------------------------------------------------
void SimTarget::init(){
}

//-------------------------------------------------------------------------------------
void SimTarget::quit() {
}

//-------------------------------------------------------------------------------------
void SimTarget::loadCode(uint32_t _address, uint32_t* _code, uint32_t _length){
	printf("SimTarget.loadCode @%d, length=%d\n", _address, _length);
}

//-------------------------------------------------------------------------------------
void SimTarget::loadData(uint32_t _address, uint32_t* _data, uint32_t _length){
}

//-------------------------------------------------------------------------------------
