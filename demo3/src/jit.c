//#include <stdio.h>
//#include <lightning.h>
//#include "cgl.h"
//
//static jit_state_t *_jit;
//
////void cgl_op_load_uchar(unsigned char** pop, unsigned char value) {
////	*((*pop)++) = OP_LOAD_UCHAR;
////	*((*pop)++) = value;
////}
//
//void cgl_op_int_reg_a(unsigned char** pop, int value) {
//	*((*pop)++) = OP_INT_REG_A;
//	cgl_int_write(*pop, value);
//	*pop += 4;
//}
//
//void cgl_op_int_reg_b(unsigned char** pop, int value) {
//	*((*pop)++) = OP_INT_REG_B;
//	cgl_int_write(*pop, value);
//	*pop += 4;
//}
//
//void cgl_op_int_reg_c(unsigned char** pop, int value) {
//	*((*pop)++) = OP_INT_REG_C;
//	cgl_int_write(*pop, value);
//	*pop += 4;
//}
//
//void cgl_op_double_reg_d(unsigned char** pop, double value) {
//	*((*pop)++) = OP_INT_REG_D;
//	cgl_double_write(*pop, value);
//	*pop += 8;
//}
//
//void cgl_op_double_reg_e(unsigned char** pop, double value) {
//	*((*pop)++) = OP_INT_REG_E;
//	cgl_double_write(*pop, value);
//	*pop += 8;
//}
//
//void cgl_op_double_reg_f(unsigned char** pop, double value) {
//	*((*pop)++) = OP_INT_REG_F;
//	cgl_double_write(*pop, value);
//	*pop += 8;
//}
//
//void cgl_op_double_reg_g(unsigned char** pop, double value) {
//	*((*pop)++) = OP_INT_REG_G;
//	cgl_double_write(*pop, value);
//	*pop += 8;
//}
//
//void cgl_op_double_reg_h(unsigned char** pop, double value) {
//	*((*pop)++) = OP_INT_REG_H;
//	cgl_double_write(*pop, value);
//	*pop += 8;
//}
//
//void cgl_op_int_sum_reg_ab(unsigned char** pop) {
//	*((*pop)++) = OP_INT_SUM_REG_AB;
//}
//
//void cgl_op_int_sub_reg_ab(unsigned char** pop) {
//	*((*pop)++) = OP_INT_SUB_REG_AB;
//}
//
//void cgl_op_int_mul_reg_ab(unsigned char** pop) {
//	*((*pop)++) = OP_INT_MUL_REG_AB;
//}
//
//void cgl_op_int_div_reg_ab(unsigned char** pop) {
//	*((*pop)++) = OP_INT_DIV_REG_AB;
//}
//
//void cgl_op_int_mod_reg_ab(unsigned char** pop) {
//	*((*pop)++) = OP_INT_MOV_REG_AB;
//}
//
//void cgl_op_int_sum_stack(unsigned char** pop) {
//	*((*pop)++) = OP_INT_SUM_STACK;
//}
//
//void cgl_op_int_sub_stack(unsigned char** pop) {
//	*((*pop)++) = OP_INT_SUB_STACK;
//}
//
//void cgl_op_int_mul_stack(unsigned char** pop) {
//	*((*pop)++) = OP_INT_MUL_STACK;
//}
//
//void cgl_op_int_div_stack(unsigned char** pop) {
//	*((*pop)++) = OP_INT_DIV_STACK;
//}
//
//void cgl_op_int_mod_stack(unsigned char** pop) {
//	*((*pop)++) = OP_INT_MOV_STACK;
//}
//
//void cgl_op_ret(unsigned char** pop) {
//	*((*pop)++) = OP_RET_STACK;
//}
//
//void cgl_op_ret_reg_a(unsigned char** pop) {
//	*((*pop)++) = OP_RET_REG_A;
//}
//
//void cgl_op_ret_reg_b(unsigned char** pop) {
//	*((*pop)++) = OP_RET_REG_B;
//}
//
//void cgl_op_ret_reg_c(unsigned char** pop) {
//	*((*pop)++) = OP_RET_REG_C;
//}
//
//pvfi cgl_vm(const unsigned char* bytecodes) {
//	jit_node_t *in;
//	jit_node_t *fn;
//	init_jit(0);
//	_jit = jit_new_state();
//	int stack_base, stack_ptr;
//	fn = jit_note(0, 0);
//	jit_prolog();
//	stack_ptr = stack_base = jit_allocai (32 * sizeof (int));
//	in = jit_arg();
//	const unsigned char* op = bytecodes;
//	unsigned char quit = 0;
//	while (!quit) {
//		switch (*(op++)) {
//		case OP_INT_REG_A: {
//			int value = cgl_int_read(op);
//			op += 4;
//			jit_movi(JIT_R0, value);
//			break;
//		}
//		case OP_INT_REG_B: {
//			int value = cgl_int_read(op);
//			op += 4;
//			jit_movi(JIT_R1, value);
//			break;
//		}
//		case OP_INT_REG_C: {
//			int value = cgl_int_read(op);
//			op += 4;
//			jit_movi(JIT_R2, value);
//			break;
//		}
//		case OP_INT_REG_D: {
//			double value = cgl_double_read(op);
//			op += 8;
//			jit_movi_d(JIT_F1, value);
//			break;
//		}
//		case OP_INT_REG_E: {
//			double value = cgl_double_read(op);
//			op += 8;
//			jit_movi_d(JIT_F2, value);
//			break;
//		}
//		case OP_INT_REG_F: {
//			double value = cgl_double_read(op);
//			op += 8;
//			jit_movi_d(JIT_F3, value);
//			break;
//		}
//		case OP_INT_REG_G: {
//			double value = cgl_double_read(op);
//			op += 8;
//			jit_movi_d(JIT_F4, value);
//			break;
//		}
//		case OP_INT_REG_H: {
//			double value = cgl_double_read(op);
//			op += 8;
//			jit_movi_d(JIT_F5, value);
//			break;
//		}
//		case OP_INT_STACK: {
//			int value = cgl_int_read(op);
//			op += 4;
//			stack_ptr += sizeof(int);
//			jit_movi(JIT_R0, value);
//			jit_stxi_i(stack_ptr, JIT_FP, JIT_R0); // *(SP+FP) = R0
//			break;
//		}
//		case OP_INT_SUM_REG_AB: {
//			jit_addr(JIT_R0, JIT_R0, JIT_R1); // R0 = R0 * R1
//			break;
//		}
//		case OP_INT_SUB_REG_AB: {
//			jit_subr(JIT_R0, JIT_R0, JIT_R1); // R0 = R0 - R1
//			break;
//		}
//		case OP_INT_MUL_REG_AB: {
//			jit_mulr(JIT_R0, JIT_R0, JIT_R1); // R0 = R0 * R1
//			break;
//		}
//		case OP_INT_DIV_REG_AB: {
//			jit_divr(JIT_R0, JIT_R0, JIT_R1); // R0 = R0 / R1
//			break;
//		}
//		case OP_INT_SUM_STACK: {
//			jit_ldxi_i(JIT_R1, JIT_FP, stack_ptr); // R1 = *(FP+SP)
//			stack_ptr -= sizeof(int);
//			jit_ldxi_i(JIT_R0, JIT_FP, stack_ptr); // R0 = *(FP+SP)
//			jit_addr(JIT_R0, JIT_R0, JIT_R1); // R0 = R0 * R1
//			jit_stxi_i(stack_ptr, JIT_FP, JIT_R0); // *(SP+FP) = R0
//			break;
//		}
//		case OP_INT_SUB_STACK: {
//			jit_ldxi_i(JIT_R1, JIT_FP, stack_ptr); // R1 = *(FP+SP)
//			stack_ptr -= sizeof(int);
//			jit_ldxi_i(JIT_R0, JIT_FP, stack_ptr); // R0 = *(FP+SP)
//			jit_subr(JIT_R0, JIT_R0, JIT_R1); // R0 = R0 - R1
//			jit_stxi_i(stack_ptr, JIT_FP, JIT_R0); // *(SP+FP) = R0
//			break;
//		}
//		case OP_INT_MUL_STACK: {
//			jit_ldxi_i(JIT_R1, JIT_FP, stack_ptr); // R1 = *(FP+SP)
//			stack_ptr -= sizeof(int);
//			jit_ldxi_i(JIT_R0, JIT_FP, stack_ptr); // R0 = *(FP+SP)
//			jit_mulr(JIT_R0, JIT_R0, JIT_R1); // R0 = R0 * R1
//			jit_stxi_i(stack_ptr, JIT_FP, JIT_R0); // *(SP+FP) = R0
//			break;
//		}
//		case OP_INT_DIV_STACK: {
//			jit_ldxi_i(JIT_R1, JIT_FP, stack_ptr); // R1 = *(FP+SP)
//			stack_ptr -= sizeof(int);
//			jit_ldxi_i(JIT_R0, JIT_FP, stack_ptr); // R0 = *(FP+SP)
//			jit_divr(JIT_R0, JIT_R0, JIT_R1); // R0 = R0 / R1
//			jit_stxi_i(stack_ptr, JIT_FP, JIT_R0); // *(SP+FP) = R0
//			break;
//		}
//		case OP_RET_REG_A: {
//			jit_retr(JIT_R0);
//			jit_epilog();
//			quit = 1;
//			break;
//		}
//		case OP_RET_REG_B: {
//			jit_retr(JIT_R1);
//			jit_epilog();
//			quit = 1;
//			break;
//		}
//		case OP_RET_REG_C: {
//			jit_retr(JIT_R2);
//			jit_epilog();
//			quit = 1;
//			break;
//		}
//		case OP_RET_STACK: {
//			jit_ldxi_i(JIT_R0, JIT_FP, stack_ptr); // O1 = *(O2+O3)
//			jit_retr(JIT_R0);
//			jit_epilog();
//			quit = 1;
//			break;
//		}
//		}
//	}
//	jit_emit();
//	pvfi myFunction = (pvfi) jit_address(fn);
//	jit_clear_state();
//	finish_jit();
//	return myFunction;
//}
