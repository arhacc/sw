#include <cstdint>
#include <xrt.h>
#include <xpu_functions.h>
#include <stddef.h>
#include <unistd.h>
#include <iostream>


int main()
{
    XRT_CONTEX_HANDLE ctx = xpu_init(true, false, false, NULL);

    xpu_load(ctx, "prim_initialize");
    xpu_load(ctx, "prim_set_addr_regs");
    xpu_load(ctx, "prim_fpga_test_0");
    xpu_load(ctx, "prim_fpga_test_01");

    XRT_FUNCTION_HANDLE prim_initialize = xpu_lowLevel(ctx, "prim_initialize");

    xpu_runRuntime(ctx, prim_initialize, 0, NULL);

    XRT_FUNCTION_HANDLE prim_set_addr_regs = xpu_lowLevel(ctx, "prim_set_addr_regs");
    uint32_t argv_set_addr_regs[2] = {0, 0};
    xpu_runRuntime(ctx, prim_set_addr_regs, 2, argv_set_addr_regs);

    XRT_FUNCTION_HANDLE prim_fpga_test_0 = xpu_lowLevel(ctx, "prim_fpga_test_0");
    xpu_runRuntime(ctx, prim_fpga_test_0, 0, NULL);

    XRT_FUNCTION_HANDLE prim_fpga_test_01 = xpu_lowLevel(ctx, "prim_fpga_test_01");
    xpu_runRuntime(ctx, prim_fpga_test_01, 0, NULL);

    for (int i = 0; i < 20; i++) {
        std::cout << std::hex << xpu_readRegister(ctx, i * 4) << std::endl;
    }

    xpu_close(ctx);
}
