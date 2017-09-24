#ifndef CGL_LIB_H
#define CGL_LIB_H

#include "platform.h"

void* cgl_lib_open(const char* path);

void* cgl_lib_func(void* self, const char* name);

void cgl_lib_free(void* self);

const char* cgl_lib_error();

#endif
