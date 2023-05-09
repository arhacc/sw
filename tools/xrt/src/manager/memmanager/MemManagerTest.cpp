#if 0

#include "manager/libmanager/FunctionInfo.hpp"
#include <manager/memmanager/MemManager.h>

int main()
{
    MemManager memManager(nullptr);

    FunctionInfo f1 {
        .length = 511,
        .name = "functie",
        .address = 0,
        .code = 0,
    };
    FunctionInfo f2 {
        .length = 513,
        .name = "functie1",
        .address = 0,
        .code = 0,
    };
    FunctionInfo f3 {
        .length = 600,
        .name = "functie2",
        .address = 0,
        .code = 0,
    };

    memManager.dump();

    printf("--------------\n");

    memManager.loadFunction(f1);
    memManager.loadFunction(f2);

    printf("--------------\n");

    memManager.dump();

    memManager.loadFunction(f3);

    printf("--------------\n");

    memManager.dump();
}

#endif
