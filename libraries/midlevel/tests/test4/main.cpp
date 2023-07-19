#include <cstdint>
#include <xrt.h>
#include <xpu_functions.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cinttypes>

int main(int argc, char **argv)
{
    XRT_CONTEX_HANDLE ctx = xpu_init(true, false, false);

    XRT_FUNCTION_HANDLE prim_initialize = xpu_lowLevel(ctx, "prim_initialize");
    XRT_FUNCTION_HANDLE prim_set_addr_regs = xpu_lowLevel(ctx, "prim_set_addr_regs");
    XRT_FUNCTION_HANDLE wait_matrix = xpu_lowLevel(ctx, "prim_wait_matrices");
    XRT_FUNCTION_HANDLE set_result_ready = xpu_lowLevel(ctx, "prim_set_res_ready");
    XRT_FUNCTION_HANDLE set_interrupt = xpu_lowLevel(ctx, "prim_set_interrupt");
    XRT_FUNCTION_HANDLE prim_fpga_test_3 = xpu_lowLevel(ctx, "prim_fpga_test_3");

    uint32_t addr = 100;

    if (argc > 1) {
        addr = atoi(argv[1]);
    }

    xpu_runRuntime(ctx, prim_initialize, 0, NULL);
    uint32_t argv_set_addr_regs[2] = {0, 0};
    xpu_runRuntime(ctx, prim_set_addr_regs, 2, argv_set_addr_regs);

    uint32_t matrix_in[4 * 16];
    for (int i = 0; i < 4 * 16; i++) {
        matrix_in[i] = i * 100;
    }

    xpu_writeMatrixArray(ctx, addr, matrix_in, 4, 16,
                         0, 0, 4, 16);
    
    uint32_t arg_wait_matrix = 1;
    xpu_runRuntime(ctx, wait_matrix, 1, &arg_wait_matrix);


    argv_set_addr_regs[0] = argv_set_addr_regs[1] = addr;
    xpu_runRuntime(ctx, prim_set_addr_regs, 2, argv_set_addr_regs);
    xpu_runRuntime(ctx, prim_fpga_test_3, 0, NULL);

    argv_set_addr_regs[0] = argv_set_addr_regs[1] = addr + 1;
    xpu_runRuntime(ctx, prim_set_addr_regs, 2, argv_set_addr_regs);
    xpu_runRuntime(ctx, prim_fpga_test_3, 0, NULL);

    argv_set_addr_regs[0] = argv_set_addr_regs[1] = addr + 2;
    xpu_runRuntime(ctx, prim_set_addr_regs, 2, argv_set_addr_regs);
    xpu_runRuntime(ctx, prim_fpga_test_3, 0, NULL);

    xpu_runRuntime(ctx, set_result_ready, 0, NULL);

    uint32_t matrix_out[4 * 16];
    xpu_readMatrixArray(ctx, addr, matrix_out, 4, 16,
                        0, 0, 4, 16, true);

    bool ok = true;

    for (int i = 0; i < 4 * 16; i++) {
        printf("%" PRIu32 "", matrix_out[i]);
        if (i % 16 == 15) {
            printf("\n");
        } else {
            printf(" ");
        }

        if ((i < 3 * 16 && matrix_out[i] != matrix_in[i] + (i % 16))
            || (i >= 3 * 16 && matrix_out[i] != matrix_in[i])) {
            fprintf(stderr, "Error at address %" PRIu32 ", cell %" PRIu32 ": %" PRIu32 " != %" PRIu32 "\n", addr, i, matrix_out[i], matrix_in[i] + i * 2);

            ok = false;
        }
    }

    xpu_runRuntime(ctx, set_interrupt, 0, NULL);
    printf("Status reg: %d\n", xpu_readRegister(ctx, 0x10));

    xpu_close(ctx);

    return ok ? 0 : 1;
}
