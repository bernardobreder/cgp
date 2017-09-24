#include <stdio.h>
#include <lightning.h>
#include "cgl_vm.h"

struct cgl_vm_call_t {
	jit_node_t* call;
	jit_node_t* label;
};

int cgl_vm(const unsigned char* bytecodes, unsigned int size) {
	unsigned char* bytes = (unsigned char*) malloc(size);
	unsigned char* op = bytes;
	memcpy(bytes, bytecodes, size);
	jit_state_t* _jit;
	jit_node_t* main_node;
	jit_node_t* labels[1024];
	unsigned int label_index = 0;
	struct cgl_vm_call_t calls[1024];
	struct cgl_vm_call_t* callc = calls;
	init_jit(0);
	_jit = jit_new_state();
	unsigned char flag = 1;
	unsigned char erro = 0;
	while (flag) {
		switch (*(op++)) {
		case CGL_OP_INT_REG_A : {
			jit_movi(JIT_R0, cgl_vm_int_read(op));
			op += 4;
			break;
		}
		case CGL_OP_INT_REG_B : {
			jit_movi(JIT_R1, cgl_vm_int_read(op));
			op += 4;
			break;
		}
		case CGL_OP_INT_REG_C : {
			jit_movi(JIT_R2, cgl_vm_int_read(op));
			op += 4;
			break;
		}
		case CGL_OP_INT_SUM_REG_AB : {
			jit_addr(JIT_R0, JIT_R0, JIT_R1);
			break;
		}
		case CGL_OP_INT_SUB_REG_AB : {
			jit_subr(JIT_R0, JIT_R0, JIT_R1);
			break;
		}
		case CGL_OP_INT_MUL_REG_AB : {
			jit_mulr(JIT_R0, JIT_R0, JIT_R1);
			break;
		}
		case CGL_OP_INT_DIV_REG_AB : {
			jit_divr(JIT_R0, JIT_R0, JIT_R1);
			break;
		}
		case CGL_OP_RET_R0 : {
			jit_retr(JIT_R0);
			break;
		}
		case CGL_OP_RET_R1 : {
			jit_retr(JIT_R1);
			break;
		}
		case CGL_OP_RET_R2 : {
			jit_retr(JIT_R2);
			break;
		}
		case CGL_OP_RET_INT : {
			jit_reti(cgl_vm_int_read(op));
			op += 4;
			break;
		}
		case CGL_OP_METHOD_BEGIN : {
			labels[label_index++] = jit_label();
			jit_prolog();
			break;
		}
		case CGL_OP_METHOD_END : {
			jit_epilog();
			break;
		}
		case CGL_OP_METHOD_BEGIN_MAIN : {
			main_node = jit_note(0, 0);
			break;
		}
		case CGL_OP_CALL_PREPARE : {
			jit_prepare();
			break;
		}
		case CGL_OP_CALL_EXECUTE : {
			callc->label = labels[cgl_vm_uint_read(op)];
			callc->call = jit_finishi(NULL);
			callc++;
			op += 4;
			break;
		}
		case CGL_OP_CALL_RET_A : {
			jit_retval(JIT_R0);
			break;
		}
		case CGL_OP_CALL_RET_B : {
			jit_retval(JIT_R1);
			break;
		}
		case CGL_OP_CALL_RET_C : {
			jit_retval(JIT_R2);
			break;
		}
		case CGL_OP_EOF : {
			flag = 0;
			break;
		}
		default : {
			printf("Erro\n");
			erro = 1;
			flag = 0;
			break;
		}
		}
	}
	free(bytes);
	if (erro) {
		jit_clear_state();
		jit_destroy_state();
		finish_jit();
		return -1;
	}
	{
		int n, patch_size = callc - calls;
		callc = calls;
		for (n = 0; n < patch_size; n++) {
			jit_patch_at(callc->call, callc->label);
			callc++;
		}
	}
	jit_emit();
	pvfv program = (pvfv) jit_address(main_node);
	jit_clear_state();
	int result = program();
	jit_destroy_state();
	finish_jit();
	return result;
}

void cgl_vm_int_write(unsigned char* b, int value) {
	*b++ = ((value >> 24) & 0x80) + ((value >> 24) & 0x7F);
	*b++ = (value >> 16) & 0xFF;
	*b++ = (value >> 8) & 0xFF;
	*b++ = (value) & 0xFF;
}

int cgl_vm_int_read(const unsigned char* bytes) {
	unsigned char i1 = *(bytes++);
	unsigned char i2 = *(bytes++);
	unsigned char i3 = *(bytes++);
	unsigned char i4 = *(bytes++);
	if (i1 & 0x80) {
		return -(((i1 & 0x7F) << 24) + (i2 << 16) + (i3 << 8) + i4);
	} else {
		return (i1 << 24) + (i2 << 16) + (i3 << 8) + i4;
	}
}

unsigned int cgl_vm_uint_read(const unsigned char* bytes) {
	unsigned char i1 = *(bytes++);
	unsigned char i2 = *(bytes++);
	unsigned char i3 = *(bytes++);
	unsigned char i4 = *(bytes++);
	return (i1 << 24) + (i2 << 16) + (i3 << 8) + i4;
}
