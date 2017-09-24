#ifndef CGL_CUDA_H
#define CGL_CUDA_H

#include "platform.h"

byte cgl_gpu_cuda_has();

void* cgl_gpu_func_compile(const char* code, const char* funcname);

void* cgl_gpu_malloc(size_t size);

void cgl_gpu_free(void* data);

err cgl_gpu_memcpyhd(void* dest, void* src, size_t size);

err cgl_gpu_memcpydh(void* dest, void* src, size_t size);

err cgl_gpu_func_execute(void* function, uint32 gridDimX, uint32 gridDimY, uint32 gridDimZ, uint32 blockDimX, uint32 blockDimY, uint32 blockDimZ, uint32 sharedMemBytes, void* hStream, void **kernelParams, void **extra);

int cgl_gpu_cuda_test();

#endif
