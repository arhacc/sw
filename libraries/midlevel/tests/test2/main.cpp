
#include <cstdint>
#include <xrt.h>
#include <xpu_functions.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

int main()
{
    void *ctx = xpu_init(true, false, false);

    xpu_load(ctx, "prim_initialize");
    xpu_load(ctx, "prim_set_addr_regs");
    xpu_load(ctx, "prim_wait_matrices");
    xpu_load(ctx, "prim_set_res_ready");
    xpu_load(ctx, "prim_set_interrupt");

    void *prim_initialize = xpu_lowLevel(ctx, "prim_initialize");
    void *prim_set_addr_regs = xpu_lowLevel(ctx, "prim_set_addr_regs");
    void *wait_matrix = xpu_lowLevel(ctx, "prim_wait_matrices");
    void *set_result_ready = xpu_lowLevel(ctx, "prim_set_res_ready");
    void *set_interrupt = xpu_lowLevel(ctx, "prim_set_interrupt");

    xpu_runRuntime(ctx, prim_initialize, 0, NULL);
    
    uint32_t argv_set_addr_regs[2] = {0, 0};
    xpu_runRuntime(ctx, prim_set_addr_regs, 2, argv_set_addr_regs);

    uint32_t matrix_in[4 * 16];
    for (int i = 0; i < 4 * 16; i++) {
        matrix_in[i] = i * 10;
    }

    xpu_writeMatrixArray(ctx, 100, matrix_in, 4, 16,
                         0, 0, 1, 16);

    xpu_writeMatrixArray(ctx, 101, matrix_in, 4, 16,
                         1, 0, 1, 16);

    xpu_writeMatrixArray(ctx, 102, matrix_in, 4, 16,
                         2, 0, 1, 16);
    
    xpu_writeMatrixArray(ctx, 103, matrix_in, 4, 16,
                         3, 0, 1, 16);
    

    uint32_t arg_wait_matrix = 4;
    xpu_runRuntime(ctx, wait_matrix, 1, &arg_wait_matrix);

    xpu_runRuntime(ctx, set_result_ready, 0, NULL);

    uint32_t matrix_out[4 * 16];
    xpu_readMatrixArray(ctx, 100, matrix_out, 4, 16,
                        0, 0, 4, 16, true);


    sleep(10);

    for (int i = 0; i < 4 * 16; i++) {
        printf("%d\n", matrix_out[i]);
    }


    xpu_runRuntime(ctx, set_interrupt, 0, NULL);
    sleep(1);
    printf("Status reg: %d\n", xpu_readRegister(ctx, 0x10));

    exit(0);
}
