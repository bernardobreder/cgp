#ifndef CGL_VM
#define CGL_VM

typedef int (*pvfv)();

#define CGL_OP_INT_REG_A 1
#define CGL_OP_INT_REG_B 2
#define CGL_OP_INT_REG_C 3
#define CGL_OP_INT_REG_D 4
#define CGL_OP_INT_REG_E 5
#define CGL_OP_INT_REG_F 6
#define CGL_OP_INT_REG_G 7
#define CGL_OP_INT_REG_H 8
#define CGL_OP_INT_STACK 9
#define CGL_OP_INT_SUM_REG_AB 50
#define CGL_OP_INT_SUB_REG_AB 51
#define CGL_OP_INT_MUL_REG_AB 52
#define CGL_OP_INT_DIV_REG_AB 53
#define CGL_OP_INT_MOV_REG_AB 54
#define CGL_OP_INT_SUM_STACK 55
#define CGL_OP_INT_SUB_STACK 56
#define CGL_OP_INT_MUL_STACK 57
#define CGL_OP_INT_DIV_STACK 58
#define CGL_OP_INT_MOV_STACK 59
#define CGL_OP_RET_STACK 200
#define CGL_OP_RET_REG_B 202
#define CGL_OP_RET_REG_C 203
#define CGL_OP_RET_INT 204
#define CGL_OP_RET_R0 210
#define CGL_OP_RET_R1 211
#define CGL_OP_RET_R2 212
#define CGL_OP_RET_F0 213
#define CGL_OP_RET_F1 214
#define CGL_OP_RET_F2 215
#define CGL_OP_RET_V0 216
#define CGL_OP_RET_V1 217
#define CGL_OP_RET_V2 218

#define CGL_OP_CALL_PREPARE 170
#define CGL_OP_CALL_EXECUTE 171
#define CGL_OP_CALL_RET_A 172
#define CGL_OP_CALL_RET_B 173
#define CGL_OP_CALL_RET_C 174

#define CGL_OP_EOF 255

#define CGL_OP_METHOD_BEGIN_MAIN 150
#define CGL_OP_METHOD_BEGIN 151
#define CGL_OP_METHOD_END 152

int cgl_vm(const unsigned char* bytecodes, unsigned int size);

void cgl_vm_int_write(unsigned char* bytes, int value);

int cgl_vm_int_read(const unsigned char* bytes);

unsigned int cgl_vm_uint_read(const unsigned char* bytes);

#define cgl_vm_opcode_eof(b) *(b++) = CGL_OP_EOF;
#define cgl_vm_opcode_method_begin_main(b) *(b++) = CGL_OP_METHOD_BEGIN_MAIN;
#define cgl_vm_opcode_method_begin(b) *(b++) = CGL_OP_METHOD_BEGIN;
#define cgl_vm_opcode_method_end(b) *(b++) = CGL_OP_METHOD_END;

#define cgl_vm_opcode_call_prepare(b) *(b++) = CGL_OP_CALL_PREPARE;
#define cgl_vm_opcode_call_execute(b, index) \
		*(b++) = CGL_OP_CALL_EXECUTE; \
		*(b++) = (index >> 24);\
		*(b++) = (index >> 16) & 0xFF;\
		*(b++) = (index >> 8) & 0xFF;\
		*(b++) = (index) & 0xFF;

#define cgl_vm_opcode_call_return_reg_a(b) *(b++) = CGL_OP_CALL_RET_A;
#define cgl_vm_opcode_call_return_reg_b(b) *(b++) = CGL_OP_CALL_RET_B;
#define cgl_vm_opcode_call_return_reg_c(b) *(b++) = CGL_OP_CALL_RET_C;

#define cgl_vm_opcode_return_reg_a(b) *(b++) = CGL_OP_RET_R0;
#define cgl_vm_opcode_return_reg_b(b) *(b++) = CGL_OP_RET_R1;
#define cgl_vm_opcode_return_reg_c(b) *(b++) = CGL_OP_RET_R2;

#define cgl_vm_opcode_int(b, value) \
		*(b++) = (value < 0 ? 0x80 : 0) + (((value<0?-value:value) >> 24) & 0x7F);\
		*(b++) = ((value<0?-value:value) >> 16) & 0xFF;\
		*(b++) = ((value<0?-value:value) >> 8) & 0xFF;\
		*(b++) = (value<0?-value:value) & 0xFF;

#define cgl_vm_opcode_return_int(b, value) \
		*(b++) = CGL_OP_RET_INT; \
		cgl_vm_opcode_int(b, value);

#define cgl_vm_opcode_return_ra(b) \
		*(b++) = CGL_OP_RET_R0;

#define cgl_vm_opcode_return_rb(b) \
		*(b++) = CGL_OP_RET_R1;

#define cgl_vm_opcode_int_load_ra(b, value) \
		*(b++) = CGL_OP_INT_REG_A; \
		cgl_vm_opcode_int(b, value);

#define cgl_vm_opcode_int_load_rb(b, value) \
		*(b++) = CGL_OP_INT_REG_B; \
		cgl_vm_opcode_int(b, value);

#define cgl_vm_opcode_int_load_rc(b, value) \
		*(b++) = CGL_OP_INT_REG_C; \
		cgl_vm_opcode_int(b, value);

#define cgl_vm_opcode_int_load_rd(b, value) \
		*(b++) = CGL_OP_INT_REG_D; \
		cgl_vm_opcode_int(b, value);

#define cgl_vm_opcode_int_load_re(b, value) \
		*(b++) = CGL_OP_INT_REG_E; \
		cgl_vm_opcode_int(b, value);

#define cgl_vm_opcode_int_load_rf(b, value) \
		*(b++) = CGL_OP_INT_REG_F; \
		cgl_vm_opcode_int(b, value);

#define cgl_vm_opcode_int_sum_ab(b) \
		*(b++) = CGL_OP_INT_SUM_REG_AB;

#define cgl_vm_opcode_int_sub_ab(b) \
		*(b++) = CGL_OP_INT_SUB_REG_AB;

#define cgl_vm_opcode_int_mul_ab(b) \
		*(b++) = CGL_OP_INT_MUL_REG_AB;

#define cgl_vm_opcode_int_div_ab(b) \
		*(b++) = CGL_OP_INT_DIV_REG_AB;

#endif
