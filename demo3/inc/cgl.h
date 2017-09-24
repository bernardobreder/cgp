#ifndef CGL
#define CGL

#include <assert.h>
#include "cgl_vm.h"

void cgl_test();

void cgl_int_write(unsigned char* bytes, int value);

int cgl_int_read(const unsigned char* bytes);

void cgl_long_write(unsigned char* bytes, unsigned long value);

unsigned long cgl_long_read(unsigned char* bytes);

void cgl_double_write(unsigned char* bytes, double v);

double cgl_double_read(const unsigned char* bytes);

int cgl_lng_nuchars_to_int(unsigned char *s, unsigned int size);

int cgl_chars_to_int(char *s);

char *cgl_uint_to_string(unsigned int value, char *digits, int base);

char *cgl_int_to_string(int value, char *digits, int base);

#define CGL_LEXICAL_EOF 0
#define CGL_LEXICAL_ID 300
#define CGL_LEXICAL_STRING_VALUE 301
#define CGL_LEXICAL_INT_VALUE 302
#define CGL_LEXICAL_HEX_VALUE 303
#define CGL_LEXICAL_GREATER_EQUAL 304
#define CGL_LEXICAL_LOWER_EQUAL 305
#define CGL_LEXICAL_GREATER_GREATER 306
#define CGL_LEXICAL_LOWER_LOWER 307
#define CGL_LEXICAL_DIFERENT 308
#define CGL_LEXICAL_EQUAL_EQUAL 309
#define CGL_LEXICAL_SUM_EQUAL 310
#define CGL_LEXICAL_SUB_EQUAL 311
#define CGL_LEXICAL_MUL_EQUAL 312
#define CGL_LEXICAL_DIV_EQUAL 313
#define CGL_LEXICAL_SUM_SUM 314
#define CGL_LEXICAL_SUB_SUB 315
#define CGL_LEXICAL_IF 400
#define CGL_LEXICAL_ELSE 401
#define CGL_LEXICAL_WHILE 402
#define CGL_LEXICAL_FOR 403
#define CGL_LEXICAL_REPEAT 404
#define CGL_LEXICAL_DO 405
#define CGL_LEXICAL_END 406
#define CGL_LEXICAL_VAR 407
#define CGL_LEXICAL_SWITCH 408
#define CGL_LEXICAL_CASE 409
#define CGL_LEXICAL_DEFAULT 410
#define CGL_LEXICAL_BREAK 411
#define CGL_LEXICAL_CONTINUE 412
#define CGL_LEXICAL_TYPE_INT 413
#define CGL_LEXICAL_TYPE_BYTE 414
#define CGL_LEXICAL_TYPE_FLOAT 415
#define CGL_LEXICAL_TYPE_DOUBLE 416
#define CGL_LEXICAL_TYPE_LONG 417
#define CGL_LEXICAL_TYPE_STRING 418
#define CGL_LEXICAL_FUNCTION 419
#define CGL_LEXICAL_OR 420
#define CGL_LEXICAL_AND 421
#define CGL_LEXICAL_RETURN 422
#define CGL_LEXICAL_TRUE 423
#define CGL_LEXICAL_FALSE 424

struct cgl_lng_token_t {
	unsigned int begin;
	unsigned int size;
	unsigned short type;
	unsigned int line;
	unsigned int column;
};

struct cgl_lng_token_t* cgl_lng_lexical(const unsigned char* text, unsigned int* tokenSize);

struct list_t {
	unsigned int size;
	unsigned int max;
	void* data;
};

struct list_t* list_new(unsigned int max);

struct list_t* list_new_full(unsigned int size);

void list_free(struct list_t* self, void* freeFunc);

unsigned char list_add(struct list_t** pself, void* value);

unsigned int list_size(struct list_t* self);

void* list_get(struct list_t* self, unsigned int index);

void list_set(struct list_t* self, unsigned int index, void* value);

const void** list_iterator(struct list_t* self);

void* list_remove(struct list_t* self, unsigned int index);

void list_clear(struct list_t* self, void* freeFunc);

#define CGL_LNG_NODE_CLASSID 1
#define CGL_LNG_INT_CLASSID 1
#define CGL_LNG_STR_CLASSID 2
#define CGL_LNG_LONG_CLASSID 3
#define CGL_LNG_FLOAT_CLASSID 4
#define CGL_LNG_DOUBLE_CLASSID 5
#define CGL_LNG_BOOL_CLASSID 6
#define CGL_LNG_ARRAY_CLASSID 7
#define CGL_LNG_ID_CLASSID 8
#define CGL_LNG_TYPE_CLASSID 9
#define CGL_LNG_TYPE_INT_CLASSID 10
#define CGL_LNG_TYPE_STR_CLASSID 11
#define CGL_LNG_TYPE_LONG_CLASSID 12
#define CGL_LNG_TYPE_FLOAT_CLASSID 13
#define CGL_LNG_TYPE_DOUBLE_CLASSID 14
#define CGL_LNG_TYPE_ID_CLASSID 15
#define CGL_LNG_TYPE_BOOL_CLASSID 16
#define CGL_LNG_TYPE_ARRAY_CLASSID 17
#define CGL_LNG_CMD_CLASSID 18
#define CGL_LNG_CALL_CLASSID 19
#define CGL_LNG_GETFIELD_CLASSID 20
#define CGL_LNG_SETFIELD_CLASSID 21
#define CGL_LNG_NEW_CLASSID 22
#define CGL_LNG_RETURN_CLASSID 23
#define CGL_LNG_IF_CLASSID 24
#define CGL_LNG_WHILE_CLASSID 25
#define CGL_LNG_FOR_CLASSID 26
#define CGL_LNG_REPEAT_CLASSID 27
#define CGL_LNG_BLOCK_CLASSID 28
#define CGL_LNG_SWITCH_CLASSID 29
#define CGL_LNG_CASE_CLASSID 30
#define CGL_LNG_EXPRESSION_CLASSID 31
#define CGL_LNG_DECLARE_CLASSID 32
#define CGL_LNG_EXP_CLASSID 33
#define CGL_LNG_TERNARY_CLASSID 34
#define CGL_LNG_NOT_CLASSID 35
#define CGL_LNG_OR_CLASSID 36
#define CGL_LNG_AND_CLASSID 37
#define CGL_LNG_OR_BIT_CLASSID 38
#define CGL_LNG_AND_BIT_CLASSID 39
#define CGL_LNG_EXCLUSIVE_CLASSID 40
#define CGL_LNG_EQUAL_CLASSID 41
#define CGL_LNG_NOT_EQUAL_CLASSID 42
#define CGL_LNG_SHIFT_LEFT_CLASSID 43
#define CGL_LNG_SHIFT_RIGHT_CLASSID 44
#define CGL_LNG_GREATER_CLASSID 45
#define CGL_LNG_LOWER_CLASSID 46
#define CGL_LNG_GREATER_EQUAL_CLASSID 47
#define CGL_LNG_LOWER_EQUAL_CLASSID 48
#define CGL_LNG_SUM_CLASSID 49
#define CGL_LNG_SUB_CLASSID 50
#define CGL_LNG_MUL_CLASSID 51
#define CGL_LNG_DIV_CLASSID 52
#define CGL_LNG_MOD_CLASSID 53
#define CGL_LNG_PRE_INC_CLASSID 54
#define CGL_LNG_PRE_DEC_CLASSID 55
#define CGL_LNG_POS_INC_CLASSID 56
#define CGL_LNG_POS_DEC_CLASSID 57

struct cgl_syntax_t {
	unsigned int memory_size;
	unsigned char* memory;
	unsigned char* text;
	struct cgl_lng_token_t* token;
	char* error;
	struct cgl_lng_node_t** stack;
	unsigned int stack_index;
	unsigned int stack_max;
};

struct cgl_lng_node_t {
	unsigned short classid;
};

struct cgl_lng_int_t {
	unsigned short classid;
	struct cgl_lng_token_t* token;
	int value;
};

struct cgl_lng_str_t {
	unsigned short classid;
	struct cgl_lng_token_t* token;
};

struct cgl_lng_long_t {
	unsigned short classid;
	struct cgl_lng_token_t* token;
	long value;
};

struct cgl_lng_float_t {
	unsigned short classid;
	struct cgl_lng_token_t* token;
	float value;
};

struct cgl_lng_double_t {
	unsigned short classid;
	struct cgl_lng_token_t* token;
	double value;
};

struct cgl_lng_bool_t {
	unsigned short classid;
	unsigned char flag;
};

struct cgl_lng_array_t {
	unsigned short classid;
	struct cgl_lng_type_t* type;
	struct cgl_lng_exp_t* value;
};

struct cgl_lng_id_t {
	unsigned short classid;
	struct cgl_lng_token_t* token;
};

struct cgl_lng_type_t {
	unsigned short classid;
};

struct cgl_lng_type_byte_t {
	unsigned short classid;
};

struct cgl_lng_type_int_t {
	unsigned short classid;
};

struct cgl_lng_type_long_t {
	unsigned short classid;
};

struct cgl_lng_type_float_t {
	unsigned short classid;
};

struct cgl_lng_type_double_t {
	unsigned short classid;
};

struct cgl_lng_type_bool_t {
	unsigned short classid;
};

struct cgl_lng_type_string_t {
	unsigned short classid;
};

struct cgl_lng_type_id_t {
	unsigned short classid;
	struct cgl_lng_token_t* name;
};

struct cgl_lng_type_array_t {
	unsigned short classid;
	struct cgl_lng_type_t* type;
};

struct cgl_lng_cmd_t {
	unsigned short classid;
};

struct cgl_lng_call_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_token_t* method;
	unsigned int param_size;
	struct cgl_lng_exp_t** params;
};

struct cgl_lng_getfield_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_token_t* field;
};

struct cgl_lng_setfield_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_token_t* field;
	struct cgl_lng_exp_t* value;
};

struct cgl_lng_new_t {
	unsigned short classid;
	struct cgl_lng_type_t* type;
	unsigned int param_size;
	struct cgl_lng_exp_t** params;
};

struct cgl_lng_return_t {
	unsigned short classid;
	struct cgl_lng_exp_t* expr;
};

struct cgl_lng_if_t {
	unsigned short classid;
	struct cgl_lng_exp_t* expr;
	struct cgl_lng_cmd_t* cmd;
};

struct cgl_lng_while_t {
	unsigned short classid;
	struct cgl_lng_exp_t* expr;
	struct cgl_lng_cmd_t* cmd;
};

struct cgl_lng_for_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* center;
	struct cgl_lng_exp_t* right;
	struct cgl_lng_cmd_t* cmd;
};

struct cgl_lng_repeat_t {
	unsigned short classid;
	struct cgl_lng_exp_t* expr;
	struct cgl_lng_cmd_t* cmd;
};

struct cgl_lng_block_t {
	unsigned short classid;
	unsigned int cmd_size;
	struct cgl_lng_cmd_t** cmds;
};

struct cgl_lng_switch_t {
	unsigned short classid;
	unsigned int case_size;
	struct cgl_lng_case_t** cases;
	struct cgl_lng_case_t* elcase;
};

struct cgl_lng_case_t {
	unsigned short classid;
	unsigned int cmd_size;
	struct cgl_lng_cmd_t** cmds;
};

struct cgl_lng_expression_t {
	unsigned short classid;
	struct cgl_lng_exp_t* expr;
};

struct cgl_lng_declare_t {
	unsigned short classid;
	struct cgl_lng_type_t* type;
	struct cgl_lng_token_t* name;
	struct cgl_lng_exp_t* value;
};

struct cgl_lng_exp_t {
	unsigned short classid;
};

struct cgl_lng_not_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
};

struct cgl_lng_pre_inc_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
};

struct cgl_lng_pre_dec_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
};

struct cgl_lng_pos_inc_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
};

struct cgl_lng_pos_dec_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
};

struct cgl_lng_or_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* right;
};

struct cgl_lng_and_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* right;
};

struct cgl_lng_or_bit_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* right;
};

struct cgl_lng_exclusive_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* right;
};

struct cgl_lng_and_bit_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* right;
};

struct cgl_lng_equal_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* right;
};

struct cgl_lng_notequal_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* right;
};

struct cgl_lng_shift_left_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* right;
};

struct cgl_lng_shift_right_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* right;
};

struct cgl_lng_greater_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* right;
};

struct cgl_lng_greater_equal_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* right;
};

struct cgl_lng_lower_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* right;
};

struct cgl_lng_lower_equal_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* right;
};

struct cgl_lng_ternary_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* center;
	struct cgl_lng_exp_t* right;
};

struct cgl_lng_sum_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* right;
};

struct cgl_lng_sub_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* right;
};

struct cgl_lng_mul_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* right;
};

struct cgl_lng_div_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* right;
};

struct cgl_lng_mod_t {
	unsigned short classid;
	struct cgl_lng_exp_t* left;
	struct cgl_lng_exp_t* right;
};

struct cgl_lng_node_t* cgl_syntax(const unsigned char* text, struct cgl_lng_token_t* token);

#endif
