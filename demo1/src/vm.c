#include <stdlib.h>
#include <string.h>
#include "platform.h"
#include "asm.h"
#include "vm.h"
#include "cg_io.h"

int cgl_vm(byte* opcodes, uint36 opcode_size) {
	int result;
	int32 int32Value;
	uint32 uint32Value;
	uint36 uint36Value;
	uint32 size = 1024;
//	uint16 params = 0;
	char** strings = 0;
	uint32 stringi = 0, stringc = 0;
	asmFunc func = (asmFunc) asm_op_new_inline(size);
	if (!func) {
		return -1;
	}
	uint32* pc = (uint32*) malloc(opcode_size * sizeof(uint32));
	if (!pc) {
		asm_free_inline(func, size);
		return -1;
	}
	byte* b = (byte*) func;
	uint32* pcaux = pc;
	byte* opcodes_aux = opcodes;
	uint32 pcc = 0;
	*(pcaux++) = pcc;
	byte opcode;
	for (; (opcode = *opcodes_aux++) != 0xFF;) {
		switch (opcode) {
		case CGL_VM_OPCODE_STACK_PROLOG:
			pcc += asm_op_prolog_size;
			break;
		case CGL_VM_OPCODE_STACK_LEAVE:
			pcc += asm_leave_size;
			break;
		case CGL_VM_OPCODE_STACK_RET:
			pcc += asm_ret_size;
			break;
		case CGL_VM_OPCODE_STACK_PUSH_S:
			pcc += asm_op_sub_sp_size;
			opcodes_aux += 4;
			break;
		case CGL_VM_OPCODE_STACK_POP_S_INT:
			pcc += asm_op_sum_sp_size;
			opcodes_aux += 4;
			break;
		case CGL_VM_OPCODE_STACK_PUSH_A:
			pcc += asm_pusha_size;
			break;
		case CGL_VM_OPCODE_STACK_POP_A:
			pcc += asm_popa_size;
			break;
		case CGL_VM_OPCODE_STACK_PUSH_B:
			pcc += asm_pushc_size;
			break;
		case CGL_VM_OPCODE_STACK_POP_B:
			pcc += asm_popc_size;
			break;
		case CGL_VM_OPCODE_INT_LOAD_A:
			pcc += asm_consta_size;
			opcodes_aux += 4;
			break;
		case CGL_VM_OPCODE_INT_LOAD_C:
			pcc += asm_constc_size;
			opcodes_aux += 4;
			break;
		case CGL_VM_OPCODE_INT_MOV_AC:
			pcc += asm_op_mov_ax_to_cx_size;
			break;
		case CGL_VM_OPCODE_INT_SUM_AC:
			pcc += asm_addac_size;
			break;
		case CGL_VM_OPCODE_INT_SUB_AC:
			pcc += asm_subac_size;
			break;
		case CGL_VM_OPCODE_INT_MUL_AC:
			pcc += asm_mulac_size;
			break;
		case CGL_VM_OPCODE_INT_DIV_AC:
			pcc += asm_divac_size;
			break;
		case CGL_VM_OPCODE_INT_NE:
			pcc += asm_cmpac_size;
			break;
		case CGL_VM_OPCODE_INT_EQ:
			pcc += asm_cmpac_size;
			break;
		case CGL_VM_OPCODE_JMP:
			pcc += asm_op_jmp_size;
			opcodes_aux += 4;
			break;
		case CGL_VM_OPCODE_JMP_EQ:
			pcc += asm_cmpac_size;
			pcc += asm_op_jmp_eq_size;
			opcodes_aux += 4;
			break;
		case CGL_VM_OPCODE_JMP_NE:
			pcc += asm_cmpac_size;
			pcc += asm_jmpne_size;
			opcodes_aux += 4;
			break;
		case CGL_VM_OPCODE_JMP_LT:
			pcc += asm_cmpac_size;
			pcc += asm_op_jmp_lt_size;
			opcodes_aux += 4;
			break;
		case CGL_VM_OPCODE_JMP_LE:
			pcc += asm_cmpac_size;
			pcc += asm_op_jmp_le_size;
			opcodes_aux += 4;
			break;
		case CGL_VM_OPCODE_JMP_GT:
			pcc += asm_cmpac_size;
			pcc += asm_op_jmp_gt_size;
			opcodes_aux += 4;
			break;
		case CGL_VM_OPCODE_JMP_GE:
			pcc += asm_cmpac_size;
			pcc += asm_op_jmp_ge_size;
			opcodes_aux += 4;
			break;
        case CGL_VM_OPCODE_CALL:
            pcc += asm_op_call_size;
            opcodes_aux += 4;
            break;
        case CGL_VM_OPCODE_CALL_FUNC:
            pcc += asm_consta_size + asm_op_call_ax_size;
            opcodes_aux += 8;
            break;
		case CGL_VM_OPCODE_IPARAM_TO_A:
			pcc += asm_op_mov_bp_to_cx_size;
			opcodes_aux += 4;
			break;
		case CGL_VM_OPCODE_IPARAM_TO_C:
			pcc += asm_op_mov_bp_to_cx_size;
			opcodes_aux += 4;
			break;
		case CGL_VM_OPCODE_STR_MAX:
			opcodes_aux += 4;
			break;
		case CGL_VM_OPCODE_STR_DEF:
			cgl_io_uint32_read_inline(uint32Value, opcodes_aux)
			opcodes_aux += uint32Value;
			break;
		case CGL_VM_OPCODE_STR_LEN:
			pcc += asm_paramadd_size(0) + asm_consta_size + asm_op_call_ax_size + asm_pops_size;
			break;
		case CGL_VM_OPCODE_STR_LOAD_A:
			pcc += asm_consta_size;
			opcodes_aux += 4;
			break;
		case CGL_VM_OPCODE_STR_LOAD_C:
			pcc += asm_constc_size;
			opcodes_aux += 4;
			break;
		default:
			free(pc);
			asm_free_inline(func, size);
			return -1;
		}
		*(pcaux++) = pcc;
	}
	opcodes_aux = opcodes;
	for (; (opcode = *opcodes_aux++) != 0xFF;) {
		switch (opcode) {
		case CGL_VM_OPCODE_STACK_PROLOG:
			asm_prolog_inline(b);
			break;
		case CGL_VM_OPCODE_STACK_LEAVE:
			asm_leave_inline(b);
			break;
		case CGL_VM_OPCODE_STACK_RET:
			asm_ret_inline(b);
			break;
		case CGL_VM_OPCODE_STACK_PUSH_S:
			cgl_io_uint32_read_inline(uint32Value, opcodes_aux);
			asm_op_sub_sp_inline(b, uint32Value);
			break;
		case CGL_VM_OPCODE_STACK_POP_S_INT:
			cgl_io_uint32_read_inline(uint32Value, opcodes_aux);
#ifdef x686
			asm_op_sum_sp_inline(b, uint32Value*2);
#else
			asm_op_sum_sp_inline(b, uint32Value)
			;
#endif
			break;
		case CGL_VM_OPCODE_STACK_PUSH_A:
			asm_pusha_inline(b);
			break;
		case CGL_VM_OPCODE_STACK_POP_A:
			asm_popa_inline(b);
			break;
		case CGL_VM_OPCODE_STACK_PUSH_B:
			asm_pushc_inline(b);
			break;
		case CGL_VM_OPCODE_STACK_POP_B:
			asm_popc_inline(b);
			break;
		case CGL_VM_OPCODE_INT_LOAD_A:
			cgl_io_int32_read_inline(int32Value, opcodes_aux);
			asm_consta_inline(b, int32Value);
			break;
		case CGL_VM_OPCODE_INT_LOAD_C:
			cgl_io_int32_read_inline(int32Value, opcodes_aux);
			asm_constc_inline(b, int32Value);
			break;
		case CGL_VM_OPCODE_INT_MOV_AC:
			asm_op_mov_ax_to_cx_inline(b);
			break;
		case CGL_VM_OPCODE_INT_SUM_AC:
			asm_addac_inline(b);
			break;
		case CGL_VM_OPCODE_INT_SUB_AC:
			asm_subac_inline(b);
			break;
		case CGL_VM_OPCODE_INT_MUL_AC:
			asm_mulac_inline(b);
			break;
		case CGL_VM_OPCODE_INT_DIV_AC:
			asm_divac_inline(b);
			break;
		case CGL_VM_OPCODE_INT_EQ:
			asm_divac_inline(b);
			break;
		case CGL_VM_OPCODE_INT_NE:
			asm_divac_inline(b);
			break;
		case CGL_VM_OPCODE_JMP:
			cgl_io_uint32_read_inline(uint32Value, opcodes_aux);
			uint32Value = pc[uint32Value] - (uint32) (b - (byte*) func) - asm_op_jmp_size;
			asm_op_jmp_inline(b, uint32Value);
			break;
		case CGL_VM_OPCODE_JMP_EQ:
			cgl_io_uint32_read_inline(uint32Value, opcodes_aux);
			uint32Value = pc[uint32Value] - (uint32) (b - (byte*) func) - asm_cmpac_size - asm_op_jmp_eq_size;
			asm_cmpac_inline(b);
			asm_op_jmp_eq_inline(b, uint32Value);
			break;
		case CGL_VM_OPCODE_JMP_NE:
			cgl_io_uint32_read_inline(uint32Value, opcodes_aux);
			uint32Value = pc[uint32Value] - (uint32) (b - (byte*) func) - asm_cmpac_size - asm_jmpne_size;
			asm_cmpac_inline(b);
			asm_jmpne_inline(b, uint32Value);
			break;
		case CGL_VM_OPCODE_JMP_LT:
			cgl_io_uint32_read_inline(uint32Value, opcodes_aux);
			uint32Value = pc[uint32Value] - (uint32) (b - (byte*) func) - asm_cmpac_size - asm_op_jmp_lt_size;
			asm_cmpac_inline(b);
			asm_op_jmp_lt_inline(b, uint32Value);
			break;
		case CGL_VM_OPCODE_JMP_LE:
			cgl_io_uint32_read_inline(uint32Value, opcodes_aux);
			uint32Value = pc[uint32Value] - (uint32) (b - (byte*) func) - asm_cmpac_size - asm_op_jmp_le_size;
			asm_cmpac_inline(b);
			asm_op_jmp_le_inline(b, uint32Value);
			break;
		case CGL_VM_OPCODE_JMP_GT:
			cgl_io_uint32_read_inline(uint32Value, opcodes_aux);
			uint32Value = pc[uint32Value] - (uint32) (b - (byte*) func) - asm_cmpac_size - asm_op_jmp_gt_size;
			asm_cmpac_inline(b);
			asm_op_jmp_gt_inline(b, uint32Value);
			break;
		case CGL_VM_OPCODE_JMP_GE:
			cgl_io_uint32_read_inline(uint32Value, opcodes_aux);
			uint32Value = pc[uint32Value] - (uint32) (b - (byte*) func) - asm_cmpac_size - asm_op_jmp_ge_size;
			asm_cmpac_inline(b);
			asm_op_jmp_ge_inline(b, uint32Value);
			break;
		case CGL_VM_OPCODE_CALL:
			cgl_io_uint32_read_inline(uint32Value, opcodes_aux);
			uint32Value = pc[uint32Value] - (uint32) (b - (byte*) func) - asm_op_call_size;
			asm_call_inline(b, uint32Value);
			break;
        case CGL_VM_OPCODE_CALL_FUNC:
            cgl_io_uint64_read_inline(uint36Value, opcodes_aux);
            asm_consta_inline(b, uint36Value);
            asm_op_call_ax_inline(b);
            break;
		case CGL_VM_OPCODE_IPARAM_TO_A:
			cgl_io_uint32_read_inline(uint32Value, opcodes_aux);
#ifdef x686
			uint32Value = uint32Value * 2 + 8 * 2;
			asm_op_mov_bp_to_ax_inline(b, uint32Value);
#else
			uint32Value = uint32Value + 8;
			asm_op_mov_bp_to_ax_inline(b, uint32Value)
			;
#endif
			break;
		case CGL_VM_OPCODE_IPARAM_TO_C:
			cgl_io_uint32_read_inline(uint32Value, opcodes_aux);
#ifdef x686
			uint32Value = uint32Value * 2 + 8 * 2;
			asm_op_mov_bp_to_cx_inline(b, uint32Value);
#else
			uint32Value = uint32Value + 8;
			asm_op_mov_bp_to_cx_inline(b, uint32Value)
			;
#endif
			break;
		case CGL_VM_OPCODE_STR_LOAD_A:
			cgl_io_uint32_read_inline(uint32Value, opcodes_aux)
			uint36Value = (uint36) strings[uint32Value];
			asm_consta_inline(b, uint36Value);
			break;
		case CGL_VM_OPCODE_STR_LOAD_C:
			cgl_io_uint32_read_inline(uint32Value, opcodes_aux)
			uint36Value = (uint36) strings[uint32Value];
			asm_constc_inline(b, uint36Value);
			break;
		case CGL_VM_OPCODE_STR_MAX:
			cgl_io_uint32_read_inline(stringc, opcodes_aux)
			strings = calloc(stringc, sizeof(char*));
			if (!strings) {
				result = -1;
				goto finish;
			}
			break;
		case CGL_VM_OPCODE_STR_DEF:
			cgl_io_uint32_read_inline(uint32Value, opcodes_aux)
			char* data = (char*) malloc(uint32Value + 1);
			if (!data) {
				result = -1;
				goto finish;
			}
			data[uint32Value] = 0;
			strncpy(data, (char*)opcodes_aux, uint32Value);
			strings[stringi++] = data;
			opcodes_aux += uint32Value;
			break;
		case CGL_VM_OPCODE_STR_LEN:
			asm_paramadd_inline(b, 0);
			asm_consta_inline(b, strlen);
			asm_op_call_ax_inline(b);
			asm_paramdel_inline(b, 1);
			break;
		default:
			result = -1;
			goto finish;
		}
	}
	result = func();
	finish: //
	if (strings) {
		for (uint32Value = 0; uint32Value < stringc; uint32Value++) {
			free(strings[uint32Value]);
		}
		free(strings);
	}
	free(pc);
	asm_free_inline(func, size);
	return result;
}
