#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "platform.h"
#include "cg_lib.h"
#include "cg_so.h"
#include "cg_io.h"

static void* cuDevice = 0;
static void* cuContext = 0;
static void* cuModule = 0;
static void* cuFunc = 0;

int (*cuInit)(unsigned int Flags) = 0;
int (*cuDeviceGetCount)(int* count);
int (*cuDeviceGet)(void* device, int ordinal);
int (*cuCtxCreate)(void *pctx, unsigned int flags, void* dev);
int (*cuModuleLoad)(void* module, const char *fname);
int (*cuModuleUnload)(void* hmod);
int (*cuModuleGetFunction)(void* hfunc, void* hmod, const char *name);
int (*cuCtxDetach)(void* ctx);
int (*cuMemAlloc)(void* *dptr, size_t bytesize);
int (*cuMemFree)(void* dptr);
int (*cuMemcpyHtoD)(void* dstDevice, const void *srcHost, size_t ByteCount);
int (*cuMemcpyDtoH)(void *dstHost, void* srcDevice, size_t ByteCount);
int (*cuLaunchKernel)(void* f, unsigned int gridDimX, unsigned int gridDimY, unsigned int gridDimZ, unsigned int blockDimX, unsigned int blockDimY, unsigned int blockDimZ, unsigned int sharedMemBytes,
		void* hStream, void **kernelParams, void **extra);
int (*cuMemcpyDtoH)(void *dstHost, void* srcDevice, size_t ByteCount);

err cgl_gpu_init() {
	static err init = 0;
	switch (init) {
	case 0: {
        void* library = cgl_lib_open("/usr/local/cuda/lib/libcuda.dylib");
        if (!library) {
            return init = 2;
        }
        cuInit = cgl_lib_func(library, "cuInit");
        cuDeviceGetCount = cgl_lib_func(library, "cuDeviceGetCount");
        cuDeviceGet = cgl_lib_func(library, "cuDeviceGet");
        cuCtxCreate = cgl_lib_func(library, "cuCtxCreate_v2");
        cuModuleLoad = cgl_lib_func(library, "cuModuleLoad");
        cuModuleUnload = cgl_lib_func(library, "cuModuleUnload");
        cuModuleGetFunction = cgl_lib_func(library, "cuModuleGetFunction");
        cuCtxDetach = cgl_lib_func(library, "cuCtxDetach");
        cuMemAlloc = cgl_lib_func(library, "cuMemAlloc_v2");
        cuMemFree = cgl_lib_func(library, "cuMemFree_v2");
        cuMemcpyHtoD = cgl_lib_func(library, "cuMemcpyHtoD_v2");
        cuLaunchKernel = cgl_lib_func(library, "cuLaunchKernel");
        cuMemcpyDtoH = cgl_lib_func(library, "cuMemcpyDtoH_v2");
        cgl_lib_free(library);
        if (!cuInit || !cuDeviceGetCount || !cuDeviceGet || !cuCtxCreate || !cuModuleLoad) {
            return init = 2;
        }
        if (!cuModuleGetFunction || !cuCtxDetach || !cuMemAlloc || !cuMemcpyHtoD || !cuLaunchKernel) {
            return init = 2;
        }
        if (!cuMemcpyDtoH|| !cuModuleUnload|| !cuMemFree) {
            return init = 2;
        }
		if (cuInit(0)) {
			return init = 2;
		}
		int deviceCount = 0;
		if (cuDeviceGetCount(&deviceCount)) {
			return init = 2;
		}
		if (deviceCount == 0) {
			return init = 2;
		}
		int devId = 0;
		if (cuDeviceGet(&cuDevice, devId)) {
			return init = 2;
		}
		if (cuCtxCreate(&cuContext, 0, cuDevice)) {
			return init = 2;
		}
		init = 1;
		break;
	}
	case 2:
		return 1;
	}
	return 0;
}

void cgl_gpu_cuda_free() {
    if (cuCtxDetach && cuCtxDetach(cuContext)) {
        printf("fail");
    }
}

byte cgl_gpu_cuda_has() {
	if (cgl_gpu_init()) {
		return 0;
	}
	return 1;
}

void* cgl_gpu_func_compile(const char* code, const char* funcname) {
    if (!cgl_gpu_cuda_has()) {
        return 0;
    }
    char* path = cgl_io_tmp((const unsigned char*) code, -1);
	if (!path) {
		return 0;
	}
	free(path);
	if (cgl_so_process_execute("/usr/local/cuda/bin/nvcc -ptx /tmp/cg.cu -o /tmp/cg.ptx")) {
        cgl_so_process_execute("rm /tmp/cg.*");
        return 0;
	}
    if (cuModuleLoad(&cuModule, "/tmp/cg.ptx")) {
        cgl_so_process_execute("rm /tmp/cg.*");
        return 0;
    }
    if (cuModuleGetFunction(&cuFunc, cuModule, funcname)) {
        cuModuleUnload(cuModule);
        cgl_so_process_execute("rm /tmp/cg.*");
        return 0;
    }
    return cuFunc;
}

void* cgl_gpu_malloc(size_t size) {
    if (!cgl_gpu_cuda_has()) {
        return 0;
    }
    void* data;
    if (cuMemAlloc(&data, size)) {
        free(data);
        return 0;
    }
    return data;
}

void cgl_gpu_free(void* data) {
    if (cgl_gpu_cuda_has()) {
        cuMemFree(data);
    }
}

err cgl_gpu_memcpyhd(void* dest, void* src, size_t size) {
    if (!cgl_gpu_cuda_has()) {
        return 1;
    }
    if (cuMemcpyHtoD(dest, src, size)) {
        return 1;
    }
    return 0;
}

err cgl_gpu_memcpydh(void* dest, void* src, size_t size) {
    if (!cgl_gpu_cuda_has()) {
        return 1;
    }
    if (cuMemcpyDtoH(dest, src, size)) {
        return 1;
    }
    return 0;
}

err cgl_gpu_func_execute(void* function, uint32 gridDimX, uint32 gridDimY, uint32 gridDimZ, uint32 blockDimX, uint32 blockDimY, uint32 blockDimZ, uint32 sharedMemBytes, void* hStream, void **kernelParams, void **extra) {
    return cuLaunchKernel(function, gridDimX, gridDimY, gridDimZ, blockDimX, blockDimY, blockDimZ, sharedMemBytes, hStream, kernelParams, extra) == 0 ? 0 : 1;
}
