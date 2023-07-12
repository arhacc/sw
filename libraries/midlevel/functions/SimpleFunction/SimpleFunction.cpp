//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#include <xrt.h>

#include <iostream>
#include <inttypes.h>
#include <stddef.h>

XRT_EXTERN
void xpu_simpleFunction(XRT_CONTEX_HANDLE _ctx) {
    uint32_t example_matrix_in[32];

    uint32_t example_matrix_out[32] = {0};

    uint32_t ram_matrix_total_lines = 2;
    uint32_t ram_matrix_total_cols = 16;
    uint32_t ram_start_pos_lines = 0;
    uint32_t ram_start_pos_cols = 0;
    uint32_t matrix_transfer_lines = 2;
    uint32_t matrix_transfer_cols = 16;
    uint32_t acc_address = 0;

    bool require_result_ready = true;

    for (int i = 0; i < 32; i++) {
        example_matrix_in[i] = i;
    }

    XRT_FUNCTION_HANDLE set_result_ready = xpu_lowLevel(_ctx, "prim_set_res_ready");

    xpu_writeMatrixArray(_ctx, acc_address, example_matrix_in,
        ram_matrix_total_lines, ram_matrix_total_cols,
        ram_start_pos_lines, ram_start_pos_cols,
        matrix_transfer_lines, matrix_transfer_cols);

    xpu_runRuntime(_ctx, set_result_ready, 0, NULL);

    xpu_readMatrixArray(_ctx, acc_address, example_matrix_out,
        ram_matrix_total_lines, ram_matrix_total_cols,
        ram_start_pos_lines, ram_start_pos_cols,
        matrix_transfer_lines, matrix_transfer_cols,
        require_result_ready);


    for (int i = 0; i < 32; i++) {
        printf("%" PRIu32 "\n", example_matrix_out[i]);
    }
}
