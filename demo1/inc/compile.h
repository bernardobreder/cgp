#ifndef CGL_COMPILE_H
#define CGL_COMPILE_H

#include "vm.h"

struct cgl_compile_t {
	struct cgl_syntax_t* syntax;
	struct cgl_node_t* node;
	unsigned char test;
    unsigned long size;
	unsigned char* bytes_next;
	unsigned char* bytes;
    unsigned char reg;
};

unsigned char* cgl_compile(const unsigned char* text);

#define cgl_compile_opcode_stack_prolog(BYTES) \
*((BYTES)++) = CGL_VM_OPCODE_STACK_PROLOG;

#define cgl_compile_opcode_stack_leave(BYTES) \
*((BYTES)++) = CGL_VM_OPCODE_STACK_LEAVE;

#define cgl_compile_opcode_stack_return(BYTES) \
*((BYTES)++) = CGL_VM_OPCODE_STACK_RET;

#define cgl_compile_opcode_stack_push_s(BYTES) \
*((BYTES)++) = CGL_VM_OPCODE_STACK_PUSH_S;

#define cgl_compile_opcode_stack_push_a(BYTES) \
*((BYTES)++) = CGL_VM_OPCODE_STACK_PUSH_A;

#define cgl_compile_opcode_stack_push_b(BYTES) \
*((BYTES)++) = CGL_VM_OPCODE_STACK_PUSH_C;

#define cgl_compile_opcode_stack_pop_s(BYTES) \
*((BYTES)++) = CGL_VM_OPCODE_STACK_POP_S_INT;

#define cgl_compile_opcode_stack_pop_a(BYTES) \
*((BYTES)++) = CGL_VM_OPCODE_STACK_POP_A;

#define cgl_compile_opcode_stack_pop_b(BYTES) \
*((BYTES)++) = CGL_VM_OPCODE_STACK_POP_C;

#define cgl_compile_opcode_int_load_a(BYTES, VALUE) \
*((BYTES)++) = CGL_VM_OPCODE_INT_LOAD_A; \
cgl_io_int_write(BYTES, VALUE); \
BYTES += 4;

#define cgl_compile_opcode_int_load_c(BYTES, VALUE) \
*((BYTES)++) = CGL_VM_OPCODE_INT_LOAD_C; \
cgl_io_int_write(BYTES, VALUE); \
BYTES += 4;

#define cgl_compile_opcode_int_mov_a_to_c(BYTES) \
*((BYTES)++) = CGL_VM_OPCODE_INT_MOV_AC; \

#define cgl_compile_opcode_int_sum(BYTES) \
*((BYTES)++) = CGL_VM_OPCODE_INT_SUM_AC; \

#define cgl_compile_opcode_int_sub(BYTES) \
*((BYTES)++) = CGL_VM_OPCODE_INT_SUB_AC; \

#define cgl_compile_opcode_int_mul(BYTES) \
*((BYTES)++) = CGL_VM_OPCODE_INT_MUL_AC; \

#define cgl_compile_opcode_int_div(BYTES) \
*((BYTES)++) = CGL_VM_OPCODE_INT_DIV_AC; \

#define cgl_compile_opcode_int_cmp_eq(BYTES) \
*((BYTES)++) = CGL_VM_OPCODE_INT_EQ; \

#define cgl_compile_opcode_int_cmp_ne(BYTES) \
*((BYTES)++) = CGL_VM_OPCODE_INT_NE; \

#define cgl_compile_opcode_jmp_ne(BYTES, VALUE) \
*((BYTES)++) = CGL_VM_OPCODE_JMP_NE; \
cgl_io_int_write(BYTES, VALUE); \
BYTES += 4;

//#define cgl_compile_write_byte(SELF, VALUE) if ((SELF)->test) { (SELF)->size += 1; } else { *((SELF)->bytes++) = (VALUE); }
//
//#define cgl_compile_write_uint(SELF, VALUE) if ((SELF)->test) { (SELF)->size += 4; } else { cgl_io_uint_write((SELF)->bytes, (VALUE)); }
//
//void cgl_compile_write_int(int i);
//
//void cgl_compile_write_ulong(unsigned long i);
//
//void cgl_compile_write_long(long i);
//
//void cgl_compile_write_chars(char* text, int len);

void cgl_compile_test();

#endif
