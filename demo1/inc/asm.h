#ifndef ASM_H
#define ASM_H

#include "platform.h"

#ifdef __WIN32__
#include <mman.h>
#else
#include <sys/mman.h>
#endif

void cgl_asm_test();

typedef unsigned (*asmFunc)(void);

#define asm_op_int32_value(B,V) *B++ = (V)&0xFF; *B++ = ((V)>>8)&0xFF; *B++ = ((V)>>16)&0xFF; *B++ = ((V)>>24)&0xFF;
#define asm_op_int64_value(B,V) *B++ = ((int64)(V))&0xFF; *B++ = (((int64)(V))>>8)&0xFF; *B++ = (((int64)(V))>>16)&0xFF; *B++ = (((int64)(V))>>24)&0xFF; *B++ = (((int64)(V))>>32)&0xFF; *B++ = (((int64)(V))>>40)&0xFF; *B++ = (((int64)(V))>>48)&0xFF; *B++ = (((int64)(V))>>56)&0xFF;

#ifdef x686

#define asm_reg_size 8

#define asm_op_prolog_size 4
#define asm_consta_size 10
#define asm_constc_size 10
#define asm_addac_size 3
#define asm_subac_size 3
#define asm_mulac_size 4
#define asm_divac_size 5
#define asm_op_mov_ax_to_cx_size 3
#define asm_op_mov_cx_to_ax_size 3
#define asm_op_mov_sp_to_bp_size 3
#define asm_op_sum_sp_size 7
#define asm_op_sub_sp_size 7
#define asm_pops_size asm_op_sum_sp_size

#define ASM_RAX 1
#define ASM_EAX 2
#define ASM_AL 3
#define ASM_RBX 4
#define ASM_EBX 5
#define ASM_BL 6
#define ASM_RCX 6
#define ASM_ECX 7
#define ASM_CL 8

#define asm_mov(V,R) \
	if
#define asm_v8(B,V) *((B)++)=(V);
#define asm_v16(B) asm_v8(B,V&0xFF); asm_v8(B,(V>>8)&0xFF);
#define asm_v32(B) asm_v16(B,V&0xFFFF); asm_v16(B,(V>>16)&0xFFFF);
#define asm_v64(B) asm_v32(B,((uint64_t)V)&0xFFFFFFFF); asm_v32(B,(((uint64_t)V)>>32)&0xFFFFFFFF);


#define asm_op_mov_eax_to_rsp(B,V) *B++ = 0x89; *B++ = 0x84; *B++ = 0x24; asm_op_int32_value(B,((uint32)V));

#define asm_prolog_inline(B) *B++ = 0x55; *B++ = 0x48; *B++ = 0x89; *B++ = 0xE5;
#define asm_op_mov_ax_to_cx_inline(B) *B++ = 0x48; *B++ = 0x89; *B++ = 0xC1;
#define asm_op_mov_cx_to_ax_inline(B) *B++ = 0x48; *B++ = 0x89; *B++ = 0xC8;
#define asm_op_mov_sp_to_bp_inline(B) *B++ = 0x48; *B++ = 0x89; *B++ = 0xE5;
#define asm_op_sum_sp_inline(B,V) *B++ = 0x48; *B++ = 0x81; *B++ = 0xC4; asm_op_int32_value(B,((uint32)V));
#define asm_op_sub_sp_inline(B,V) *B++ = 0x48; *B++ = 0x81; *B++ = 0xEC; asm_op_int32_value(B,((uint32)V));

#define asm_op_mov_int_to_ax_inline(B,V) *B++ = 0xB8; asm_op_int32_value(B,V);
#define asm_op_mov_int_to_cx_inline(B,V) *B++ = 0xB9; asm_op_int32_value(B,V);

#define asm_consta_inline(B,V) *B++ = 0x48; *B++ = 0xB8; asm_op_int64_value(B,V);
#define asm_constc_inline(B,V) *B++ = 0x48; *B++ = 0xB9; asm_op_int64_value(B,V);
#define asm_op_int36_to_dx_inline(B,V) *B++ = 0x48; *B++ = 0xBA; asm_op_int64_value(B,V);

#define asm_addac_inline(B) *B++ = 0x48; *B++ = 0x01; *B++ = 0xC8;
#define asm_subac_inline(B) *B++ = 0x48; *B++ = 0x29; *B++ = 0xC8;
#define asm_mulac_inline(B) *B++ = 0x48; *B++ = 0x0F; *B++ = 0xAF; *B++ = 0xC1;
#define asm_divac_inline(B) *B++ = 0x48; *B++ = 0x99; *B++ = 0x48; *B++ = 0xF7; *B++ = 0xF9;

#define asm_eqac_inline(B) *B++ = 0x48; *B++ = 0x99; *B++ = 0x48; *B++ = 0xF7; *B++ = 0xF9;

// #define asm_paramadd_inline(B,I) ((I)==0)?(asm_op_mov_rax_rdi_inline(B)):((I)==1)?(asm_op_mov_rax_rsi_inline(B)):((I)==2)?(asm_op_mov_rax_rdx_inline(B)):((I)==3)?(asm_op_mov_rax_rcx_inline(B)):((I)==4)?(asm_op_mov_rax_r8_inline(B)):((I)==5)?(asm_op_mov_rax_r9_inline(B)):(assert(0))
#define asm_paramadd_size(I) ((I)==0)?(asm_op_mov_rax_rdi_size):((I)==1)?(asm_op_mov_rax_rsi_size):((I)==2)?(asm_op_mov_rax_rdx_size):((I)==3)?(asm_op_mov_rax_rcx_size):((I)==4)?(asm_op_mov_rax_r8_size):((I)==5)?(asm_op_mov_rax_r9_size):(0)
#define asm_paramadd_inline(B,I) if((I)==0){asm_op_mov_rax_rdi_inline(B)}else{if((I)==1){asm_op_mov_rax_rsi_inline(B)}else{if((I)==2){asm_op_mov_rax_rdx_inline(B)}else{if((I)==3){asm_op_mov_rax_rcx_inline(B)}else{if((I)==4){asm_op_mov_rax_r8_inline(B)}else{if((I)==5){asm_op_mov_rax_r9_inline(B)}else{}}}}}}
#define asm_paramdel_inline(B,I)

#else

#define asm_reg_size 4

#define asm_op_prolog_size 3
#define asm_consta_size 5
#define asm_constc_size 5
#define asm_addac_size 2
#define asm_subac_size 2
#define asm_mulac_size 3
#define asm_divac_size 3

#define asm_op_mov_ax_to_cx_size 2
#define asm_op_mov_cx_to_ax_size 2
#define asm_op_mov_sp_to_bp_size 2
#define asm_op_sum_sp_size 6
#define asm_op_sub_sp_size 6
#define asm_pops_size asm_op_sum_sp_size

#define asm_prolog_inline(B) *B++ = 0x55; *B++ = 0x89; *B++ = 0xE5;
#define asm_op_mov_ax_to_cx_inline(B) *B++ = 0x89; *B++ = 0xC1;
#define asm_op_mov_cx_to_ax_inline(B) *B++ = 0x89; *B++ = 0xC8;
#define asm_op_mov_sp_to_bp_inline(B) *B++ = 0x89; *B++ = 0xE5;
#define asm_op_sum_sp_inline(B,V) *B++ = 0x81; *B++ = 0xC4; asm_op_int32_value(B,((uint32)V));
#define asm_op_sub_sp_inline(B,V) *B++ = 0x81; *B++ = 0xEC; asm_op_int32_value(B,((uint32)V));

#define asm_op_mov_int_to_ax_inline(B,V) *B++ = 0xB8; asm_op_int32_value(B,V);
#define asm_op_mov_int_to_cx_inline(B,V) *B++ = 0xB9; asm_op_int32_value(B,V);

#define asm_consta_inline(B,V) *B++ = 0xB8; asm_op_int32_value(B,((uint32)V));
#define asm_constc_inline(B,V) *B++ = 0xB9; asm_op_int32_value(B,((uint32)V));

#define asm_addac_inline(B) asm_op_sum_ax_to_cx_inline(B)
#define asm_subac_inline(B) asm_op_sub_ax_to_cx_inline(B)
#define asm_mulac_inline(B) asm_op_mul_ax_to_cx_inline(B)
#define asm_divac_inline(B) asm_op_div_ax_to_cx_inline(B)

#define asm_paramadd_size(I) asm_pusha_size
#define asm_paramadd_inline(B,I) asm_pusha_inline(B);
#define asm_paramdel_inline(B,I) asm_pops_inline(B,I);

#endif

#define asm_pushs_inline(B,V) asm_op_sub_sp_inline(B,V * asm_reg_size)
#define asm_pops_inline(B,V) asm_op_sum_sp_inline(B,V * asm_reg_size)
#define asm_parama_inline(B,V) asm_op_mov_bp_to_ax_inline(b, (V+2) * asm_reg_size);
#define asm_paramc_inline(B,V) asm_op_mov_bp_to_cx_inline(b, (V+2) * asm_reg_size);
#define asm_loada_inline(B,V) asm_op_mov_bp_to_ax_inline(b, -(V+1) * asm_reg_size);
#define asm_loadc_inline(B,V) asm_op_mov_bp_to_cx_inline(b, -(V+1) * asm_reg_size);
#define asm_storea_inline(B,V) asm_op_mov_ax_to_bp_inline(b, -(V+1) * asm_reg_size);

#define asm_leave_size 1
#define asm_ret_size 1
#define asm_op_push_bp_size 1
#define asm_op_pop_bp_size 1
#define asm_pusha_size 1
#define asm_popa_size 1
#define asm_pushc_size 1
#define asm_popc_size 1
#define asm_op_shift_int_left_size 3
#define asm_op_shift_int_right_size 3
#define asm_op_mov_bp_to_ax_size 6
#define asm_op_mov_bp_to_cx_size 6
#define asm_op_mov_ax_to_bp_size 6
#define asm_cmpac_size 2
#define asm_op_jmp_size 5
#define asm_op_jmp_eq_size 6
#define asm_jmpne_size 6
#define asm_op_jmp_lt_size 6
#define asm_op_jmp_le_size 6
#define asm_op_jmp_gt_size 6
#define asm_op_jmp_ge_size 6
#define asm_op_call_size 5
#define asm_op_call_ax_size 2
// #define asm_op_mov_long_to_ax_size 10

#define asm_op_new_inline(SIZE) mmap(0, SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_ANON | MAP_PRIVATE, 0, 0)

#define asm_free_inline(FUNC,SIZE) munmap(FUNC, SIZE)

#define asm_leave_inline(B) *B++ = 0x5D;
#define asm_ret_inline(B) *B++ = 0xC3;
#define asm_op_push_bp_inline(B) *B++ = 0x55;
#define asm_op_pop_bp_inline(B) *B++ = 0x5D;
#define asm_pusha_inline(B) *B++ = 0x50;
#define asm_popa_inline(B) *B++ = 0x58;
#define asm_pushc_inline(B) *B++ = 0x51;
#define asm_popc_inline(B) *B++ = 0x59;
#define asm_op_shift_int_left_inline(B,V) *B++ = 0xC1; *B++ = 0xE0; *B++ = V&0xFF;
#define asm_op_shift_int_right_inline(B,V) *B++ = 0xC1; *B++ = 0xF8; *B++ = V&0xFF;
#define asm_op_sum_ax_to_cx_inline(B) *B++ = 0x01; *B++ = 0xC8;
#define asm_op_sub_ax_to_cx_inline(B) *B++ = 0x29; *B++ = 0xC8;
#define asm_op_mul_ax_to_cx_inline(B) *B++ = 0x0F; *B++ = 0xAF; *B++ = 0xC1;
#define asm_op_div_ax_to_cx_inline(B) *B++ = 0x99; *B++ = 0xF7; *B++ = 0xF9;
#define asm_op_mov_bp_to_ax_inline(B,V) *B++ = 0x8B; *B++ = 0x85; asm_op_int32_value(B,V);
#define asm_op_mov_bp_to_cx_inline(B,V) *B++ = 0x8B; *B++ = 0x8D; asm_op_int32_value(B,V);
#define asm_op_mov_ax_to_bp_inline(B,V) *B++ = 0x89; *B++ = 0x85; asm_op_int32_value(B,V);
#define asm_cmpac_inline(B) *B++ = 0x39; *B++ = 0xC8;
#define asm_cmpa_inline(B) *B++ = 0x0F; *B++ = 0x94; *B++ = 0xC0; *B++ = 0x48; *B++ = 0x0F; *B++ = 0xB6; *B++ = 0xC0;
#define asm_op_jmp_inline(B,V) *B++ = 0xE9; asm_op_int32_value(B,V);
#define asm_jmpne_inline(B,V) *B++ = 0x0F; *B++ = 0x85; asm_op_int32_value(B,V);
#define asm_op_jmp_eq_inline(B,V) *B++ = 0x0F; *B++ = 0x84; asm_op_int32_value(B,V);
#define asm_op_jmp_lt_inline(B,V) *B++ = 0x0F; *B++ = 0x8C; asm_op_int32_value(B,V);
#define asm_op_jmp_le_inline(B,V) *B++ = 0x0F; *B++ = 0x8E; asm_op_int32_value(B,V);
#define asm_op_jmp_gt_inline(B,V) *B++ = 0x0F; *B++ = 0x8F; asm_op_int32_value(B,V);
#define asm_op_jmp_ge_inline(B,V) *B++ = 0x0F; *B++ = 0x8D; asm_op_int32_value(B,V);
#define asm_call_inline(B,V) *B++ = 0xE8; asm_op_int32_value(B,V);
#define asm_op_call_ax_inline(B) *B++ = 0xFF; *B++ = 0xD0;
// Primeiro argumento
#define asm_op_int32_mov_edi_inline(B,V) *B++ = 0xBF; asm_op_int32_value(B,V);
// Segundo argumento
#define asm_op_int32_mov_esi_inline(B,V) *B++ = 0xBE; asm_op_int32_value(B,V);
// Terceiro argumento
#define asm_op_int32_mov_edx_inline(B,V) *B++ = 0xBA; asm_op_int32_value(B,V);
// Quarto argumento
#define asm_op_int32_mov_ecx_inline(B,V) asm_op_mov_int_to_cx_inline(B,V);
// Quinto argumento
#define asm_op_int32_mov_r8d_inline(B,V) *B++ = 0x41; *B++ = 0xB8; asm_op_int32_value(B,V);
// Sexto argumento
#define asm_op_int32_mov_r9d_inline(B,V) *B++ = 0x41; *B++ = 0xB9; asm_op_int32_value(B,V);

// Primeiro argumento
#define asm_op_int64_mov_edi_inline(B,V) *B++ = 0x48; *B++ = 0xBF; asm_op_int64_value(B,V);
#define asm_op_mov_rax_rdi_size 3
#define asm_op_mov_rax_rdi_inline(B) *B++ = 0x48; *B++ = 0x89; *B++ = 0xC7;
// Segundo argumento
#define asm_op_int64_mov_esi_inline(B,V) *B++ = 0x48; *B++ = 0xBE; asm_op_int64_value(B,V);
#define asm_op_mov_rax_rsi_size 3
#define asm_op_mov_rax_rsi_inline(B) *B++ = 0x48; *B++ = 0x89; *B++ = 0xC6;
// Terceiro argumento
#define asm_op_int64_mov_edx_inline(B,V) *B++ = 0x48; *B++ = 0xBA; asm_op_int64_value(B,V);
#define asm_op_mov_rax_rdx_size 3
#define asm_op_mov_rax_rdx_inline(B) *B++ = 0x48; *B++ = 0x89; *B++ = 0xC2;
// Quarto argumento
#define asm_op_int64_mov_ecx_inline(B,V) *B++ = 0x48; *B++ = 0xB9; asm_op_int64_value(B,V);
#define asm_op_mov_rax_rcx_size 3
#define asm_op_mov_rax_rcx_inline(B) *B++ = 0x48; *B++ = 0x89; *B++ = 0xC1;
// Quinto argumento
#define asm_op_int64_mov_r8_inline(B,V) *B++ = 0x49; *B++ = 0xB8; asm_op_int64_value(B,V);
#define asm_op_mov_rax_r8_size 3
#define asm_op_mov_rax_r8_inline(B) *B++ = 0x49; *B++ = 0x89; *B++ = 0xC0;
// Sexto argumento
#define asm_op_int64_mov_r9_inline(B,V) *B++ = 0x49; *B++ = 0xB9; asm_op_int64_value(B,V);
#define asm_op_mov_rax_r9_size 3
#define asm_op_mov_rax_r9_inline(B) *B++ = 0x49; *B++ = 0x89; *B++ = 0xC1;

#endif
