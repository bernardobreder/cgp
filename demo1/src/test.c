#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "platform.h"
#include "asm.h"
#include "vm.h"
#include "cg_io.h"
#include "lexical.h"
#include "syntax.h"
#include "linker.h"
#include "compile.h"
#include "ihashmap.h"
#include "lexasm.h"
#include "gpu.h"

static int cgl_asm_sum6_test(int a, int b, int c, int d, int e, int f) {
	return a + b + c + d + e + f;
}

static int cgl_asm_sum5_test(int a, int b, int c, int d, int e) {
	return a + b + c + d + e;
}

static int cgl_asm_sum4_test(int a, int b, int c, int d) {
	return a + b + c + d;
}

static int cgl_asm_sum3_test(int a, int b, int c) {
	return a + b + c;
}

static int cgl_asm_sum2_test(int a, int b) {
	return a + b;
}

static int cgl_asm_k_test() {
	return 10;
}

void cgl_asm_test() {
	printf("cgl_asm_test");
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_consta_inline(b, 10);
		asm_ret_inline(b);
		assert(10 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_consta_inline(b, -1);
		asm_ret_inline(b);
		assert(-1 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_consta_inline(b, 10);
		asm_constc_inline(b, 20);
		asm_addac_inline(b);
		asm_ret_inline(b);
		assert(30 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_consta_inline(b, 10);
		asm_constc_inline(b, 20);
		asm_subac_inline(b);
		asm_ret_inline(b);
		assert(-10 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_consta_inline(b, 10);
		asm_constc_inline(b, 20);
		asm_mulac_inline(b);
		asm_ret_inline(b);
		assert(200 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_consta_inline(b, 20);
		asm_constc_inline(b, 10);
		asm_divac_inline(b);
		asm_ret_inline(b);
		assert(2 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_prolog_inline(b);
		asm_consta_inline(b, 10);
		asm_constc_inline(b, 20);
		asm_addac_inline(b);
		asm_constc_inline(b, 25);
		asm_subac_inline(b);
		asm_leave_inline(b);
		asm_ret_inline(b);
		assert(5 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_prolog_inline(b);
		asm_pushs_inline(b, 2);
		asm_consta_inline(b, 5);
		asm_storea_inline(b, 0);
		asm_consta_inline(b, 10);
		asm_storea_inline(b, 1);
		asm_loada_inline(b, 0);
		asm_loadc_inline(b, 1);
		asm_addac_inline(b);
		asm_pops_inline(b, 2);
		asm_leave_inline(b);
		asm_ret_inline(b);
		assert(15 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_consta_inline(b, 10);
		asm_constc_inline(b, 15);
		asm_cmpac_inline(b);
		asm_jmpne_inline(b, asm_consta_size);
		asm_consta_inline(b, 20);
		asm_ret_inline(b);
		assert(10 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_consta_inline(b, 10);
		asm_constc_inline(b, 15);
		asm_cmpac_inline(b);
		asm_cmpa_inline(b);
		asm_ret_inline(b);
		assert(0 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_consta_inline(b, 10);
		asm_constc_inline(b, 10);
		asm_cmpac_inline(b);
		asm_cmpa_inline(b);
		asm_ret_inline(b);
		assert(1 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_op_mov_int_to_ax_inline(b, 15);
		asm_call_inline(b, asm_ret_size);
		asm_ret_inline(b);
		asm_op_mov_int_to_ax_inline(b, 10);
		asm_ret_inline(b);
		assert(10 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_op_mov_int_to_ax_inline(b, 4);
		asm_op_shift_int_left_inline(b, 1);
		asm_ret_inline(b);
		assert(8 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_op_mov_int_to_ax_inline(b, 4);
		asm_op_shift_int_right_inline(b, 1);
		asm_ret_inline(b);
		assert(2 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_op_mov_int_to_ax_inline(b, 10);
		asm_pusha_inline(b);
		asm_op_mov_int_to_ax_inline(b, 15);
		asm_popa_inline(b);
		asm_ret_inline(b);
		assert(10 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_op_mov_int_to_cx_inline(b, 10);
		asm_pushc_inline(b);
		asm_op_mov_int_to_cx_inline(b, 15);
		asm_popc_inline(b);
		asm_op_mov_cx_to_ax_inline(b);
		asm_ret_inline(b);
		assert(10 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_op_mov_int_to_ax_inline(b, 10);
		asm_op_mov_ax_to_cx_inline(b);
		asm_op_mov_int_to_ax_inline(b, 15);
		asm_op_mov_cx_to_ax_inline(b);
		asm_ret_inline(b);
		assert(10 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_consta_inline(b, cgl_asm_k_test);
		asm_op_call_ax_inline(b);
		asm_ret_inline(b);
		assert(10 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_consta_inline(b, 10);
		asm_paramadd_inline(b, 1);
		asm_consta_inline(b, 5);
		asm_paramadd_inline(b, 0);
		asm_consta_inline(b, cgl_asm_sum2_test);
		asm_op_call_ax_inline(b);
		asm_paramdel_inline(B, 2);
		asm_ret_inline(b);
		assert(15 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_consta_inline(b, 4);
		asm_paramadd_inline(b, 2);
		asm_consta_inline(b, 2);
		asm_paramadd_inline(b, 1);
		asm_consta_inline(b, 1);
		asm_paramadd_inline(b, 0);
		asm_consta_inline(b, cgl_asm_sum3_test);
		asm_op_call_ax_inline(b);
		asm_paramdel_inline(b, 3);
		asm_ret_inline(b);
		assert(7 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_op_int32_mov_edi_inline(b, 1);
		asm_op_int32_mov_esi_inline(b, 2);
		asm_op_int32_mov_edx_inline(b, 4);
		asm_op_int32_mov_ecx_inline(b, 8);
		asm_consta_inline(b, 8);
		asm_paramadd_inline(b, 3);
		asm_consta_inline(b, 4);
		asm_paramadd_inline(b, 2);
		asm_consta_inline(b, 2);
		asm_paramadd_inline(b, 1);
		asm_consta_inline(b, 1);
		asm_paramadd_inline(b, 0);
		asm_consta_inline(b, cgl_asm_sum4_test);
		asm_op_call_ax_inline(b);
		asm_paramdel_inline(b, 3);
		asm_ret_inline(b);
		assert(15 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_consta_inline(b, 16);
		asm_paramadd_inline(b, 4);
		asm_consta_inline(b, 8);
		asm_paramadd_inline(b, 3);
		asm_consta_inline(b, 4);
		asm_paramadd_inline(b, 2);
		asm_consta_inline(b, 2);
		asm_paramadd_inline(b, 1);
		asm_consta_inline(b, 1);
		asm_paramadd_inline(b, 0);
		asm_consta_inline(b, cgl_asm_sum5_test);
		asm_op_call_ax_inline(b);
		asm_paramdel_inline(b, 4);
		asm_ret_inline(b);
		assert(31 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_consta_inline(b, 32);
		asm_paramadd_inline(b, 5);
		asm_consta_inline(b, 16);
		asm_paramadd_inline(b, 4);
		asm_consta_inline(b, 8);
		asm_paramadd_inline(b, 3);
		asm_consta_inline(b, 4);
		asm_paramadd_inline(b, 2);
		asm_consta_inline(b, 2);
		asm_paramadd_inline(b, 1);
		asm_consta_inline(b, 1);
		asm_paramadd_inline(b, 0);
		asm_consta_inline(b, cgl_asm_sum6_test);
		asm_op_call_ax_inline(b);
		asm_paramdel_inline(b, 5);
		asm_ret_inline(b);
		assert(63 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		asm_prolog_inline(b);
		asm_consta_inline(b, 1);
		asm_pusha_inline(b);
		asm_consta_inline(b, 2);
		asm_pusha_inline(b);
		asm_call_inline(b, asm_pops_size + asm_ret_size + asm_leave_size);
		asm_pops_inline(b, 2);
		asm_leave_inline(b);
		asm_ret_inline(b);
		asm_prolog_inline(b);
		asm_parama_inline(b, 1);
		asm_paramc_inline(b, 0);
		asm_addac_inline(b);
		asm_leave_inline(b);
		asm_ret_inline(b);
		assert(3 == func());
		asm_free_inline(func, 1024);
	}
	{
		printf(".");
		asmFunc func = (asmFunc) asm_op_new_inline(1024);
		byte* b = (byte*) func;
		*b++ = 0x48;
		*b++ = 0xC7;
		*b++ = 0xC0;
		*b++ = 0x1;
		*b++ = 0x0;
		*b++ = 0x0;
		*b++ = 0x0;
		*b++ = 0x48;
		*b++ = 0xC7;
		*b++ = 0xC1;
		*b++ = 0x1;
		*b++ = 0x0;
		*b++ = 0x0;
		*b++ = 0x0;
		*b++ = 0x48;
		*b++ = 0x39;
		*b++ = 0xC8;
		*b++ = 0xF;
		*b++ = 0x84;
		*b++ = 0x7;
		*b++ = 0x0;
		*b++ = 0x0;
		*b++ = 0x0;
		*b++ = 0x48;
		*b++ = 0xC7;
		*b++ = 0xC0;
		*b++ = 0x2;
		*b++ = 0x0;
		*b++ = 0x0;
		*b++ = 0x0;
		*b++ = 0xC3;
		assert(1 == func());
		asm_free_inline(func, 1024);
	}
	printf("\n");
}

static void cgl_compile_test_code(const char* text) {
	printf(".");
	unsigned char* bytes = cgl_compile((const unsigned char*) text);
	assert(bytes);
	free(bytes);
}

void cgl_compile_test() {
	printf("cgl_compile_test");
	{
		cgl_compile_test_code("package a class a do end");
		cgl_compile_test_code("package a class a do int a () do end end");
		cgl_compile_test_code("package a class a do int a () do return 1 end end");
		cgl_compile_test_code("package a class a do int a () do return 1 end int b () do return 2 end end");
	}
	printf("\n");
}

void cgl_lexical_test() {
	printf("cgl_lexical_test");
	unsigned int size;
	{
		printf(".");
		unsigned char* code = (unsigned char*) "a";
		struct lng_token_t* tokens = cgl_lexical(code, &size);
		assert(size == 1);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "a b";
		struct lng_token_t* tokens = cgl_lexical(code, &size);
		assert(size == 2);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "a b c";
		struct lng_token_t* tokens = cgl_lexical(code, &size);
		assert(size == 3);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "a(a)";
		struct lng_token_t* tokens = cgl_lexical(code, &size);
		assert(size == 4);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "a.1.3";
		struct lng_token_t* tokens = cgl_lexical(code, &size);
		assert(size == 5);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "a==1";
		struct lng_token_t* tokens = cgl_lexical(code, &size);
		assert(size == 3);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "string int class";
		struct lng_token_t* tokens = cgl_lexical(code, &size);
		assert(size == 3);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "1>1";
		struct lng_token_t* tokens = cgl_lexical(code, &size);
		assert(size == 3);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "do 1>1 end";
		struct lng_token_t* tokens = cgl_lexical(code, &size);
		assert(size == 5);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "do ++a end";
		struct lng_token_t* tokens = cgl_lexical(code, &size);
		assert(size == 4);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "do --a end";
		struct lng_token_t* tokens = cgl_lexical(code, &size);
		assert(size == 4);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "do a++ end";
		struct lng_token_t* tokens = cgl_lexical(code, &size);
		assert(size == 4);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "do a-- end";
		struct lng_token_t* tokens = cgl_lexical(code, &size);
		assert(size == 4);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "do a++-- end";
		struct lng_token_t* tokens = cgl_lexical(code, &size);
		assert(size == 5);
		free(tokens);
	}
	{
		printf(".");
		unsigned char* code = (unsigned char*) "do -1 end";
		struct lng_token_t* tokens = cgl_lexical(code, &size);
		assert(size == 4);
		free(tokens);
	}
	printf("\n");
}

static void cgl_linker_test_code(const char* text) {
	printf(".");
	struct cgl_linker_t* linker = cgl_linker_new();
	{
		unsigned char* binary = cgl_compile((const unsigned char*) text);
		assert(!cgl_linker_add(linker, binary));
	}
	byte* byte = cgl_linker_compile(linker, 0);
	assert(byte);
	cgl_linker_free(linker);
	free(byte);
}

void cgl_linker_test() {
	printf("cgl_linker_test");
	cgl_linker_test_code("package a class b do end");
	cgl_linker_test_code("package a class b do static int main () do return 0 end end");
	cgl_linker_test_code("package a class b do static int main () do return 1 end end");
	printf("\n");
}

static void cgl_syntax_test_code(const char* text) {
	printf(".");
	struct cgl_syntax_t* self = cgl_syntax_new((unsigned char*) text);
	struct cgl_node_unit_t* node = cgl_syntax_unit(self);
	if (!node) {
		printf("%s\n", text);
	}
	assert(node);
	free(node);
	cgl_syntax_free(self);
}

void cgl_syntax_test() {
	printf("cgl_syntax_test");
	cgl_syntax_test_code("package a class b do end");
	cgl_syntax_test_code("package a import b.c class d do end");
	cgl_syntax_test_code("package a import b.c import d.e class d do end");
	cgl_syntax_test_code("package a import b.c import d.e class d do static int a end");
	cgl_syntax_test_code("package a import b.c import d.e class d do int main () do return 0 end end");
	cgl_syntax_test_code("package a import b.c import d.e class d do int main (int b) do end end");
	cgl_syntax_test_code("package a import b.c import d.e class d do int main (int b, int c) do end end");
	cgl_syntax_test_code("package a class b do static int a () do ; end end");
	cgl_syntax_test_code("package a class b do static int a () do 1 end end");
	cgl_syntax_test_code("package a class b do static int a () do -1 end end");
	cgl_syntax_test_code("package a class b do static int a () do 1+1 end end");
	cgl_syntax_test_code("package a class b do static int a () do 1+1+1 end end");
	cgl_syntax_test_code("package a class b do static int a () do 1+1-1*1/1 end end");
	cgl_syntax_test_code("package a class b do static int a () do 1+1-1 or 1*1/1 end end");
	cgl_syntax_test_code("package a class b do static int a () do 1+1-1 and 1*1/1 end end");
	cgl_syntax_test_code("package a class b do static int a () do 1?1:1 end end");
	cgl_syntax_test_code("package a class b do static int a () do 1==1 end end");
	cgl_syntax_test_code("package a class b do static int a () do 1!=1 end end");
	cgl_syntax_test_code("package a class b do static int a () do 1>1 end end");
	cgl_syntax_test_code("package a class b do static int a () do 1>=1 end end");
	cgl_syntax_test_code("package a class b do static int a () do 1<1 end end");
	cgl_syntax_test_code("package a class b do static int a () do 1<=1 end end");
	cgl_syntax_test_code("package a class b do static int a () do !true end end");
	cgl_syntax_test_code("package a class b do static int a () do true end end");
	cgl_syntax_test_code("package a class b do static int a () do false end end");
	cgl_syntax_test_code("package a class b do static int a () do (1!=1)==1 end end");
	cgl_syntax_test_code("package a class b do static int a () do a end end");
	cgl_syntax_test_code("package a class b do static int a () do \"a\" end end");
	cgl_syntax_test_code("package a class b do static int a () do a = 1 end end");
	cgl_syntax_test_code("package a class b do static int a () do a = b = 2 end end");
	cgl_syntax_test_code("package a class b do static int a () do ++a end end");
	cgl_syntax_test_code("package a class b do static int a () do --a end end");
	cgl_syntax_test_code("package a class b do static int a () do a++ end end");
	cgl_syntax_test_code("package a class b do static int a () do a-- end end");
	cgl_syntax_test_code("package a class b do static int a () do a++++ end end");
	cgl_syntax_test_code("package a class b do static int a () do a---- end end");
	cgl_syntax_test_code("package a class b do static int a () do a++-- end end");
	cgl_syntax_test_code("package a class b do static int a () do a--++ end end");
	cgl_syntax_test_code("package a class b do static int a () do return a+b end end");
	cgl_syntax_test_code("package a class b do static int a () do a[1] end end");
	cgl_syntax_test_code("package a class b do static int a () do a = b = c[1+1] end end");
	cgl_syntax_test_code("package a class b do static int a () do int a end end");
	cgl_syntax_test_code("package a class b do static int a () do int a = 1 end end");
	cgl_syntax_test_code("package a class b do static int a () do int a = 1 int b = 2 end end");
	cgl_syntax_test_code("package a class b do static int a () do if true do end end end");
	cgl_syntax_test_code("package a class b do static int a () do while true do end end end");
	cgl_syntax_test_code("package a class b do static int a () do repeat do end while true end end");
	cgl_syntax_test_code("package a class b do static int a () do for n = 1, 2 do end end end");
	cgl_syntax_test_code("package a class b do static int a () do for (1;1;1) do end end end");
	cgl_syntax_test_code("package a class b do static int a () do for (1;1;) do end end end");
	cgl_syntax_test_code("package a class b do static int a () do for (1;;) do end end end");
	cgl_syntax_test_code("package a class b do static int a () do for (;;) do end end end");
	cgl_syntax_test_code("package a class b do static int a () do end int b () do end end");
	cgl_syntax_test_code("package a class b do int c static int a () do end int d int b () do end end");
	cgl_syntax_test_code("package a class a do static int a () do return 1 end static int b () do return 2 end end");
	printf("\n");
}

static void cgl_vm_test_opcode() {
	unsigned char bytes[1024];
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_prolog_inline(b);
		cgl_vm_op_iloada_inline(b, 10);
		cgl_vm_op_iloadc_inline(b, 15);
		cgl_vm_op_isum_inline(b);
		cgl_vm_op_leave_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(25 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_prolog_inline(b);
		cgl_vm_op_iloada_inline(b, 10);
		cgl_vm_op_iloadc_inline(b, 15);
		cgl_vm_op_isub_inline(b);
		cgl_vm_op_leave_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(-5 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_prolog_inline(b);
		cgl_vm_op_iloada_inline(b, 10);
		cgl_vm_op_iloadc_inline(b, 15);
		cgl_vm_op_imul_inline(b);
		cgl_vm_op_leave_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(150 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_prolog_inline(b);
		cgl_vm_op_iloada_inline(b, 10);
		cgl_vm_op_iloadc_inline(b, 5);
		cgl_vm_op_idiv_inline(b);
		cgl_vm_op_leave_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(2 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_prolog_inline(b);
		cgl_vm_op_iloada_inline(b, 10);
		cgl_vm_op_iloadc_inline(b, 5);
		cgl_vm_op_isum_inline(b);
		cgl_vm_op_iloadc_inline(b, 2);
		cgl_vm_op_isub_inline(b);
		cgl_vm_op_iloadc_inline(b, 2);
		cgl_vm_op_imul_inline(b);
		cgl_vm_op_iloadc_inline(b, 2);
		cgl_vm_op_idiv_inline(b);
		cgl_vm_op_leave_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(13 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_iloada_inline(b, 10);
		cgl_vm_op_iloadc_inline(b, 5);
		cgl_vm_op_isum_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(15 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_iloada_inline(b, 10);
		cgl_vm_op_iloadc_inline(b, 5);
		cgl_vm_op_ieq_inline(b);
        cgl_vm_op_iloada_inline(b, 15);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(15 == cgl_vm(bytes, 1024));
	}
    {
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_iloada_inline(b, 10);
		cgl_vm_op_iloadc_inline(b, 10);
		cgl_vm_op_ieq_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(1 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_iloada_inline(b, 10);
		cgl_vm_op_iloadc_inline(b, 5);
		cgl_vm_op_jmp_inline(b, 4);
		cgl_vm_op_iloadc_inline(b, 2);
		cgl_vm_op_isum_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(15 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_iloada_inline(b, 10);
		cgl_vm_op_iloadc_inline(b, 5);
		cgl_vm_op_jmp_inline(b, 8);
		cgl_vm_op_iloadc_inline(b, 2);
		cgl_vm_op_iloadc_inline(b, 2);
		cgl_vm_op_iloadc_inline(b, 2);
		cgl_vm_op_iloadc_inline(b, 2);
		cgl_vm_op_iloadc_inline(b, 2);
		cgl_vm_op_isum_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(15 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_iloada_inline(b, 10);
		cgl_vm_op_iloadc_inline(b, 10);
		cgl_vm_op_jmp_eq_inline(b, 4);
		cgl_vm_op_iloadc_inline(b, 5);
		cgl_vm_op_isum_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(20 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_iloada_inline(b, 10);
		cgl_vm_op_iloadc_inline(b, 20);
		cgl_vm_op_jmp_lt_inline(b, 4);
		cgl_vm_op_iloadc_inline(b, 5);
		cgl_vm_op_isum_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(30 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_iloada_inline(b, 10);
		cgl_vm_op_iloadc_inline(b, 10);
		cgl_vm_op_jmp_le_inline(b, 4);
		cgl_vm_op_iloadc_inline(b, 5);
		cgl_vm_op_isum_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(20 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_iloada_inline(b, 10);
		cgl_vm_op_iloadc_inline(b, 20);
		cgl_vm_op_jmp_le_inline(b, 4);
		cgl_vm_op_iloadc_inline(b, 5);
		cgl_vm_op_isum_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(30 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_iloada_inline(b, 20);
		cgl_vm_op_iloadc_inline(b, 10);
		cgl_vm_op_jmp_gt_inline(b, 4);
		cgl_vm_op_iloadc_inline(b, 5);
		cgl_vm_op_isum_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(30 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_iloada_inline(b, 10);
		cgl_vm_op_iloadc_inline(b, 10);
		cgl_vm_op_jmp_ge_inline(b, 4);
		cgl_vm_op_iloadc_inline(b, 5);
		cgl_vm_op_isum_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(20 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_iloada_inline(b, 20);
		cgl_vm_op_iloadc_inline(b, 10);
		cgl_vm_op_jmp_ge_inline(b, 4);
		cgl_vm_op_iloadc_inline(b, 5);
		cgl_vm_op_isum_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(30 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_iloada_inline(b, 20);
		cgl_vm_op_iloadc_inline(b, 10);
		cgl_vm_op_call_inline(b, 5);
		cgl_vm_op_isum_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_iloada_inline(b, 40);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(50 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_iloada_inline(b, 5);
		cgl_vm_op_iloadc_inline(b, 10);
		cgl_vm_op_call_inline(b, 5);
		cgl_vm_op_isum_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_iloada_inline(b, 15);
		cgl_vm_op_call_inline(b, 8);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_iloada_inline(b, 20);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(30 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_prolog_inline(b);
		cgl_vm_op_call_inline(b, 4);
		cgl_vm_op_leave_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_iloada_inline(b, 15);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(15 == cgl_vm(bytes, 1024));
	}
    {
        printf(".");
        unsigned char* b = bytes;
        cgl_vm_op_iloada_inline(b, 5);
        cgl_vm_op_iloadc_inline(b, 10);
        cgl_vm_op_call_func_inline(b, cgl_asm_k_test);
        cgl_vm_op_return_inline(b);
        cgl_vm_op_system_eof_inline(b);
        assert(10 == cgl_vm(bytes, 1024));
    }
    {
        printf(".");
        unsigned char* b = bytes;
        cgl_vm_op_iloada_inline(b, 5);
        cgl_vm_op_iloadc_inline(b, 2);
        cgl_vm_op_call_func_inline(b, cgl_asm_k_test);
        cgl_vm_op_isum_inline(b);
        cgl_vm_op_return_inline(b);
        cgl_vm_op_system_eof_inline(b);
        assert(12 == cgl_vm(bytes, 1024));
    }
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_prolog_inline(b);
		cgl_vm_op_iloada_inline(b, 5);
		cgl_vm_op_ipusha_inline(b);
		cgl_vm_op_iloada_inline(b, 10);
		cgl_vm_op_ipusha_inline(b);
		cgl_vm_op_ipopa_inline(b);
		cgl_vm_op_ipopa_inline(b);
		cgl_vm_op_leave_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(5 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_prolog_inline(b);
		cgl_vm_op_iloada_inline(b, 5);
		cgl_vm_op_ipusha_inline(b);
		cgl_vm_op_call_inline(b, 7);
		cgl_vm_op_ipopa_inline(b);
		cgl_vm_op_leave_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_prolog_inline(b);
		cgl_vm_op_leave_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(5 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_prolog_inline(b);
		cgl_vm_op_iloada_inline(b, 5);
		cgl_vm_op_ipusha_inline(b);
		cgl_vm_op_iloada_inline(b, 10);
		cgl_vm_op_ipusha_inline(b);
		cgl_vm_op_call_inline(b, 9);
		cgl_vm_op_ipops_inline(b, 8);
		cgl_vm_op_leave_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_prolog_inline(b);
		cgl_vm_op_iparam_to_a_inline(b, 0);
		cgl_vm_op_iparam_to_c_inline(b, 4);
		cgl_vm_op_isum_inline(b);
		cgl_vm_op_leave_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(15 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_prolog_inline(b);
		cgl_vm_op_iloada_inline(b, 5);
		cgl_vm_op_ipusha_inline(b);
		cgl_vm_op_iloada_inline(b, 10);
		cgl_vm_op_ipusha_inline(b);
		cgl_vm_op_iloada_inline(b, 15);
		cgl_vm_op_ipusha_inline(b);
		cgl_vm_op_iloada_inline(b, 20);
		cgl_vm_op_ipusha_inline(b);
		cgl_vm_op_call_inline(b, 13);
		cgl_vm_op_ipops_inline(b, 4 * 4);
		cgl_vm_op_leave_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_prolog_inline(b);
		cgl_vm_op_iparam_to_a_inline(b, 0);
		cgl_vm_op_iparam_to_c_inline(b, 4);
		cgl_vm_op_isum_inline(b);
		cgl_vm_op_iparam_to_c_inline(b, 8);
		cgl_vm_op_isum_inline(b);
		cgl_vm_op_iparam_to_c_inline(b, 12);
		cgl_vm_op_isum_inline(b);
		cgl_vm_op_leave_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(50 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_prolog_inline(b);
		cgl_vm_op_iloada_inline(b, 5);
		cgl_vm_op_ipusha_inline(b);
		cgl_vm_op_call_inline(b, 7);
		cgl_vm_op_ipops_inline(b, 4);
		cgl_vm_op_leave_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_prolog_inline(b);
		cgl_vm_op_iloada_inline(b, 10);
		cgl_vm_op_ipusha_inline(b);
		cgl_vm_op_call_inline(b, 14);
		cgl_vm_op_ipops_inline(b, 4);
		cgl_vm_op_leave_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_prolog_inline(b);
		cgl_vm_op_iloada_inline(b, 15);
		cgl_vm_op_leave_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(15 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_prolog_inline(b);
		cgl_vm_op_smax_inline(b, 1);
		cgl_vm_op_sdef_inline(b, "cgp", -1);
		cgl_vm_op_sloada_inline(b, 0);
		cgl_vm_op_slen_inline(b);
		cgl_vm_op_leave_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(3 == cgl_vm(bytes, 1024));
	}
	{
		printf(".");
		unsigned char* b = bytes;
		cgl_vm_op_prolog_inline(b);
		cgl_vm_op_smax_inline(b, 1);
		cgl_vm_op_sdef_inline(b, "cgp", -1);
		cgl_vm_op_sloada_inline(b, 0);
		cgl_vm_op_slen_inline(b);
		cgl_vm_op_iloadc_inline(b, 10);
		cgl_vm_op_isum_inline(b);
		cgl_vm_op_leave_inline(b);
		cgl_vm_op_return_inline(b);
		cgl_vm_op_system_eof_inline(b);
		assert(13 == cgl_vm(bytes, 1024));
	}
}

static int cgl_vm_test_lng_code(char* text) {
	printf(".");
	struct cgl_linker_t* linker = cgl_linker_new();
	unsigned char* binary = cgl_compile((const unsigned char*) text);
	assert(!cgl_linker_add(linker, binary));
	uint36 size;
	byte* opcodes = cgl_linker_compile(linker, &size);
	cgl_linker_free(linker);
	int result = cgl_vm(opcodes, size);
	free(opcodes);
	return result;
}

void cgl_vm_test_lng() {
	{
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return 1 end end") == 1);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return 10 end end") == 10);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return 50 end end") == 50);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return -1 end end") == -1);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return -10 end end") == -10);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return -50 end end") == -50);
	}
	{
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return 1+1 end end") == 2);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return 3-2 end end") == 1);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return 2*3 end end") == 6);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return 10/5 end end") == 2);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return 1024+1024 end end") == 1024 + 1024);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return -1024+2048 end end") == 1024);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return (2) end end") == 2);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return 1+2+3 end end") == 6);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return (1+2)+3 end end") == 6);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return (1+2)+(3) end end") == 6);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return (1+2)+(3+4) end end") == 10);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return (1+2)+(3+(4+5)) end end") == 15);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return (1+2)+((3+1)+(4+5)) end end") == 16);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return ((1+2)+2)+((3+1)+(4+5)) end end") == 18);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return ((1+2)+(2+3))+((3+1)+(4+5)) end end") == 21);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return (1-2)+(3-4) end end") == -2);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return (-1+2)+(-3+4) end end") == 2);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return (2*3)+(3*4) end end") == 18);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return (1+2)*(3+4) end end") == 21);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return (1+2)-(3+4) end end") == -4);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return (4+5)/(2+1) end end") == 3);
		assert(cgl_vm_test_lng_code("package a class b do static int main () do return 1==1 end end") == 1);
//		assert(cgl_vm_test_lng_code("package a class b do static int main () do if 1==1 do return 1 end return 0 end end") == 1);
	}
}

void cgl_vm_test() {
	printf("cgl_vm_test");
	cgl_vm_test_opcode();
	cgl_vm_test_lng();
	printf("\n");
}

void cgl_ihashmap_test() {
	printf("cgl_ihashmap_test");
	{
		printf(".");
		struct ihashmap* map = cgl_ihashmap_new(0);
		cgl_ihashmap_add(map, 1, map);
		cgl_ihashmap_add(map, 10, map);
		cgl_ihashmap_add(map, 20, map);
		cgl_ihashmap_add(map, 500, map);
		assert(cgl_ihashmap_get(map, 1) == map);
		assert(cgl_ihashmap_get(map, 10) == map);
		assert(cgl_ihashmap_get(map, 20) == map);
		assert(cgl_ihashmap_get(map, 500) == map);
		assert(!cgl_ihashmap_get(map, 30));
		assert(!cgl_ihashmap_get(map, 0xFFFF));
		cgl_ihashmap_free(map);
	}
	{
		printf(".");
		struct ihashmap* map = cgl_ihashmap_new(0);
		unsigned long n;
		for (n = 0; n < 0xFFFF; n += 2) {
			cgl_ihashmap_add(map, (uint32) n, map);
		}
		for (n = 0; n < 0xFFFF; n += 2) {
			assert(cgl_ihashmap_get(map, (uint32 )n) == map);
		}
		cgl_ihashmap_free(map);
	}
	printf("\n");
}

int cgl_lexasm_test_line(const char* text) {
	printf(".");
	return cgl_asm_eval((unsigned char*) text);
}

void cgl_lexasm_test() {
	printf("cgl_lexasm_test");
	assert(10 == cgl_lexasm_test_line("imova 10 ret"));
	assert(3 == cgl_lexasm_test_line("imova 1 imovb 2 iadd ret"));
	assert(1 == cgl_lexasm_test_line("imova 2 imovb -1 iadd ret"));
	assert(-1 == cgl_lexasm_test_line("imova 1 imovb 2 isub ret"));
	assert(6 == cgl_lexasm_test_line("imova 2 imovb 3 imul ret"));
	assert(-2 == cgl_lexasm_test_line("imova 2 imovb -1 imul ret"));
	assert(12 == cgl_lexasm_test_line("imova 5 imovb 4 iadd imovb 3 isub imovb 2 imul ret"));
	assert(3 == cgl_lexasm_test_line("smax 1 sdef \"cgl\" smova 0 slen ret"));
	printf("\n");
}

int cgl_gpu_cuda_test() {
	printf("cgl_gpu_cuda_test");
	if (cgl_gpu_cuda_has()) {
		{
			printf(".");
			char* code = "extern \"C\" __global__ void add(int *a, int *b, int *c) { int tid = blockIdx.x; c[tid] = 2 * a[tid] + b[tid]; }";
			void *func, *d_A, *d_B, *d_C;
			assert(func = cgl_gpu_func_compile(code, "add"));
			int N = 10;
			int h_A[N], h_B[N], h_C[N];
			int n;
			for (n = 0; n < N; n++) {
				h_A[n] = -n;
				h_B[n] = n * n;
				h_C[n] = 0;
			}
			if (!(d_A = cgl_gpu_malloc(sizeof(h_A)))) {
				printf("fail");
				return 1;
			}
			if (!(d_B = cgl_gpu_malloc(sizeof(h_B)))) {
				printf("fail");
				return 1;
			}
			if (!(d_C = cgl_gpu_malloc(sizeof(h_C)))) {
				printf("fail");
				return 1;
			}
			if (cgl_gpu_memcpyhd(d_A, h_A, sizeof(h_A))) {
				printf("fail");
				return 1;
			}
			if (cgl_gpu_memcpyhd(d_B, h_B, sizeof(h_B))) {
				printf("fail");
				return 1;
			}
			int threadsPerBlock = 1;
			int blocksPerGrid = N;
			void* args[] = { &d_A, &d_B, &d_C };
			if (cgl_gpu_func_execute(func, blocksPerGrid, 1, 1, threadsPerBlock, 1, 1, 0, 0, args, 0)) {
				printf("fail");
				return 1;
			}
			if (cgl_gpu_memcpydh(h_C, d_C, sizeof(h_C))) {
				printf("fail");
				return 1;
			}
			for (n = 0; n < N; n++) {
				assert(h_A[n] == -n);
				assert(h_B[n] == n * n);
				assert(h_C[n] == 2 * -n + n * n);
			}
		}
	}
	printf("\n");
	return 0;
}
