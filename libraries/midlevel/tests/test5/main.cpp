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
    XRT_FUNCTION_HANDLE mat_mat_add_hadamard_square = xpu_lowLevel(ctx, "prim_mat_mat_add_hadamard_square");


    uint32_t matrix_in_0[32 * 32];
    uint32_t matrix_in_1[32 * 32];
    uint32_t matrix_out[32 * 32] = {0};

    for (size_t i = 0; i < 32 * 32; i++) {
        matrix_in_0[i] = i;
        matrix_in_1[i] = i * 10'000;
    }

    xpu_runRuntime(ctx, prim_initialize, 0, NULL);

    {
        uint32_t argv_set_addr_regs[2] = {0, 0};
        xpu_runRuntime(ctx, prim_set_addr_regs, 2, argv_set_addr_regs);

        xpu_writeMatrixArray(ctx, 0, matrix_in_0,
            32, 32,
            0, 0,
            16, 16);
        
        xpu_writeMatrixArray(ctx, 16, matrix_in_1,
            32, 32,
            0, 0,
            16, 16);

        uint32_t arg_wait_matrix = 2;
        xpu_runRuntime(ctx, wait_matrix, 1, &arg_wait_matrix);

        uint32_t argv_add_hadamard_square[3] = {0, 16, 64};
        xpu_runRuntime(ctx, mat_mat_add_hadamard_square, 3, argv_add_hadamard_square);

        xpu_runRuntime(ctx, set_result_ready, 0, NULL);

        xpu_readMatrixArray(ctx, 64, matrix_out,
            32, 32,
            0, 0,
            16, 16, true);
    }
    
    {
        uint32_t argv_set_addr_regs[2] = {512, 512};
        xpu_runRuntime(ctx, prim_set_addr_regs, 2, argv_set_addr_regs);

        xpu_writeMatrixArray(ctx, 512, matrix_in_0,
            32, 32,
            0, 16,
            16, 16);
        
        xpu_writeMatrixArray(ctx, 528, matrix_in_1,
            32, 32,
            0, 16,
            16, 16);

        uint32_t arg_wait_matrix = 2;
        xpu_runRuntime(ctx, wait_matrix, 1, &arg_wait_matrix);

        uint32_t argv_add_hadamard_square[3] = {0, 16, 64};
        xpu_runRuntime(ctx, mat_mat_add_hadamard_square, 3, argv_add_hadamard_square);

        xpu_runRuntime(ctx, set_result_ready, 0, NULL);

        xpu_readMatrixArray(ctx, 576, matrix_out,
            32, 32,
            0, 16,
            16, 16, true);
    }

    {
        uint32_t argv_set_addr_regs[2] = {0, 0};
        xpu_runRuntime(ctx, prim_set_addr_regs, 2, argv_set_addr_regs);

        xpu_writeMatrixArray(ctx, 0, matrix_in_0,
            32, 32,
            16, 0,
            16, 16);
        
        xpu_writeMatrixArray(ctx, 16, matrix_in_1,
            32, 32,
            16, 0,
            16, 16);

        uint32_t arg_wait_matrix = 2;
        xpu_runRuntime(ctx, wait_matrix, 1, &arg_wait_matrix);

        uint32_t argv_add_hadamard_square[3] = {0, 16, 64};
        xpu_runRuntime(ctx, mat_mat_add_hadamard_square, 3, argv_add_hadamard_square);

        xpu_runRuntime(ctx, set_result_ready, 0, NULL);

        xpu_readMatrixArray(ctx, 64, matrix_out,
            32, 32,
            16, 0,
            16, 16, true);
    }

    {
        uint32_t argv_set_addr_regs[2] = {512, 512};
        xpu_runRuntime(ctx, prim_set_addr_regs, 2, argv_set_addr_regs);

        xpu_writeMatrixArray(ctx, 512, matrix_in_0,
            32, 32,
            16, 16,
            16, 16);
        
        xpu_writeMatrixArray(ctx, 528, matrix_in_1,
            32, 32,
            16, 16,
            16, 16);

        uint32_t arg_wait_matrix = 2;
        xpu_runRuntime(ctx, wait_matrix, 1, &arg_wait_matrix);

        uint32_t argv_add_hadamard_square[3] = {0, 16, 64};
        xpu_runRuntime(ctx, mat_mat_add_hadamard_square, 3, argv_add_hadamard_square);

        xpu_runRuntime(ctx, set_result_ready, 0, NULL);

        xpu_readMatrixArray(ctx, 576, matrix_out,
            32, 32,
            16, 16,
            16, 16, true);
    }

    xpu_runRuntime(ctx, set_interrupt, 0, NULL);
    printf("Status reg: %d\n", xpu_readRegister(ctx, 0x10));

    bool ok = true;

    for (size_t i = 0; i < 32 * 32; i++) {
        printf("%d ", matrix_out[i]);
        if (i % 32 == 31) {
            printf("\n");
        }
    }

    for (size_t i = 0; i < 32 * 32; i++) {
        if (matrix_out[i] != matrix_in_0[i] + matrix_in_1[i]) {
            ok = false;
            printf("\nError at line %zd, column %zd, %d != %d\n", i / 32, i % 32, matrix_out[i], matrix_in_0[i] + matrix_in_1[i]);
        }
    }

    xpu_close(ctx);

    return ok ? 0 : 1;
}
