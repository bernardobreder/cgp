#ifndef CGL_SYNTAX_H
#define CGL_SYNTAX_H

#include "lexical.h"
#include "node.h"

struct cgl_syntax_t {
	const unsigned char* text;
	struct lng_token_t* tokens;
	struct lng_token_t* token;
	unsigned int token_count;
	unsigned char test;
	unsigned char* memory;
	unsigned int memory_size;
};

struct cgl_node_unit_t* cgl_syntax_unit(struct cgl_syntax_t* self);

struct cgl_syntax_t* cgl_syntax_new(const unsigned char* text);

void cgl_syntax_free(struct cgl_syntax_t* self);

struct cgl_node_unit_t* cgl_syntax_execute(struct cgl_syntax_t* self);

struct cgl_node_package_t* cgl_syntax_package(struct cgl_syntax_t* self);

struct cgl_node_import_t* cgl_syntax_imports(struct cgl_syntax_t* self);

struct cgl_node_import_t* cgl_syntax_import(struct cgl_syntax_t* self);

struct cgl_node_class_t* cgl_syntax_class(struct cgl_syntax_t* self);

struct cgl_node_t* cgl_syntax_field(struct cgl_syntax_t* self);

struct cgl_node_t* cgl_syntax_method(struct cgl_syntax_t* self);

struct cgl_node_param_t* cgl_syntax_parameters(struct cgl_syntax_t* self);

struct cgl_node_command_t* cgl_syntax_command(struct cgl_syntax_t* self);

struct cgl_node_type_t* cgl_syntax_type(struct cgl_syntax_t* self);

struct cgl_node_block_t* cgl_syntax_block(struct cgl_syntax_t* self);

struct cgl_node_t* cgl_syntax_if(struct cgl_syntax_t* self);

struct cgl_node_t* cgl_syntax_while(struct cgl_syntax_t* self);

struct cgl_node_t* cgl_syntax_repeat(struct cgl_syntax_t* self);

struct cgl_node_t* cgl_syntax_for(struct cgl_syntax_t* self);

struct cgl_node_t* cgl_syntax_declare(struct cgl_syntax_t* self);

struct cgl_node_expression_t* cgl_syntax_expression(struct cgl_syntax_t* self);

struct cgl_node_value_t* cgl_syntax_expr(struct cgl_syntax_t* self);

struct cgl_node_value_t* cgl_syntax_ternary(struct cgl_syntax_t* self);

struct cgl_node_value_t* cgl_syntax_and(struct cgl_syntax_t* self);

struct cgl_node_value_t* cgl_syntax_or(struct cgl_syntax_t* self);

struct cgl_node_value_t* cgl_syntax_compare(struct cgl_syntax_t* self);

struct cgl_node_value_t* cgl_syntax_sum(struct cgl_syntax_t* self);

struct cgl_node_value_t* cgl_syntax_mul(struct cgl_syntax_t* self);

struct cgl_node_value_t* cgl_syntax_unary(struct cgl_syntax_t* self);

struct cgl_node_value_t* cgl_syntax_literal(struct cgl_syntax_t* self);

void cgl_syntax_test();

#endif
