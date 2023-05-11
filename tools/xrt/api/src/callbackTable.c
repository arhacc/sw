#include <stddef.h>

void  (*load)(const char *) = NULL;
void *(*lowLevel)(const char *) = NULL;
void  (*runRuntime)(void *) = NULL;
