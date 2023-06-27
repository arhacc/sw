//-------------------------------------------------------------------------------------
//
//                             The XRT Project
//
// See LICENSE.TXT for details.
//-------------------------------------------------------------------------------------

#include <xrt.h>

#include <iostream>



XRT_EXTERN
void xpu_simpleFunction(void *_ctx) {
    uint32_t example_matrix_in[128];

    uint32_t example_matrix_out[128] = {0};

    uint32_t ram_matrix_total_lines = 2;
    uint32_t ram_matrix_total_cols = 64;
    uint32_t ram_start_pos_lines = 0;
    uint32_t ram_start_pos_cols = 0;
    uint32_t matrix_transfer_lines = 1;
    uint32_t matrix_transfer_cols = 9;
    uint32_t acc_address = 0;



    bool require_result_ready = 1;

    for (int i = 0; i < 128; i++) {
        example_matrix_in[i] = i;
    }

    xpu_writeMatrixArray(_ctx, acc_address, example_matrix_in, ram_matrix_total_lines, ram_matrix_total_cols, ram_start_pos_lines, ram_start_pos_cols, matrix_transfer_lines, matrix_transfer_cols);

    xpu_readMatrixArray(_ctx, acc_address, example_matrix_out, ram_matrix_total_lines, ram_matrix_total_cols, ram_start_pos_lines, ram_start_pos_cols, matrix_transfer_lines, matrix_transfer_cols, require_result_ready);


    /*  example_matrix_out =
     *
     *  0   0  0  0  0
     *  0   7  8  9  0
     *  0  12 13 14  0
     *  0  17 18 19  0
     *  0   0  0  0  0
     */

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%d ", example_matrix_out[i * 5 + j]);
        }

        printf("\n");
    }

    xpu_load(_ctx, "prim_delay_5");

    void *someFunction = xpu_lowLevel(_ctx, "prim_delay_5");
 
    if (someFunction != NULL) {
        xpu_runRuntime(_ctx, someFunction, 0, NULL);
    }
}
