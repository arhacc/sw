#include <xrt.h>

XRT_EXTERN
void test()
{
    load("/home/grffn/work/xpu/example_program.hex");

    void *function = lowLevel("example_program");

    runRuntime(function);
}
