#ifndef VM_H
#define VM_H

#include "platform.h"

#define CGL_VM_OPCODE_STACK_PROLOG 1
#define CGL_VM_OPCODE_STACK_LEAVE 2
#define CGL_VM_OPCODE_STACK_RET 3
#define CGL_VM_OPCODE_STACK_PUSH_S 4
#define CGL_VM_OPCODE_STACK_POP_S_INT 5
#define CGL_VM_OPCODE_STACK_PUSH_A 6
#define CGL_VM_OPCODE_STACK_POP_A 7
#define CGL_VM_OPCODE_STACK_PUSH_B 8
#define CGL_VM_OPCODE_STACK_POP_B 9
#define CGL_VM_OPCODE_STACK_INT 10

#define CGL_VM_OPCODE_JMP 50
#define CGL_VM_OPCODE_JMP_EQ 51
#define CGL_VM_OPCODE_JMP_NE 52
#define CGL_VM_OPCODE_JMP_LT 53
#define CGL_VM_OPCODE_JMP_LE 54
#define CGL_VM_OPCODE_JMP_GT 55
#define CGL_VM_OPCODE_JMP_GE 56

#define CGL_VM_OPCODE_CALL 60
#define CGL_VM_OPCODE_CALL_FUNC 61

#define CGL_VM_OPCODE_INT_LOAD_A 20
#define CGL_VM_OPCODE_INT_LOAD_C 21
#define CGL_VM_OPCODE_INT_MOV_AC 22
#define CGL_VM_OPCODE_INT_SUM_AC 23
#define CGL_VM_OPCODE_INT_SUB_AC 24
#define CGL_VM_OPCODE_INT_MUL_AC 25
#define CGL_VM_OPCODE_INT_DIV_AC 26
#define CGL_VM_OPCODE_INT_EQ 27
#define CGL_VM_OPCODE_INT_NE 28


#define CGL_VM_OPCODE_IPARAM_TO_A 70
#define CGL_VM_OPCODE_IPARAM_TO_C 71

#define CGL_VM_OPCODE_STR_MAX 80
#define CGL_VM_OPCODE_STR_DEF 81
#define CGL_VM_OPCODE_STR_LEN 82
#define CGL_VM_OPCODE_STR_LOAD_A 83
#define CGL_VM_OPCODE_STR_LOAD_C 84

#define cgl_vm_op_system_eof_inline(BYTES) *(BYTES)++ = 0xFF
#define cgl_vm_op_prolog_inline(BYTES) *(BYTES)++ = CGL_VM_OPCODE_STACK_PROLOG;
#define cgl_vm_op_leave_inline(BYTES) *(BYTES)++ = CGL_VM_OPCODE_STACK_LEAVE;
#define cgl_vm_op_return_inline(BYTES) *(BYTES)++ = CGL_VM_OPCODE_STACK_RET;
#define cgl_vm_op_ipushs_inline(BYTES,UINT) *(BYTES)++ = CGL_VM_OPCODE_STACK_PUSH_S; cgl_io_uint32_write_inline(BYTES, UINT);
#define cgl_vm_op_ipops_inline(BYTES,UINT) *(BYTES)++ = CGL_VM_OPCODE_STACK_POP_S_INT; cgl_io_uint32_write_inline(BYTES, UINT);
#define cgl_vm_op_ipusha_inline(BYTES) *(BYTES)++ = CGL_VM_OPCODE_STACK_PUSH_A;
#define cgl_vm_op_ipopa_inline(BYTES) *(BYTES)++ = CGL_VM_OPCODE_STACK_POP_A;
#define cgl_vm_op_ipushb_inline(BYTES) *(BYTES)++ = CGL_VM_OPCODE_STACK_PUSH_B;
#define cgl_vm_op_ipopb_inline(BYTES) *(BYTES)++ = CGL_VM_OPCODE_STACK_POP_B;
#define cgl_vm_op_iloada_inline(BYTES, VALUE) *(BYTES)++ = CGL_VM_OPCODE_INT_LOAD_A; cgl_io_int32_write_inline(BYTES, VALUE);
#define cgl_vm_op_iloadc_inline(BYTES, VALUE) *(BYTES)++ = CGL_VM_OPCODE_INT_LOAD_C; cgl_io_int32_write_inline(BYTES, VALUE);
#define cgl_vm_op_sloada_inline(BYTES, VALUE) *(BYTES)++ = CGL_VM_OPCODE_STR_LOAD_A; cgl_io_int32_write_inline(BYTES, VALUE);
#define cgl_vm_op_sloadc_inline(BYTES, VALUE) *(BYTES)++ = CGL_VM_OPCODE_STR_LOAD_C; cgl_io_int32_write_inline(BYTES, VALUE);
#define cgl_vm_op_imov_inline(BYTES) *(BYTES)++ = CGL_VM_OPCODE_INT_MOV_AC;
#define cgl_vm_op_isum_inline(BYTES) *(BYTES)++ = CGL_VM_OPCODE_INT_SUM_AC;
#define cgl_vm_op_isub_inline(BYTES) *(BYTES)++ = CGL_VM_OPCODE_INT_SUB_AC;
#define cgl_vm_op_imul_inline(BYTES) *(BYTES)++ = CGL_VM_OPCODE_INT_MUL_AC;
#define cgl_vm_op_idiv_inline(BYTES) *(BYTES)++ = CGL_VM_OPCODE_INT_DIV_AC;

#define cgl_vm_op_iparam_to_a_inline(BYTES,UINT) *(BYTES)++ = CGL_VM_OPCODE_IPARAM_TO_A; cgl_io_uint32_write_inline(BYTES, UINT);
#define cgl_vm_op_iparam_to_c_inline(BYTES,UINT) *(BYTES)++ = CGL_VM_OPCODE_IPARAM_TO_C; cgl_io_uint32_write_inline(BYTES, UINT);

#define cgl_vm_op_smax_inline(BYTES, VALUE) *(BYTES)++ = CGL_VM_OPCODE_STR_MAX; cgl_io_uint32_write_inline(BYTES, VALUE);
#define cgl_vm_op_sdef_inline(BYTES, VALUE, SIZE) *(BYTES)++ = CGL_VM_OPCODE_STR_DEF; \
{ \
    uint36 n, size = (SIZE)<0?strlen(VALUE):(SIZE); \
	cgl_io_uint32_write_inline(BYTES, size); \
	char* aux = (char*) (VALUE); \
	for (n = 0 ; n < size ; n++) { \
		*(BYTES)++ = *(aux)++; \
	} \
}
#define cgl_vm_op_slen_inline(BYTES) *(BYTES)++ = CGL_VM_OPCODE_STR_LEN;

/**
 * [vm] A = A == C
 */
#define cgl_vm_op_ieq_inline(BYTES) *(BYTES)++ = CGL_VM_OPCODE_INT_EQ;

#define cgl_vm_op_jmp_inline(BYTES,UINT) *(BYTES)++ = CGL_VM_OPCODE_JMP; cgl_io_uint32_write_inline(BYTES, UINT);
#define cgl_vm_op_jmp_eq_inline(BYTES,UINT) *(BYTES)++ = CGL_VM_OPCODE_JMP_EQ; cgl_io_uint32_write_inline(BYTES, UINT);
#define cgl_vm_op_jmp_ne_inline(BYTES,UINT) *(BYTES)++ = CGL_VM_OPCODE_JMP_NE; cgl_io_uint32_write_inline(BYTES, UINT);
#define cgl_vm_op_jmp_lt_inline(BYTES,UINT) *(BYTES)++ = CGL_VM_OPCODE_JMP_LT; cgl_io_uint32_write_inline(BYTES, UINT);
#define cgl_vm_op_jmp_le_inline(BYTES,UINT) *(BYTES)++ = CGL_VM_OPCODE_JMP_LE; cgl_io_uint32_write_inline(BYTES, UINT);
#define cgl_vm_op_jmp_gt_inline(BYTES,UINT) *(BYTES)++ = CGL_VM_OPCODE_JMP_GT; cgl_io_uint32_write_inline(BYTES, UINT);
#define cgl_vm_op_jmp_ge_inline(BYTES,UINT) *(BYTES)++ = CGL_VM_OPCODE_JMP_GE; cgl_io_uint32_write_inline(BYTES, UINT);

#define cgl_vm_op_call_inline(BYTES,UINT) *(BYTES)++ = CGL_VM_OPCODE_CALL; cgl_io_uint32_write_inline(BYTES, UINT);
#define cgl_vm_op_call_func_inline(BYTES,UINT) *(BYTES)++ = CGL_VM_OPCODE_CALL_FUNC; cgl_io_uint64_write_inline(BYTES, UINT);

int cgl_vm(byte* opcodes, uint36 opcode_size);

void cgl_vm_test();

#endif
