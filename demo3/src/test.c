#include <stdio.h>
#include <stdlib.h>
#include "cgl.h"
#include "cgl_test.h"

#define cgl_assert_notnull(actual) cgl_assert_notnull_(actual, __FILE__, __LINE__)

static void cgl_assert_notnull_(void* data, const char* file, int line) {
	if (!data) {
		printf("Expected not null at line %d in %s\n", line, file);
	}
}

#define cgl_assert_uint(expected, actual) cgl_assert_uint_(expected, actual, __FILE__, __LINE__)

static void cgl_assert_uint_(unsigned int expected, unsigned int actual, const char* file, int line) {
	if (expected != actual) {
		printf("Expected %d, and not %d at line %d in %s\n", expected, actual, line, file);
	}
}

#define cgl_assert_int(expected, actual) cgl_assert_int_(expected, actual, __FILE__, __LINE__)

static void cgl_assert_int_(int expected, int actual, const char* file, int line) {
	if (expected != actual) {
		printf("Expected %d, and not %d at line %d in %s\n", expected, actual, line, file);
	}
}

static void test_cgl_op_call() {
	printf("test_cgl_op_call...");
	unsigned char bytecodes[1024];
	unsigned char* op = bytecodes;
	{
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_return_int(op, 5);
		cgl_vm_opcode_method_end(op);
	}
	{
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_call_prepare(op);
		cgl_vm_opcode_call_execute(op, 0);
		cgl_vm_opcode_call_return_reg_a(op);
		cgl_vm_opcode_return_reg_a(op);
		cgl_vm_opcode_method_end(op);
	}
	{
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_call_prepare(op);
		cgl_vm_opcode_call_execute(op, 1);
		cgl_vm_opcode_call_return_reg_a(op);
		cgl_vm_opcode_return_reg_a(op);
		cgl_vm_opcode_method_end(op);
	}
	cgl_vm_opcode_eof(op);
	cgl_assert_int(5, cgl_vm(bytecodes, op - bytecodes));
	printf("ok\n");
}

static void test_cgl_op_int_load() {
	printf("test_cgl_op_int_load...");
	unsigned char bytecodes[1024];
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_return_int(op, 5);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(5, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_return_int(op, 0);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(0, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_return_int(op, -1);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(-1, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_return_int(op, 2147483647);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(2147483647, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_return_int(op, -2147483647);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(-2147483647, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, 1);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(1, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, 5);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(5, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, -1);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(-1, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, 2147483647);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(2147483647, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, -2147483647);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(-2147483647, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, 0xffffffff);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(2147483647, cgl_vm(bytecodes, op - bytecodes));
	}
	printf("ok\n");
}

static void test_cgl_op_int_sum() {
	printf("test_cgl_op_int_sum...");
	unsigned char bytecodes[1024];
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, 1);
		cgl_vm_opcode_int_load_rb(op, 2);
		cgl_vm_opcode_int_sum_ab(op);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(3, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, -1);
		cgl_vm_opcode_int_load_rb(op, 1);
		cgl_vm_opcode_int_sum_ab(op);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(0, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, -2147483647);
		cgl_vm_opcode_int_load_rb(op, 2147483647);
		cgl_vm_opcode_int_sum_ab(op);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(0, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, 2147483647);
		cgl_vm_opcode_int_load_rb(op, 1);
		cgl_vm_opcode_int_sum_ab(op);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(-2147483648, cgl_vm(bytecodes, op - bytecodes));
	}
	printf("ok\n");
}

static void test_cgl_op_int_sub() {
	printf("test_cgl_op_int_sub...");
	unsigned char bytecodes[1024];
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, 1);
		cgl_vm_opcode_int_load_rb(op, 1);
		cgl_vm_opcode_int_sub_ab(op);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(0, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, 0);
		cgl_vm_opcode_int_load_rb(op, 1);
		cgl_vm_opcode_int_sub_ab(op);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(-1, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, 2147483647);
		cgl_vm_opcode_int_load_rb(op, 2147483647);
		cgl_vm_opcode_int_sub_ab(op);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(0, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, -2147483647);
		cgl_vm_opcode_int_load_rb(op, 1);
		cgl_vm_opcode_int_sub_ab(op);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(-2147483648, cgl_vm(bytecodes, op - bytecodes));
	}
	printf("ok\n");
}

static void test_cgl_op_int_mul() {
	printf("test_cgl_op_int_mul...");
	unsigned char bytecodes[1024];
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, 1);
		cgl_vm_opcode_int_load_rb(op, 1);
		cgl_vm_opcode_int_mul_ab(op);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(1, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, 0);
		cgl_vm_opcode_int_load_rb(op, 1);
		cgl_vm_opcode_int_mul_ab(op);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(0, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, 2);
		cgl_vm_opcode_int_load_rb(op, 3);
		cgl_vm_opcode_int_mul_ab(op);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(6, cgl_vm(bytecodes, op - bytecodes));
	}
	printf("ok\n");
}

static void test_cgl_op_int_div() {
	printf("test_cgl_op_int_div...");
	unsigned char bytecodes[1024];
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, 1);
		cgl_vm_opcode_int_load_rb(op, 1);
		cgl_vm_opcode_int_div_ab(op);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(1, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, 0);
		cgl_vm_opcode_int_load_rb(op, 1);
		cgl_vm_opcode_int_div_ab(op);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(0, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, 4);
		cgl_vm_opcode_int_load_rb(op, 2);
		cgl_vm_opcode_int_div_ab(op);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(2, cgl_vm(bytecodes, op - bytecodes));
	}
	{
		unsigned char* op = bytecodes;
		cgl_vm_opcode_method_begin_main(op);
		cgl_vm_opcode_method_begin(op);
		cgl_vm_opcode_int_load_ra(op, 3);
		cgl_vm_opcode_int_load_rb(op, 2);
		cgl_vm_opcode_int_div_ab(op);
		cgl_vm_opcode_return_ra(op);
		cgl_vm_opcode_method_end(op);
		cgl_vm_opcode_eof(op);
		cgl_assert_int(1, cgl_vm(bytecodes, op - bytecodes));
	}
	printf("ok\n");
}

static void test_lexical() {
	printf("test_lexical...");
	const unsigned char* text = (unsigned char*) " a int > 0 < 1 <= ab : ";
	unsigned int token_size;
	struct cgl_lng_token_t* tokens = cgl_lng_lexical(text, &token_size);
	cgl_assert_notnull(tokens);
	cgl_assert_int(token_size, 9);
	cgl_assert_int(1, tokens[0].line);
	cgl_assert_int(2, tokens[0].column);
	cgl_assert_int(1, tokens[0].size);
	cgl_assert_int(1, tokens[0].begin);
	cgl_assert_int(CGL_LEXICAL_ID, tokens[0].type);
	cgl_assert_int(1, tokens[1].line);
	cgl_assert_int(4, tokens[1].column);
	cgl_assert_int(3, tokens[1].size);
	cgl_assert_int(3, tokens[1].begin);
	cgl_assert_int(CGL_LEXICAL_TYPE_INT, tokens[1].type);
	cgl_assert_int(1, tokens[2].line);
	cgl_assert_int(8, tokens[2].column);
	cgl_assert_int(1, tokens[2].size);
	cgl_assert_int(7, tokens[2].begin);
	cgl_assert_int('>', tokens[2].type);
	cgl_assert_int(1, tokens[3].line);
	cgl_assert_int(10, tokens[3].column);
	cgl_assert_int(1, tokens[3].size);
	cgl_assert_int(9, tokens[3].begin);
	cgl_assert_int(CGL_LEXICAL_INT_VALUE, tokens[3].type);
	cgl_assert_int(1, tokens[4].line);
	cgl_assert_int(12, tokens[4].column);
	cgl_assert_int(1, tokens[4].size);
	cgl_assert_int(11, tokens[4].begin);
	cgl_assert_int('<', tokens[4].type);
	cgl_assert_int(1, tokens[5].line);
	cgl_assert_int(14, tokens[5].column);
	cgl_assert_int(1, tokens[5].size);
	cgl_assert_int(13, tokens[5].begin);
	cgl_assert_int(CGL_LEXICAL_INT_VALUE, tokens[5].type);
	cgl_assert_int(1, tokens[6].line);
	cgl_assert_int(16, tokens[6].column);
	cgl_assert_int(2, tokens[6].size);
	cgl_assert_int(15, tokens[6].begin);
	cgl_assert_int(CGL_LEXICAL_LOWER_EQUAL, tokens[6].type);
	cgl_assert_int(1, tokens[7].line);
	cgl_assert_int(19, tokens[7].column);
	cgl_assert_int(2, tokens[7].size);
	cgl_assert_int(18, tokens[7].begin);
	cgl_assert_int(CGL_LEXICAL_ID, tokens[7].type);
	cgl_assert_int(1, tokens[8].line);
	cgl_assert_int(22, tokens[8].column);
	cgl_assert_int(1, tokens[8].size);
	cgl_assert_int(21, tokens[8].begin);
	cgl_assert_int(':', tokens[8].type);
	free(tokens);
	printf("ok\n");
}

static void test_syntax_int() {
	printf("test_syntax_int...");
	{
		const unsigned char* text = (unsigned char*) "1";
		unsigned int token_size;
		struct cgl_lng_token_t* tokens = cgl_lng_lexical(text, &token_size);
		cgl_assert_notnull(tokens);
		struct cgl_lng_node_t* node = cgl_syntax(text, tokens);
		free(node);
		free(tokens);
	}
	{
		const unsigned char* text = (unsigned char*) "123";
		unsigned int token_size;
		struct cgl_lng_token_t* tokens = cgl_lng_lexical(text, &token_size);
		cgl_assert_notnull(tokens);
		struct cgl_lng_node_t* node = cgl_syntax(text, tokens);
		free(node);
		free(tokens);
	}
	printf("ok\n");
}

static void test_syntax_block() {
	printf("test_syntax_block...");
	{
		const unsigned char* text = (unsigned char*) "do do 1 1 end end";
		unsigned int token_size;
		struct cgl_lng_token_t* tokens = cgl_lng_lexical(text, &token_size);
		cgl_assert_notnull(tokens);
		struct cgl_lng_node_t* node = cgl_syntax(text, tokens);
		free(node);
		free(tokens);
	}
	{
		const unsigned char* text = (unsigned char*) "do if true 1 end";
		unsigned int token_size;
		struct cgl_lng_token_t* tokens = cgl_lng_lexical(text, &token_size);
		cgl_assert_notnull(tokens);
		struct cgl_lng_node_t* node = cgl_syntax(text, tokens);
		free(node);
		free(tokens);
	}
	{
		const unsigned char* text = (unsigned char*) "while true repeat 1 while true do end";
		unsigned int token_size;
		struct cgl_lng_token_t* tokens = cgl_lng_lexical(text, &token_size);
		cgl_assert_notnull(tokens);
		struct cgl_lng_node_t* node = cgl_syntax(text, tokens);
		free(node);
		free(tokens);
	}
	printf("ok\n");
}

void cgl_test() {
	test_cgl_op_int_load();
	test_cgl_op_int_sum();
	test_cgl_op_int_sub();
	test_cgl_op_int_mul();
	test_cgl_op_int_div();
	test_cgl_op_call();
	test_lexical();
	test_syntax_int();
	test_syntax_block();
}
