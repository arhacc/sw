#ifndef XRT_API_HEADER
#define XRT_API_HEADER

#ifdef __cplusplus
#define XRT_EXTERN extern "C"
#else
#define XRT_EXTERN
#endif

extern void  (*load)(const char *);
extern void *(*lowLevel)(const char *);
extern void  (*runRuntime)(void *);

#endif
