#include <xrt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t example_matrix_in[64];

uint32_t example_matrix_out[64] = {0};

uint32_t ram_matrix_total_lines = 1;
uint32_t ram_matrix_total_cols = 64;
uint32_t ram_start_pos_lines = 0;
uint32_t ram_start_pos_cols = 0;
uint32_t matrix_transfer_lines = 1;
uint32_t matrix_transfer_cols = 64;
uint32_t acc_address = 0;

int require_result_ready = 0;

XRT_EXTERN
void test()
{
    for (int i = 0; i < 64; i++) {
        example_matrix_in[i] = i;
    }

    writeMatrixArray(example_matrix_in,
		     ram_matrix_total_lines, ram_matrix_total_cols,
		     ram_start_pos_lines, ram_start_pos_cols,
		     matrix_transfer_lines, matrix_transfer_cols,
		     acc_address);

    readMatrixArray(acc_address,
		    matrix_transfer_lines, matrix_transfer_cols,
		    require_result_ready,
		    example_matrix_out,
		    ram_matrix_total_lines, ram_matrix_total_cols,
		    ram_start_pos_lines, ram_start_pos_cols);

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

    load("some_library");

    void *someFunction = lowLevel("some_function");

    uint32_t args[] = { 1, 2, 3 };
 
    if (someFunction != NULL) {
        runRuntime(someFunction, 3, args);
        runRuntime(someFunction, 0, NULL);
    }
}
