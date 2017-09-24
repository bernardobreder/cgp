#ifndef CGL_LINKER_H
#define CGL_LINKER_H

#include "treemap.h"

struct cgl_linker_t {
    struct b_treemap_t* classes;
};

struct cgl_linker_class_t {
    unsigned char* name;
	struct b_treemap_t* fields;
	struct b_treemap_t* methods;
	byte* binary;
	byte* opcodes;
};

struct cgl_linker_method_t {
    unsigned char* name;
};

struct cgl_linker_field_t {
    unsigned char* name;
};

struct cgl_linker_t* cgl_linker_new();

void cgl_linker_free(struct cgl_linker_t* self);

void cgl_linker_class_free(struct cgl_linker_class_t* self);

void cgl_linker_field_free(struct cgl_linker_field_t* self);

void cgl_linker_method_free(struct cgl_linker_method_t* self);

unsigned char cgl_linker_add(struct cgl_linker_t* self, unsigned char* bytes);

unsigned char* cgl_linker_compile(struct cgl_linker_t* self, uint36* size);

void cgl_linker_test();

#endif
