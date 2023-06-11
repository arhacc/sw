#include <xrt.h>
#include <xpu_functions.h>

int main()
{
    void *ctx = xpu_init(true, false, false);
    xpu_simpleFunction(ctx);
}
