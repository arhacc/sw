#include <xrt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint32_t example_matrix_in[] = { 1,  2,  3,  4,  5,
                                 6,  7,  8,  9, 10,
                                 11, 12, 13, 14, 15,
                                 16, 17, 18, 19, 20,
                                 21, 22, 23, 24, 25};

uint32_t example_matrix_out[25] = {0};

XRT_EXTERN
void test()
{
    writeMatrixArray(example_matrix_in, 5, 5, 1, 1, 3, 3, 0);

    readMatrixArray(0, 3, 3, 0, example_matrix_out, 5, 5, 1, 1);


    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%d ", example_matrix_out[i * 5 + j]);
        }

        printf("\n");
    }
}
