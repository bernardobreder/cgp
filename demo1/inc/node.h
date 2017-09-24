#ifndef CGL_NODE_H
#define CGL_NODE_H

#include "compile.h"

#define CGL_NODE_UNIT_TYPE 1
#define CGL_NODE_BLOCK_TYPE 2
#define CGL_NODE_WHILE_TYPE 3
#define CGL_NODE_REPEAT_TYPE 4
#define CGL_NODE_FORN_TYPE 5
#define CGL_NODE_FOREACH_TYPE 6
#define CGL_NODE_IF_TYPE 7
#define CGL_NODE_EXPRESSION_TYPE 8
#define CGL_NODE_TERNARY_TYPE 9
#define CGL_NODE_ID_TYPE 10
#define CGL_NODE_INT_TYPE 11
#define CGL_NODE_STR_TYPE 12
#define CGL_NODE_BOOL_TYPE 13
#define CGL_NODE_AND_TYPE 14
#define CGL_NODE_OR_TYPE 15
#define CGL_NODE_EQ_TYPE 16
#define CGL_NODE_NE_TYPE 17
#define CGL_NODE_LT_TYPE 18
#define CGL_NODE_LE_TYPE 19
#define CGL_NODE_GT_TYPE 20
#define CGL_NODE_GE_TYPE 21
#define CGL_NODE_NOT_TYPE 22
#define CGL_NODE_SHL_TYPE 23
#define CGL_NODE_SHR_TYPE 24
#define CGL_NODE_SUM_TYPE 25
#define CGL_NODE_SUB_TYPE 26
#define CGL_NODE_MUL_TYPE 27
#define CGL_NODE_DIV_TYPE 28
#define CGL_NODE_MOD_TYPE 29
#define CGL_NODE_PREINC_TYPE 30
#define CGL_NODE_PREDEC_TYPE 31
#define CGL_NODE_POSINC_TYPE 32
#define CGL_NODE_POSDEC_TYPE 33
#define CGL_NODE_INT_TYPE_TYPE 34
#define CGL_NODE_STR_TYPE_TYPE 35
#define CGL_NODE_BOOL_TYPE_TYPE 36
#define CGL_NODE_PACKAGE_TYPE 37
#define CGL_NODE_IMPORT_TYPE 38
#define CGL_NODE_CLASS_TYPE 39
#define CGL_NODE_FIELD_TYPE 40
#define CGL_NODE_METHOD_TYPE 41
#define CGL_NODE_VARIABLE_TYPE 42
#define CGL_NODE_DECLARE_TYPE 43
#define CGL_NODE_ARRAY_GET_TYPE 44
#define CGL_NODE_ARRAY_SET_TYPE 45
#define CGL_NODE_ARRAY_LEN_TYPE 46
#define CGL_NODE_ARRAY_INC_TYPE 47
#define CGL_NODE_PARAMETER_TYPE 48
#define CGL_NODE_COMMA_TYPE 49
#define CGL_NODE_ASSIGN_TYPE 50
#define CGL_NODE_ARRAY_TYPE 51
#define CGL_NODE_NEG_TYPE 52
#define CGL_NODE_RETURN_TYPE 53

struct cgl_node_t {
	struct cgl_node_vtable_t* vtable;
};

struct cgl_node_unit_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_package_t* package;
	struct cgl_node_import_t* imports;
	unsigned int import_count;
	struct cgl_node_class_t* clazz;
};

struct cgl_node_command_t {
	struct cgl_node_vtable_t* vtable;
};

struct cgl_node_block_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_command_t** cmds;
};

struct cgl_node_while_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* cond;
	struct cgl_node_command_t* cmd;
};

struct cgl_node_repeat_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* cond;
	struct cgl_node_command_t* cmd;
};

struct cgl_node_forn_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* init;
	struct cgl_node_value_t* cond;
	struct cgl_node_value_t* inc;
	struct cgl_node_command_t* cmd;
};

struct cgl_node_foreach_t {
	struct cgl_node_vtable_t* vtable;
	struct lng_token_t* name;
	struct cgl_node_value_t* begin;
	struct cgl_node_value_t* end;
	struct cgl_node_command_t* cmd;
};

struct cgl_node_if_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* cond;
	struct cgl_node_command_t* cmd;
};

struct cgl_node_return_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
};

struct cgl_node_expression_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
};

struct cgl_node_unary_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
};

struct cgl_node_binary_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
	struct cgl_node_value_t* right;
};

struct cgl_node_ternary_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
	struct cgl_node_value_t* right;
	struct cgl_node_value_t* center;
};

struct cgl_node_primitive_t {
	struct cgl_node_vtable_t* vtable;
};

struct cgl_node_value_t {
	struct cgl_node_vtable_t* vtable;
};

struct cgl_node_id_t {
	struct cgl_node_vtable_t* vtable;
	struct lng_token_t* name;
};

struct cgl_node_int_t {
	struct cgl_node_vtable_t* vtable;
	int value;
};

struct cgl_node_str_t {
	struct cgl_node_vtable_t* vtable;
	int value;
};

struct cgl_node_bool_t {
	struct cgl_node_vtable_t* vtable;
	unsigned char value;
};

struct cgl_node_and_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
	struct cgl_node_value_t* right;
};

struct cgl_node_or_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
	struct cgl_node_value_t* right;
};

struct cgl_node_eq_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
	struct cgl_node_value_t* right;
};

struct cgl_node_neq_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
	struct cgl_node_value_t* right;
};

struct cgl_node_ge_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
	struct cgl_node_value_t* right;
};

struct cgl_node_gt_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
	struct cgl_node_value_t* right;
};

struct cgl_node_lt_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
	struct cgl_node_value_t* right;
};

struct cgl_node_le_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
	struct cgl_node_value_t* right;
};

struct cgl_node_shl_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
	struct cgl_node_value_t* right;
};

struct cgl_node_shr_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
	struct cgl_node_value_t* right;
};

struct cgl_node_sum_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
	struct cgl_node_value_t* right;
};

struct cgl_node_sub_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
	struct cgl_node_value_t* right;
};

struct cgl_node_mul_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
	struct cgl_node_value_t* right;
};

struct cgl_node_div_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
	struct cgl_node_value_t* right;
};

struct cgl_node_not_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
};

struct cgl_node_neg_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
};

struct cgl_node_preinc_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
};

struct cgl_node_predec_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
};

struct cgl_node_posinc_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
};

struct cgl_node_posdec_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
};

struct cgl_node_type_t {
	struct cgl_node_vtable_t* vtable;
};

struct cgl_node_type_int_t {
	struct cgl_node_vtable_t* vtable;
	struct lng_token_t* token;
};

struct cgl_node_type_str_t {
	struct cgl_node_vtable_t* vtable;
	struct lng_token_t* token;
};

struct cgl_node_type_bool_t {
	struct cgl_node_vtable_t* vtable;
	struct lng_token_t* token;
};

struct cgl_node_package_t {
	struct cgl_node_vtable_t* vtable;
	struct lng_token_t* name;
};

struct cgl_node_import_t {
	struct cgl_node_vtable_t* vtable;
	struct lng_token_t* name;
};

struct cgl_node_assign_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
	struct cgl_node_value_t* value;
};

struct cgl_node_declare_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_type_t* type;
	struct lng_token_t* name;
	struct cgl_node_value_t* value;
};

struct cgl_node_array_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_value_t* left;
	struct cgl_node_value_t* value;
};

struct cgl_node_class_t {
	struct cgl_node_vtable_t* vtable;
	struct lng_token_t* name;
	struct cgl_node_method_t* methods;
	unsigned int method_count;
	struct cgl_node_field_t* fields;
	unsigned int field_count;
};

struct cgl_node_field_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_type_t* clazz;
	struct lng_token_t* name;
};

struct cgl_node_method_t {
	struct cgl_node_vtable_t* vtable;
	unsigned char is_static;
	struct cgl_node_class_t* clazz;
	struct lng_token_t* name;
	struct cgl_node_type_t* type;
	struct cgl_node_param_t* parameters;
	struct cgl_node_block_t* cmd;
};

struct cgl_node_param_t {
	struct cgl_node_vtable_t* vtable;
	struct cgl_node_type_t* type;
	struct lng_token_t* name;
};

struct cgl_node_vtable_t {
	unsigned char classid;
	void (*head)(struct cgl_node_t*, struct cgl_compile_t*);
	void (*body)(struct cgl_node_t*, struct cgl_compile_t*);
	unsigned long (*size)(struct cgl_node_t*, struct cgl_compile_t*);
	void (*build)(struct cgl_node_t*, struct cgl_compile_t*);
};

void cgl_node_unit_head(struct cgl_node_unit_t* self, struct cgl_compile_t* compile);

void cgl_node_unit_body(struct cgl_node_unit_t* self, struct cgl_compile_t* compile);

unsigned long cgl_node_unit_size(struct cgl_node_unit_t* self, struct cgl_compile_t* compile);

void cgl_node_unit_build(struct cgl_node_unit_t* self, struct cgl_compile_t* compile);

void cgl_node_package_head(struct cgl_node_package_t* self, struct cgl_compile_t* compile);

void cgl_node_package_body(struct cgl_node_package_t* self, struct cgl_compile_t* compile);

unsigned long cgl_node_package_size(struct cgl_node_unit_t* self, struct cgl_compile_t* compile);

void cgl_node_package_build(struct cgl_node_package_t* self, struct cgl_compile_t* compile);

void cgl_node_import_head(struct cgl_node_import_t* self, struct cgl_compile_t* compile);

void cgl_node_import_body(struct cgl_node_import_t* self, struct cgl_compile_t* compile);

unsigned long cgl_node_import_size(struct cgl_node_unit_t* self, struct cgl_compile_t* compile);

void cgl_node_import_build(struct cgl_node_import_t* self, struct cgl_compile_t* compile);

void cgl_node_class_head(struct cgl_node_class_t* self, struct cgl_compile_t* compile);

void cgl_node_class_body(struct cgl_node_class_t* self, struct cgl_compile_t* compile);

unsigned long cgl_node_class_size(struct cgl_node_unit_t* self, struct cgl_compile_t* compile);

void cgl_node_class_build(struct cgl_node_class_t* self, struct cgl_compile_t* compile);

void cgl_node_field_head(struct cgl_node_field_t* self, struct cgl_compile_t* compile);

void cgl_node_field_body(struct cgl_node_field_t* self, struct cgl_compile_t* compile);

unsigned long cgl_node_field_size(struct cgl_node_unit_t* self, struct cgl_compile_t* compile);

void cgl_node_field_build(struct cgl_node_field_t* self, struct cgl_compile_t* compile);

void cgl_node_method_head(struct cgl_node_method_t* self, struct cgl_compile_t* compile);

void cgl_node_method_body(struct cgl_node_method_t* self, struct cgl_compile_t* compile);

unsigned long cgl_node_method_size(struct cgl_node_method_t* self, struct cgl_compile_t* compile);

void cgl_node_method_build(struct cgl_node_method_t* self, struct cgl_compile_t* compile);

void cgl_node_type_int_head(struct cgl_node_type_int_t* self, struct cgl_compile_t* compile);

void cgl_node_type_int_body(struct cgl_node_type_int_t* self, struct cgl_compile_t* compile);

unsigned long cgl_node_type_int_size(struct cgl_node_unit_t* self, struct cgl_compile_t* compile);

void cgl_node_type_int_build(struct cgl_node_type_int_t* self, struct cgl_compile_t* compile);

void cgl_node_param_head(struct cgl_node_param_t* self, struct cgl_compile_t* compile);

void cgl_node_param_body(struct cgl_node_param_t* self, struct cgl_compile_t* compile);

unsigned long cgl_node_param_size(struct cgl_node_unit_t* self, struct cgl_compile_t* compile);

void cgl_node_param_build(struct cgl_node_param_t* self, struct cgl_compile_t* compile);

void cgl_node_block_head(struct cgl_node_block_t* self, struct cgl_compile_t* compile);

void cgl_node_block_body(struct cgl_node_block_t* self, struct cgl_compile_t* compile);

unsigned long cgl_node_block_size(struct cgl_node_block_t* self, struct cgl_compile_t* compile);

void cgl_node_block_build(struct cgl_node_block_t* self, struct cgl_compile_t* compile);

void cgl_node_return_head(struct cgl_node_return_t* self, struct cgl_compile_t* compile);

void cgl_node_return_body(struct cgl_node_return_t* self, struct cgl_compile_t* compile);

unsigned long cgl_node_return_size(struct cgl_node_return_t* self, struct cgl_compile_t* compile);

void cgl_node_return_build(struct cgl_node_return_t* self, struct cgl_compile_t* compile);

void cgl_node_expression_head(struct cgl_node_expression_t* self, struct cgl_compile_t* compile);

void cgl_node_expression_body(struct cgl_node_expression_t* self, struct cgl_compile_t* compile);

unsigned long cgl_node_expression_size(struct cgl_node_expression_t* self, struct cgl_compile_t* compile);

void cgl_node_expression_build(struct cgl_node_expression_t* self, struct cgl_compile_t* compile);

void cgl_node_int_head(struct cgl_node_int_t* self, struct cgl_compile_t* compile);

void cgl_node_int_body(struct cgl_node_int_t* self, struct cgl_compile_t* compile);

unsigned long cgl_node_int_size(struct cgl_node_int_t* self, struct cgl_compile_t* compile);

void cgl_node_int_build(struct cgl_node_int_t* self, struct cgl_compile_t* compile);

void cgl_node_neg_head(struct cgl_node_neg_t* self, struct cgl_compile_t* compile);

void cgl_node_neg_body(struct cgl_node_neg_t* self, struct cgl_compile_t* compile);

unsigned long cgl_node_neg_size(struct cgl_node_neg_t* self, struct cgl_compile_t* compile);

void cgl_node_neg_build(struct cgl_node_neg_t* self, struct cgl_compile_t* compile);

void cgl_node_id_head(struct cgl_node_id_t* self, struct cgl_compile_t* compile);

void cgl_node_id_body(struct cgl_node_id_t* self, struct cgl_compile_t* compile);

unsigned long cgl_node_id_size(struct cgl_node_id_t* self, struct cgl_compile_t* compile);

void cgl_node_id_build(struct cgl_node_id_t* self, struct cgl_compile_t* compile);

void cgl_node_sum_head(struct cgl_node_sum_t* self, struct cgl_compile_t* compile);

void cgl_node_sum_body(struct cgl_node_sum_t* self, struct cgl_compile_t* compile);

unsigned long cgl_node_sum_size(struct cgl_node_sum_t* self, struct cgl_compile_t* compile);

void cgl_node_sum_build(struct cgl_node_sub_t* self, struct cgl_compile_t* compile);

void cgl_node_sub_head(struct cgl_node_sub_t* self, struct cgl_compile_t* compile);

void cgl_node_sub_body(struct cgl_node_sub_t* self, struct cgl_compile_t* compile);

unsigned long cgl_node_sub_size(struct cgl_node_sub_t* self, struct cgl_compile_t* compile);

void cgl_node_sub_build(struct cgl_node_sub_t* self, struct cgl_compile_t* compile);

void cgl_node_mul_head(struct cgl_node_mul_t* self, struct cgl_compile_t* compile);

void cgl_node_mul_body(struct cgl_node_mul_t* self, struct cgl_compile_t* compile);

unsigned long cgl_node_mul_size(struct cgl_node_mul_t* self, struct cgl_compile_t* compile);

void cgl_node_mul_build(struct cgl_node_mul_t* self, struct cgl_compile_t* compile);

void cgl_node_div_head(struct cgl_node_div_t* self, struct cgl_compile_t* compile);

void cgl_node_div_body(struct cgl_node_div_t* self, struct cgl_compile_t* compile);

unsigned long cgl_node_div_size(struct cgl_node_div_t* self, struct cgl_compile_t* compile);

void cgl_node_div_build(struct cgl_node_div_t* self, struct cgl_compile_t* compile);

void cgl_node_eq_head(struct cgl_node_eq_t* self, struct cgl_compile_t* compile) ;

void cgl_node_eq_body(struct cgl_node_eq_t* self, struct cgl_compile_t* compile) ;

unsigned long cgl_node_eq_size(struct cgl_node_eq_t* self, struct cgl_compile_t* compile) ;

void cgl_node_eq_build(struct cgl_node_eq_t* self, struct cgl_compile_t* compile);

void cgl_node_if_head(struct cgl_node_if_t* self, struct cgl_compile_t* compile);

void cgl_node_if_body(struct cgl_node_if_t* self, struct cgl_compile_t* compile);

unsigned long cgl_node_if_size(struct cgl_node_if_t* self, struct cgl_compile_t* compile);

void cgl_node_if_build(struct cgl_node_if_t* self, struct cgl_compile_t* compile);

#endif
