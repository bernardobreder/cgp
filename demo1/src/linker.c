#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "platform.h"
#include "cg_io.h"
#include "node.h"
#include "compile.h"
#include "linker.h"
#include "treemap.h"

struct cgl_linker_t* cgl_linker_new() {
	struct cgl_linker_t* self = (struct cgl_linker_t*) calloc(1, sizeof(struct cgl_linker_t));
	if (!self) {
		return 0;
	}
	struct b_treemap_t* classes = self->classes = b_treemap_new((int (*)(void*, void*)) strcmp, 0, (void (*)(void*)) cgl_linker_class_free);
	if (!classes) {
		free(self);
		return 0;
	}
	return self;
}

void cgl_linker_free(struct cgl_linker_t* self) {
	b_treemap_free(self->classes);
	free(self);
}

void cgl_linker_class_free(struct cgl_linker_class_t* self) {
	free(self->binary);
	b_treemap_free(self->fields);
	b_treemap_free(self->methods);
	free(self->name);
	free(self);
}

void cgl_linker_field_free(struct cgl_linker_field_t* self) {
	free(self->name);
	free(self);
}

void cgl_linker_method_free(struct cgl_linker_method_t* self) {
	free(self->name);
	free(self);
}

unsigned char cgl_linker_add(struct cgl_linker_t* self, unsigned char* binary) {
	unsigned char* bytes = (unsigned char*) binary;
	uint32 n;
	if (*(bytes++) != 0xBB) {
		return 1;
	}
	struct cgl_linker_class_t* clazz;
	unsigned char* classname;
	{
		int32 size = cgl_io_uint_read(bytes);
		bytes += 4;
		classname = cgl_io_chars_read(bytes, size);
		if (!classname) {
			return 0;
		}
		bytes += size;
		if (b_treemap_get(self->classes, classname)) {
			free(classname);
			return 0;
		}
		clazz = (struct cgl_linker_class_t*) calloc(1, sizeof(struct cgl_linker_class_t));
		if (!clazz) {
			free(classname);
			return 0;
		}
		clazz->name = classname;
		struct b_treemap_t* fields = clazz->fields = b_treemap_new((int (*)(void*, void*)) strcmp, 0, (void (*)(void*)) cgl_linker_field_free);
		if (!fields) {
			free(clazz);
			free(classname);
			return 0;
		}
		struct b_treemap_t* methods = clazz->methods = b_treemap_new((int (*)(void*, void*)) strcmp, 0, (void (*)(void*)) cgl_linker_method_free);
		if (!fields) {
			b_treemap_free(fields);
			free(clazz);
			free(classname);
			return 0;
		}
		if (b_treemap_put(self->classes, classname, clazz)) {
			b_treemap_free(methods);
			b_treemap_free(fields);
			free(clazz);
			free(classname);
			return 0;
		}
	}
	//	uint32 field_count = cgl_io_uint_read(bytes);
	bytes += 4;
	uint32 method_count = cgl_io_uint_read(bytes);
	bytes += 4;
	for (n = 0; n < method_count; n++) {
		int32 size = cgl_io_uint_read(bytes);
		bytes += 4;
		unsigned char* methodname = cgl_io_chars_read(bytes, size);
		if (!methodname) {
			return 0;
		}
		struct cgl_linker_method_t* method = (struct cgl_linker_method_t*) calloc(1, sizeof(struct cgl_linker_method_t));
		if (!method) {
			free(methodname);
			return 0;
		}
		method->name = methodname;
		if (b_treemap_put(clazz->methods, methodname, method)) {
			free(method);
			free(methodname);
			return 0;
		}
		bytes += size;
	}
	clazz->binary = binary;
	clazz->opcodes = bytes;
	return 0;
}

unsigned char* cgl_linker_compile(struct cgl_linker_t* self, uint36* size) {
	int n;
	int class_count = b_treemap_size(self->classes);
	struct cgl_linker_class_t** classes = (struct cgl_linker_class_t**) malloc(class_count * sizeof(struct cgl_linker_class_t*));
	if (!classes) {
		return 0;
	}
	struct b_treemap_entry_t* entry = b_treemap_first(self->classes);
	for (n = 0; n < class_count; n++) {
		classes[n] = (struct cgl_linker_class_t*) entry->value;
		entry = b_treemap_successor(entry);
	}
	uint32 data_size = 0;
	for (n = 0; n < class_count; n++) {
		struct cgl_linker_class_t* clazz = classes[n];
		byte* opcodes = clazz->opcodes;
		byte c = *opcodes;
		for (; c != 0xFF;) {
			switch (c) {
			case CGL_VM_OPCODE_STACK_PROLOG:
				data_size++;
				break;
			case CGL_VM_OPCODE_STACK_LEAVE:
				data_size++;
				break;
			case CGL_VM_OPCODE_STACK_RET:
				data_size++;
				break;
			case CGL_VM_OPCODE_STACK_PUSH_S:
				opcodes += 4;
				data_size += 5;
				break;
			case CGL_VM_OPCODE_STACK_POP_S_INT:
				opcodes += 4;
				data_size += 5;
				break;
			case CGL_VM_OPCODE_STACK_PUSH_A:
				data_size++;
				break;
			case CGL_VM_OPCODE_STACK_POP_A:
				data_size++;
				break;
			case CGL_VM_OPCODE_STACK_PUSH_B:
				data_size++;
				break;
			case CGL_VM_OPCODE_STACK_POP_B:
				data_size++;
				break;
			case CGL_VM_OPCODE_STACK_INT:
				opcodes += 4;
				data_size += 5;
				break;
			case CGL_VM_OPCODE_INT_LOAD_A:
				opcodes += 4;
				data_size += 5;
				break;
			case CGL_VM_OPCODE_INT_LOAD_C:
				opcodes += 4;
				data_size += 5;
				break;
			case CGL_VM_OPCODE_INT_SUM_AC:
				data_size++;
				break;
			case CGL_VM_OPCODE_INT_MOV_AC:
				data_size++;
				break;
			case CGL_VM_OPCODE_INT_SUB_AC:
				data_size++;
				break;
			case CGL_VM_OPCODE_INT_MUL_AC:
				data_size++;
				break;
			case CGL_VM_OPCODE_INT_DIV_AC:
				data_size++;
				break;
			case CGL_VM_OPCODE_INT_EQ:
				data_size++;
				break;
			case CGL_VM_OPCODE_INT_NE:
				data_size++;
				break;
			default:
				free(classes);
				return 0;
			}
			c = *(++opcodes);
		}
	}
	byte* data = (byte*) malloc(data_size + 1);
	if (!data) {
		free(classes);
		return 0;
	}
	byte* data_next = data;
	for (n = 0; n < class_count; n++) {
		struct cgl_linker_class_t* clazz = classes[n];
		byte* opcodes = clazz->opcodes;
		byte opcode = *(opcodes++);
		for (; opcode != 0xFF;) {
			switch (opcode) {
			case CGL_VM_OPCODE_STACK_PROLOG:
				*(data_next++) = opcode;
				break;
			case CGL_VM_OPCODE_STACK_LEAVE:
				*(data_next++) = opcode;
				break;
			case CGL_VM_OPCODE_STACK_RET:
				*(data_next++) = opcode;
				break;
			case CGL_VM_OPCODE_STACK_PUSH_S:
				*(data_next++) = opcode;
				*(data_next++) = *(opcodes++);
				*(data_next++) = *(opcodes++);
				*(data_next++) = *(opcodes++);
				*(data_next++) = *(opcodes++);
				break;
			case CGL_VM_OPCODE_STACK_POP_S_INT:
				*(data_next++) = opcode;
				*(data_next++) = *(opcodes++);
				*(data_next++) = *(opcodes++);
				*(data_next++) = *(opcodes++);
				*(data_next++) = *(opcodes++);
				break;
			case CGL_VM_OPCODE_STACK_PUSH_A:
				*(data_next++) = opcode;
				break;
			case CGL_VM_OPCODE_STACK_POP_A:
				*(data_next++) = opcode;
				break;
			case CGL_VM_OPCODE_STACK_PUSH_B:
				*(data_next++) = opcode;
				break;
			case CGL_VM_OPCODE_STACK_POP_B:
				*(data_next++) = opcode;
				break;
			case CGL_VM_OPCODE_STACK_INT:
				*(data_next++) = opcode;
				*(data_next++) = *(opcodes++);
				*(data_next++) = *(opcodes++);
				*(data_next++) = *(opcodes++);
				*(data_next++) = *(opcodes++);
				break;
			case CGL_VM_OPCODE_INT_LOAD_A:
				*(data_next++) = opcode;
				*(data_next++) = *(opcodes++);
				*(data_next++) = *(opcodes++);
				*(data_next++) = *(opcodes++);
				*(data_next++) = *(opcodes++);
				break;
			case CGL_VM_OPCODE_INT_LOAD_C:
				*(data_next++) = opcode;
				*(data_next++) = *(opcodes++);
				*(data_next++) = *(opcodes++);
				*(data_next++) = *(opcodes++);
				*(data_next++) = *(opcodes++);
				break;
			case CGL_VM_OPCODE_INT_MOV_AC:
				*(data_next++) = opcode;
				break;
			case CGL_VM_OPCODE_INT_SUM_AC:
				*(data_next++) = opcode;
				break;
			case CGL_VM_OPCODE_INT_SUB_AC:
				*(data_next++) = opcode;
				break;
			case CGL_VM_OPCODE_INT_MUL_AC:
				*(data_next++) = opcode;
				break;
			case CGL_VM_OPCODE_INT_DIV_AC:
				*(data_next++) = opcode;
				break;
			case CGL_VM_OPCODE_INT_EQ:
				*(data_next++) = opcode;
				break;
			case CGL_VM_OPCODE_INT_NE:
				*(data_next++) = opcode;
				break;
			default:
				free(classes);
				free(data);
				return 0;
			}
			opcode = *(opcodes++);
		}
	}
	*(data_next++) = 0xFF;
	if (size) {
		*size = data_size + 1;
	}
	free(classes);
	return data;
}
