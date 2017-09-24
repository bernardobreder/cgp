#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "lexical.h"
#include "syntax.h"
#include "node.h"
#include "cg_io.h"

#define cgl_syntax_can(SELF, TYPE) (((SELF)->token++)->type == (TYPE))
#define cgl_syntax_is(SELF, TYPE) ((SELF)->token->type == (TYPE))
#define cgl_syntax_look(SELF, INDEX, TYPE) (((SELF)->token + (INDEX) < (SELF)->tokens + (SELF)->token_count) && (((SELF)->token + (INDEX))->type == (TYPE)))
#define cgl_syntax_get(SELF) ((SELF)->token)
#define cgl_syntax_next(SELF) ((SELF)->token++)

unsigned char cgl_syntax_unit_test(struct cgl_syntax_t* self);

struct cgl_node_unit_t* cgl_syntax_unit_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_package_test(struct cgl_syntax_t* self);

struct cgl_node_package_t* cgl_syntax_package_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_imports_test(struct cgl_syntax_t* self);

struct cgl_node_import_t* cgl_syntax_imports_build(struct cgl_syntax_t* self, unsigned int* import_count);

unsigned char cgl_syntax_class_test(struct cgl_syntax_t* self);

struct cgl_node_class_t* cgl_syntax_class_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_type_is(struct cgl_syntax_t* self);

unsigned char cgl_syntax_type_test(struct cgl_syntax_t* self);

struct cgl_node_type_t* cgl_syntax_type_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_parameters_test(struct cgl_syntax_t* self);

struct cgl_node_param_t* cgl_syntax_parameters_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_command_test(struct cgl_syntax_t* self);

struct cgl_node_command_t* cgl_syntax_command_build(struct cgl_syntax_t* self);

struct cgl_node_block_t* cgl_syntax_block_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_block_test(struct cgl_syntax_t* self);

unsigned char cgl_syntax_expr_test(struct cgl_syntax_t* self);

struct cgl_node_value_t* cgl_syntax_expr_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_return_test(struct cgl_syntax_t* self);

struct cgl_node_return_t* cgl_syntax_return_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_if_test(struct cgl_syntax_t* self);

struct cgl_node_if_t* cgl_syntax_if_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_while_test(struct cgl_syntax_t* self);

struct cgl_node_while_t* cgl_syntax_while_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_repeat_test(struct cgl_syntax_t* self);

struct cgl_node_repeat_t* cgl_syntax_repeat_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_for_test(struct cgl_syntax_t* self);

struct cgl_node_command_t* cgl_syntax_for_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_declare_is(struct cgl_syntax_t* self);

unsigned char cgl_syntax_declare_test(struct cgl_syntax_t* self);

struct cgl_node_declare_t* cgl_syntax_declare_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_expression_test(struct cgl_syntax_t* self);

struct cgl_node_expression_t* cgl_syntax_expression_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_ternary_test(struct cgl_syntax_t* self);

struct cgl_node_value_t* cgl_syntax_ternary_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_and_test(struct cgl_syntax_t* self);

struct cgl_node_value_t* cgl_syntax_and_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_or_test(struct cgl_syntax_t* self);

struct cgl_node_value_t* cgl_syntax_or_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_compare_test(struct cgl_syntax_t* self);

struct cgl_node_value_t* cgl_syntax_compare_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_sum_test(struct cgl_syntax_t* self);

struct cgl_node_value_t* cgl_syntax_sum_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_mul_test(struct cgl_syntax_t* self);

struct cgl_node_value_t* cgl_syntax_mul_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_unary_test(struct cgl_syntax_t* self);

struct cgl_node_value_t* cgl_syntax_unary_build(struct cgl_syntax_t* self);

unsigned char cgl_syntax_literal_test(struct cgl_syntax_t* self);

struct cgl_node_value_t* cgl_syntax_literal_build(struct cgl_syntax_t* self);

struct cgl_node_vtable_t cgl_node_unit_vtable = { //
	CGL_NODE_UNIT_TYPE, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_unit_head, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_unit_body, //
	(unsigned long (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_unit_size, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_unit_build //
};

struct cgl_node_vtable_t cgl_node_package_vtable = { //
	CGL_NODE_PACKAGE_TYPE, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_package_head, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_package_body, //
	(unsigned long (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_package_size, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_package_build //
};

struct cgl_node_vtable_t cgl_node_import_vtable = { //
	CGL_NODE_IMPORT_TYPE, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_import_head, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_import_body, //
	(unsigned long (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_import_size, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_import_build //
};

struct cgl_node_vtable_t cgl_node_class_vtable = { //
	CGL_NODE_CLASS_TYPE, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_class_head, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_class_body, //
	(unsigned long (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_class_size, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_class_build //
};

struct cgl_node_vtable_t cgl_node_field_vtable = { //
	CGL_NODE_FIELD_TYPE, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_field_head, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_field_body, //
	(unsigned long (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_field_size, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_field_build //
};

struct cgl_node_vtable_t cgl_node_method_vtable = { //
	CGL_NODE_METHOD_TYPE, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_method_head, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_method_body, //
	(unsigned long (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_method_size, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_method_build //
};

struct cgl_node_vtable_t cgl_node_type_int_vtable = { //
	CGL_NODE_INT_TYPE_TYPE, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_type_int_head, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_type_int_body, //
	(unsigned long (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_type_int_size, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_type_int_build //
};

struct cgl_node_vtable_t cgl_node_param_vtable = { //
	CGL_NODE_PARAMETER_TYPE, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_param_head, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_param_body, //
	(unsigned long (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_param_size, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_param_build //
};

struct cgl_node_vtable_t cgl_node_expression_vtable = { //
	CGL_NODE_EXPRESSION_TYPE, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_expression_head, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_expression_body, //
	(unsigned long (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_expression_size, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_expression_build //
};

struct cgl_node_vtable_t cgl_node_return_vtable = { //
	CGL_NODE_RETURN_TYPE, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_return_head, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_return_body, //
	(unsigned long (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_return_size, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_return_build //
};

struct cgl_node_vtable_t cgl_node_block_vtable = { //
	CGL_NODE_BLOCK_TYPE, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_block_head, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_block_body, //
	(unsigned long (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_block_size, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_block_build //
};

struct cgl_node_vtable_t cgl_node_int_vtable = { //
	CGL_NODE_INT_TYPE, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_int_head, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_int_body, //
	(unsigned long (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_int_size, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_int_build //
};

struct cgl_node_vtable_t cgl_node_neg_vtable = { //
	CGL_NODE_NEG_TYPE, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_neg_head, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_neg_body, //
	(unsigned long (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_neg_size, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_neg_build //
};

struct cgl_node_vtable_t cgl_node_sum_vtable = { //
	CGL_NODE_SUM_TYPE, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_sum_head, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_sum_body, //
	(unsigned long (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_sum_size, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_sum_build //
};


struct cgl_node_vtable_t cgl_node_sub_vtable = { //
	CGL_NODE_SUB_TYPE, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_sub_head, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_sub_body, //
	(unsigned long (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_sub_size, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_sub_build //
};

struct cgl_node_vtable_t cgl_node_mul_vtable = { //
	CGL_NODE_MUL_TYPE, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_mul_head, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_mul_body, //
	(unsigned long (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_mul_size, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_mul_build //
};

struct cgl_node_vtable_t cgl_node_div_vtable = { //
	CGL_NODE_DIV_TYPE, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_div_head, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_div_body, //
	(unsigned long (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_div_size, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_div_build //
};

struct cgl_node_vtable_t cgl_node_eq_vtable = { //
	CGL_NODE_EQ_TYPE, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_eq_head, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_eq_body, //
	(unsigned long (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_eq_size, //
	(void (*)(struct cgl_node_t*, struct cgl_compile_t*)) cgl_node_eq_build //
};

struct cgl_node_unit_t* cgl_syntax_unit(struct cgl_syntax_t* self) {
	self->test = 1;
	self->memory_size = 0;
	unsigned char error = cgl_syntax_unit_test(self);
	if (error) {
		return 0;
	}
	self->test = 0;
	self->token = self->tokens;
	assert(self->memory_size);
	unsigned char* memory = (unsigned char*) calloc(self->memory_size, 1);
	if (!memory) {
		return 0;
	}
	self->memory = memory;
	struct cgl_node_unit_t* unit = cgl_syntax_unit_build(self);
	//    unsigned long delta = self->memory - memory;
	assert(self->memory == memory + self->memory_size);
	assert((void*)memory == (void*)unit);
	return unit;
}

struct cgl_syntax_t* cgl_syntax_new(const unsigned char* text) {
	struct cgl_syntax_t* self = (struct cgl_syntax_t*) calloc(1, sizeof(struct cgl_syntax_t));
	if (!self) {
		return 0;
	}
	self->tokens = self->token = cgl_lexical(text, &self->token_count);
	if (!self->tokens) {
		free(self);
		return 0;
	}
	self->text = text;
	self->test = 1;
	return self;
}

void cgl_syntax_free(struct cgl_syntax_t* self) {
	free(self->tokens);
	free(self);
}

unsigned char cgl_syntax_id_test(struct cgl_syntax_t* self) {
	if (!cgl_syntax_is(self, CGL_LEXICAL_ID)) {
		return 1;
	}
	cgl_syntax_next(self);
	return 0;
}

unsigned char cgl_syntax_ids_test(struct cgl_syntax_t* self) {
	for (;;) {
		if (!cgl_syntax_is(self, CGL_LEXICAL_ID)) {
			return 1;
		}
		cgl_syntax_next(self);
		if (!cgl_syntax_is(self, '.')) {
			break;
		}
		cgl_syntax_next(self);
	}
	return 0;
}

struct lng_token_t* cgl_syntax_ids_build(struct cgl_syntax_t* self) {
	struct lng_token_t* token = cgl_syntax_next(self);
	while (cgl_syntax_is(self, '.')) {
		cgl_syntax_next(self);
		token = cgl_syntax_next(self);
	}
	return token;
}

unsigned char cgl_syntax_unit_test(struct cgl_syntax_t* self) {
	self->memory_size += sizeof(struct cgl_node_unit_t);
	if (cgl_syntax_package_test(self)) {
		return 1;
	}
	if (cgl_syntax_imports_test(self)) {
		return 1;
	}
	if (cgl_syntax_class_test(self)) {
		return 1;
	}
	if (!cgl_syntax_is(self, CGL_LEXICAL_EOF)) {
		return 1;
	}
	return 0;
}

struct cgl_node_unit_t* cgl_syntax_unit_build(struct cgl_syntax_t* self) {
	struct cgl_node_unit_t* node = (struct cgl_node_unit_t*) self->memory;
	self->memory += sizeof(struct cgl_node_unit_t);
	node->vtable = &cgl_node_unit_vtable;
	struct cgl_node_package_t* package = cgl_syntax_package_build(self);
	struct cgl_node_import_t* imports = cgl_syntax_imports_build(self, &node->import_count);
	struct cgl_node_class_t* clazz = cgl_syntax_class_build(self);
	node->package = package;
	node->imports = imports;
	node->clazz = clazz;
	return node;
}

unsigned char cgl_syntax_package_test(struct cgl_syntax_t* self) {
	self->memory_size += sizeof(struct cgl_node_package_t);
	if (!cgl_syntax_is(self, CGL_LEXICAL_PACKAGE_WORD)) {
		return 1;
	}
	cgl_syntax_next(self);
	if (cgl_syntax_ids_test(self)) {
		return 1;
	}
	return 0;
}

struct cgl_node_package_t* cgl_syntax_package_build(struct cgl_syntax_t* self) {
	cgl_syntax_next(self);
	struct lng_token_t* token = cgl_syntax_ids_build(self);
	struct cgl_node_package_t* node = (struct cgl_node_package_t*) self->memory;
	self->memory += sizeof(struct cgl_node_package_t);
	node->vtable = &cgl_node_package_vtable;
	node->name = token;
	return node;
}

static unsigned int cgl_syntax_imports_count(struct cgl_syntax_t* self) {
	unsigned int size = 0;
	struct lng_token_t* token = self->token;
	while (!cgl_syntax_is(self, CGL_LEXICAL_CLASS_WORD)) {
		if (cgl_syntax_is(self, CGL_LEXICAL_IMPORT_WORD)) {
			size++;
		}
		cgl_syntax_next(self);
	}
	self->token = token;
	return size;
}

unsigned char cgl_syntax_imports_test(struct cgl_syntax_t* self) {
	unsigned int size = 0;
	while (cgl_syntax_is(self, CGL_LEXICAL_IMPORT_WORD)) {
		cgl_syntax_next(self);
		if (cgl_syntax_ids_test(self)) {
			return 1;
		}
		size++;
	}
	self->memory_size += (size + 1) * sizeof(struct cgl_node_import_t);
	return 0;
}

struct cgl_node_import_t* cgl_syntax_imports_build(struct cgl_syntax_t* self, unsigned int* import_count) {
	struct cgl_node_import_t* array = (struct cgl_node_import_t*) self->memory;
	unsigned int n, size = cgl_syntax_imports_count(self);
	self->memory += (size + 1) * sizeof(struct cgl_node_import_t);
	for (n = 0; n < size; n++) {
		cgl_syntax_next(self);
		struct lng_token_t* token = cgl_syntax_ids_build(self);
		struct cgl_node_import_t* node = array + n;
		node->vtable = &cgl_node_import_vtable;
		node->name = token;
	}
	struct cgl_node_import_t* node = array + size;
	node->vtable = 0;
	*import_count = size;
	return array;
}

unsigned char cgl_syntax_class_test(struct cgl_syntax_t* self) {
	if (!cgl_syntax_is(self, CGL_LEXICAL_CLASS_WORD)) {
		return 1;
	}
	cgl_syntax_next(self);
	self->memory_size += sizeof(struct cgl_node_class_t);
	if (!cgl_syntax_is(self, CGL_LEXICAL_ID)) {
		return 1;
	}
	cgl_syntax_next(self);
	if (!cgl_syntax_is(self, CGL_LEXICAL_DO_WORD)) {
		return 1;
	}
	cgl_syntax_next(self);
	while (!cgl_syntax_is(self, CGL_LEXICAL_END_WORD) && !cgl_syntax_is(self, CGL_LEXICAL_EOF)) {
        uint8 is_static = 0;
        for (;;) {
            if (cgl_syntax_is(self, CGL_LEXICAL_STATIC_WORD) && !is_static) {
                cgl_syntax_next(self);
                is_static = 1;
            } else {
                break;
            }
        }
		if (cgl_syntax_type_test(self)) {
			return 1;
		}
		if (!cgl_syntax_is(self, CGL_LEXICAL_ID)) {
			return 1;
		}
		cgl_syntax_next(self);
		if (cgl_syntax_is(self, '(')) {
			if (cgl_syntax_parameters_test(self)) {
				return 1;
			}
			if (cgl_syntax_block_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct cgl_node_method_t);
		} else {
			self->memory_size += sizeof(struct cgl_node_field_t);
		}
	}
	self->memory_size += sizeof(struct cgl_node_field_t);
	self->memory_size += sizeof(struct cgl_node_method_t);
	if (!cgl_syntax_is(self, CGL_LEXICAL_END_WORD)) {
		return 1;
	}
	cgl_syntax_next(self);
	return 0;
}

static void cgl_syntax_class_count(struct cgl_syntax_t* self, unsigned int* fields, unsigned int* methods) {
	struct lng_token_t* token = self->token;
	*fields = 0;
	*methods = 0;
	while (!cgl_syntax_is(self, CGL_LEXICAL_END_WORD)) {
        for (;;) {
            if (cgl_syntax_is(self, CGL_LEXICAL_STATIC_WORD)) {
                cgl_syntax_next(self);
            } else {
                break;
            }
        }
		cgl_syntax_next(self);
		cgl_syntax_next(self);
		if (cgl_syntax_is(self, '(')) {
			cgl_syntax_next(self);
			while (!cgl_syntax_is(self, CGL_LEXICAL_DO_WORD)) {
				cgl_syntax_next(self);
			}
			cgl_syntax_next(self);
			int counter = 1;
			while (counter != 0) {
				if (cgl_syntax_is(self, CGL_LEXICAL_END_WORD)) {
					counter--;
				} else if (cgl_syntax_is(self, CGL_LEXICAL_DO_WORD)) {
					counter++;
				}
				cgl_syntax_next(self);
			}
			(*methods)++;
		} else {
			(*fields)++;
		}
	}
	self->token = token;
}

struct cgl_node_class_t* cgl_syntax_class_build(struct cgl_syntax_t* self) {
	cgl_syntax_next(self);
	struct cgl_node_class_t* node = (struct cgl_node_class_t*) self->memory;
	self->memory += sizeof(struct cgl_node_class_t);
	node->vtable = &cgl_node_class_vtable;
	node->name = cgl_syntax_next(self);
	cgl_syntax_next(self);
	cgl_syntax_class_count(self, &node->field_count, &node->method_count);
	node->fields = (struct cgl_node_field_t*) self->memory;
	self->memory += (node->field_count + 1) * sizeof(struct cgl_node_field_t);
	node->methods = (struct cgl_node_method_t*) self->memory;
	self->memory += (node->method_count + 1) * sizeof(struct cgl_node_method_t);
	unsigned int field_index = 0, method_index = 0;
	unsigned int n, size = node->field_count + node->method_count;
	for (n = 0; n < size; n++) {
        uint8 is_static = 0;
        for (;;) {
            if (cgl_syntax_is(self, CGL_LEXICAL_STATIC_WORD) && !is_static) {
                cgl_syntax_next(self);
                is_static = 1;
            } else {
                break;
            }
        }
		struct cgl_node_type_t* type = cgl_syntax_type_build(self);
		struct lng_token_t* name = cgl_syntax_next(self);
		if (cgl_syntax_is(self, '(')) {
			struct cgl_node_param_t* params = cgl_syntax_parameters_build(self);
			struct cgl_node_block_t* cmd = cgl_syntax_block_build(self);
			struct cgl_node_method_t* method = node->methods + method_index++;
			method->vtable = &cgl_node_method_vtable;
			method->clazz = node;
			method->is_static = 1;
			method->name = name;
			method->type = type;
			method->parameters = params;
			method->cmd = cmd;
		} else {
			struct cgl_node_field_t* field = node->fields + field_index++;
			field->vtable = &cgl_node_field_vtable;
			field->name = name;
		}
	}
	return node;
}

unsigned char cgl_syntax_type_is(struct cgl_syntax_t* self) {
	if (cgl_syntax_is(self, CGL_LEXICAL_INT_WORD)) {
		return 1;
	} else {
		return 0;
	}
}

unsigned char cgl_syntax_type_test(struct cgl_syntax_t* self) {
	if (cgl_syntax_is(self, CGL_LEXICAL_INT_WORD)) {
		cgl_syntax_next(self);
		self->memory_size += sizeof(struct cgl_node_type_int_t);
		return 0;
	} else {
		return 1;
	}
}

struct cgl_node_type_t* cgl_syntax_type_build(struct cgl_syntax_t* self) {
	if (cgl_syntax_is(self, CGL_LEXICAL_INT_WORD)) {
		struct lng_token_t* token = cgl_syntax_next(self);
		struct cgl_node_type_int_t* node = (struct cgl_node_type_int_t*) self->memory;
		self->memory += sizeof(struct cgl_node_type_int_t);
		node->vtable = &cgl_node_type_int_vtable;
		node->token = token;
		return (struct cgl_node_type_t*) node;
	} else {
		assert(0);
		return 0;
	}
}

unsigned int cgl_syntax_parameters_count(struct cgl_syntax_t* self) {
	struct lng_token_t* token = self->token;
	cgl_syntax_next(self);
	if (cgl_syntax_is(self, ')')) {
		self->token = token;
		return 0;
	}
	unsigned int size = 1;
	while (!cgl_syntax_is(self, ')') && !cgl_syntax_is(self, CGL_LEXICAL_EOF)) {
		if (cgl_syntax_is(self, ',')) {
			size++;
		}
		cgl_syntax_next(self);
	}
	self->token = token;
	return size;
}

unsigned char cgl_syntax_parameters_test(struct cgl_syntax_t* self) {
	if (!cgl_syntax_is(self, '(')) {
		return 1;
	}
	cgl_syntax_next(self);
	while (!cgl_syntax_is(self, ')') && !cgl_syntax_is(self, CGL_LEXICAL_EOF)) {
		if (cgl_syntax_type_test(self)) {
			return 1;
		}
		if (cgl_syntax_id_test(self)) {
			return 1;
		}
		self->memory_size += sizeof(struct cgl_node_param_t);
		if (!cgl_syntax_is(self, ',')) {
			break;
		}
		cgl_syntax_next(self);
	}
	if (!cgl_syntax_is(self, ')')) {
		return 1;
	}
	cgl_syntax_next(self);
	self->memory_size += sizeof(struct cgl_node_param_t);
	return 0;
}

struct cgl_node_param_t* cgl_syntax_parameters_build(struct cgl_syntax_t* self) {
	unsigned int n, size = cgl_syntax_parameters_count(self);
	cgl_syntax_next(self);
	struct cgl_node_param_t* array = (struct cgl_node_param_t*) self->memory;
	self->memory += (size + 1) * sizeof(struct cgl_node_param_t);
	for (n = 0; n < size; n++) {
		struct cgl_node_type_t* type = cgl_syntax_type_build(self);
		struct lng_token_t* name = cgl_syntax_next(self);
		struct cgl_node_param_t* node = array + n;
		node->vtable = &cgl_node_param_vtable;
		node->type = type;
		node->name = name;
		if (n != size - 1) {
			cgl_syntax_next(self);
		}
	}
	cgl_syntax_next(self);
	return array;
}

unsigned char cgl_syntax_command_test(struct cgl_syntax_t* self) {
	if (cgl_syntax_is(self, ';')) {
		cgl_syntax_next(self);
		return 0;
	} else if (cgl_syntax_is(self, CGL_LEXICAL_DO_WORD)) {
		return cgl_syntax_block_test(self);
	} else if (cgl_syntax_is(self, CGL_LEXICAL_IF_WORD)) {
		return cgl_syntax_if_test(self);
	} else if (cgl_syntax_is(self, CGL_LEXICAL_WHILE_WORD)) {
		return cgl_syntax_while_test(self);
	} else if (cgl_syntax_is(self, CGL_LEXICAL_REPEAT_WORD)) {
		return cgl_syntax_repeat_test(self);
	} else if (cgl_syntax_is(self, CGL_LEXICAL_RETURN_WORD)) {
		return cgl_syntax_return_test(self);
	} else if (cgl_syntax_is(self, CGL_LEXICAL_FOR_WORD)) {
		return cgl_syntax_for_test(self);
	} else if (cgl_syntax_declare_is(self)) {
		return cgl_syntax_declare_test(self);
	} else {
		return cgl_syntax_expression_test(self);
	}
}

struct cgl_node_command_t* cgl_syntax_command_build(struct cgl_syntax_t* self) {
	if (cgl_syntax_is(self, ';')) {
		cgl_syntax_next(self);
		return 0;
	} else if (cgl_syntax_is(self, CGL_LEXICAL_DO_WORD)) {
		return (struct cgl_node_command_t*) cgl_syntax_block_build(self);
	} else if (cgl_syntax_is(self, CGL_LEXICAL_IF_WORD)) {
		return (struct cgl_node_command_t*) cgl_syntax_if_build(self);
	} else if (cgl_syntax_is(self, CGL_LEXICAL_WHILE_WORD)) {
		return (struct cgl_node_command_t*) cgl_syntax_while_build(self);
	} else if (cgl_syntax_is(self, CGL_LEXICAL_REPEAT_WORD)) {
		return (struct cgl_node_command_t*) cgl_syntax_repeat_build(self);
	} else if (cgl_syntax_is(self, CGL_LEXICAL_RETURN_WORD)) {
		return (struct cgl_node_command_t*) cgl_syntax_return_build(self);
	} else if (cgl_syntax_is(self, CGL_LEXICAL_FOR_WORD)) {
		return (struct cgl_node_command_t*) cgl_syntax_for_build(self);
	} else if (cgl_syntax_declare_is(self)) {
		return (struct cgl_node_command_t*) cgl_syntax_declare_build(self);
	} else {
		return (struct cgl_node_command_t*) cgl_syntax_expression_build(self);
	}
}

unsigned char cgl_syntax_block_test(struct cgl_syntax_t* self) {
	if (!cgl_syntax_is(self, CGL_LEXICAL_DO_WORD)) {
		return 1;
	}
	cgl_syntax_next(self);
	if (cgl_syntax_is(self, CGL_LEXICAL_END_WORD)) {
		cgl_syntax_next(self);
		return 0;
	}
	self->memory_size += sizeof(struct cgl_node_block_t);
	do {
		if (cgl_syntax_command_test(self)) {
			return 1;
		}
		self->memory_size += sizeof(struct cgl_node_command_t*);
	} while (!cgl_syntax_is(self, CGL_LEXICAL_END_WORD) && !cgl_syntax_is(self, CGL_LEXICAL_EOF));
	if (!cgl_syntax_is(self, CGL_LEXICAL_END_WORD)) {
		return 1;
	}
	cgl_syntax_next(self);
	self->memory_size += sizeof(struct cgl_node_command_t*);
	return 0;
}

unsigned int cgl_syntax_block_count(struct cgl_syntax_t* self) {
	struct lng_token_t* token = self->token;
	unsigned int memory_size = self->memory_size;
	unsigned int size = 0;
	while (!cgl_syntax_is(self, CGL_LEXICAL_END_WORD)) {
		cgl_syntax_command_test(self);
		size++;
	}
	self->memory_size = memory_size;
	self->token = token;
	return size;
}

struct cgl_node_block_t* cgl_syntax_block_build(struct cgl_syntax_t* self) {
	cgl_syntax_next(self);
	if (cgl_syntax_is(self, CGL_LEXICAL_END_WORD)) {
		cgl_syntax_next(self);
		static struct cgl_node_block_t node = { &cgl_node_block_vtable, 0 };
		return &node;
	}
	unsigned int n, size = cgl_syntax_block_count(self);
	struct cgl_node_command_t** array = (struct cgl_node_command_t**) self->memory;
	self->memory += (size + 1) * sizeof(struct cgl_node_command_t*);
	for (n = 0; n < size; n++) {
		struct cgl_node_command_t* node = cgl_syntax_command_build(self);
		if (node) {
			array[n] = node;
		}
	}
	cgl_syntax_next(self);
	struct cgl_node_block_t* node = (struct cgl_node_block_t*) self->memory;
	self->memory += sizeof(struct cgl_node_block_t);
	node->vtable = &cgl_node_block_vtable;
	node->cmds = array;
	return node;
}

unsigned char cgl_syntax_return_test(struct cgl_syntax_t* self) {
	self->memory_size += sizeof(struct cgl_node_return_t);
	if (!cgl_syntax_is(self, CGL_LEXICAL_RETURN_WORD)) {
		return 1;
	}
	cgl_syntax_next(self);
	if (cgl_syntax_expr_test(self)) {
		return 1;
	}
	return 0;
}

struct cgl_node_return_t* cgl_syntax_return_build(struct cgl_syntax_t* self) {
	struct cgl_node_return_t* node = (struct cgl_node_return_t*) self->memory;
	self->memory += sizeof(struct cgl_node_return_t);
	node->vtable = &cgl_node_return_vtable;
	cgl_syntax_next(self);
	node->left = cgl_syntax_expr_build(self);
	return node;
}

unsigned char cgl_syntax_if_test(struct cgl_syntax_t* self) {
	self->memory_size += sizeof(struct cgl_node_if_t);
	if (!cgl_syntax_is(self, CGL_LEXICAL_IF_WORD)) {
		return 1;
	}
	cgl_syntax_next(self);
	if (cgl_syntax_expr_test(self)) {
		return 1;
	}
	if (cgl_syntax_command_test(self)) {
		return 1;
	}
	return 0;
}

struct cgl_node_if_t* cgl_syntax_if_build(struct cgl_syntax_t* self) {
	struct cgl_node_if_t* node = (struct cgl_node_if_t*) self->memory;
	self->memory += sizeof(struct cgl_node_if_t);
	cgl_syntax_next(self);
	node->cond = cgl_syntax_expr_build(self);
	node->cmd = cgl_syntax_command_build(self);
	return node;
}

unsigned char cgl_syntax_while_test(struct cgl_syntax_t* self) {
	self->memory_size += sizeof(struct cgl_node_while_t);
	if (!cgl_syntax_is(self, CGL_LEXICAL_WHILE_WORD)) {
		return 1;
	}
	cgl_syntax_next(self);
	if (cgl_syntax_expr_test(self)) {
		return 1;
	}
	if (cgl_syntax_command_test(self)) {
		return 1;
	}
	return 0;
}

struct cgl_node_while_t* cgl_syntax_while_build(struct cgl_syntax_t* self) {
	struct cgl_node_while_t* node = (struct cgl_node_while_t*) self->memory;
	self->memory += sizeof(struct cgl_node_while_t);
	cgl_syntax_next(self);
	node->cond = cgl_syntax_expr_build(self);
	node->cmd = cgl_syntax_command_build(self);
	return node;
}

unsigned char cgl_syntax_repeat_test(struct cgl_syntax_t* self) {
	self->memory_size += sizeof(struct cgl_node_while_t);
	if (!cgl_syntax_is(self, CGL_LEXICAL_REPEAT_WORD)) {
		return 1;
	}
	cgl_syntax_next(self);
	if (cgl_syntax_command_test(self)) {
		return 1;
	}
	if (!cgl_syntax_is(self, CGL_LEXICAL_WHILE_WORD)) {
		return 1;
	}
	cgl_syntax_next(self);
	if (cgl_syntax_expr_test(self)) {
		return 1;
	}
	return 0;
}

struct cgl_node_repeat_t* cgl_syntax_repeat_build(struct cgl_syntax_t* self) {
	struct cgl_node_repeat_t* node = (struct cgl_node_repeat_t*) self->memory;
	self->memory += sizeof(struct cgl_node_repeat_t);
	cgl_syntax_next(self);
	node->cmd = cgl_syntax_command_build(self);
	cgl_syntax_next(self);
	node->cond = cgl_syntax_expr_build(self);
	return node;
}

unsigned char cgl_syntax_for_test(struct cgl_syntax_t* self) {
	if (!cgl_syntax_is(self, CGL_LEXICAL_FOR_WORD)) {
		return 1;
	}
	cgl_syntax_next(self);
	if (cgl_syntax_is(self, CGL_LEXICAL_ID)) {
		self->memory_size += sizeof(struct cgl_node_foreach_t);
		cgl_syntax_next(self);
		if (!cgl_syntax_is(self, '=')) {
			return 1;
		}
		cgl_syntax_next(self);
		if (cgl_syntax_expr_test(self)) {
			return 1;
		}
		if (!cgl_syntax_is(self, ',')) {
			return 1;
		}
		cgl_syntax_next(self);
		if (cgl_syntax_expr_test(self)) {
			return 1;
		}
		if (cgl_syntax_command_test(self)) {
			return 1;
		}
		return 0;
	} else if (cgl_syntax_is(self, '(')) {
		self->memory_size += sizeof(struct cgl_node_forn_t);
		cgl_syntax_next(self);
		if (!cgl_syntax_is(self,';')) {
			if (cgl_syntax_expr_test(self)) {
				return 1;
			}
		}
		if (!cgl_syntax_is(self,';')) {
			return 1;
		}
		cgl_syntax_next(self);
		if (!cgl_syntax_is(self,';')) {
			if (cgl_syntax_expr_test(self)) {
				return 1;
			}
		}
		if (!cgl_syntax_is(self,';')) {
			return 1;
		}
		cgl_syntax_next(self);
		if (!cgl_syntax_is(self,')')) {
			if (cgl_syntax_expr_test(self)) {
				return 1;
			}
		}
		if (!cgl_syntax_is(self,')')) {
			return 1;
		}
		cgl_syntax_next(self);
		if (cgl_syntax_command_test(self)) {
			return 1;
		}
		return 0;
	} else {
		return 1;
	}
}

struct cgl_node_command_t* cgl_syntax_for_build(struct cgl_syntax_t* self) {
	cgl_syntax_next(self);
	if (cgl_syntax_is(self, CGL_LEXICAL_ID)) {
		struct cgl_node_foreach_t* node = (struct cgl_node_foreach_t*) self->memory;
		self->memory += sizeof(struct cgl_node_foreach_t);
		node->name = cgl_syntax_next(self);
		cgl_syntax_next(self);
		node->begin = cgl_syntax_expr_build(self);
		cgl_syntax_next(self);
		node->end = cgl_syntax_expr_build(self);
		node->cmd = cgl_syntax_command_build(self);
		return (struct cgl_node_command_t*) node;
	} else {
		struct cgl_node_forn_t* node = (struct cgl_node_forn_t*) self->memory;
		self->memory += sizeof(struct cgl_node_forn_t);
		cgl_syntax_next(self);
		if (!cgl_syntax_is(self,';')) {
			node->init = cgl_syntax_expr_build(self);
		}
		cgl_syntax_next(self);
		if (!cgl_syntax_is(self,';')) {
			node->cond = cgl_syntax_expr_build(self);
		}
		cgl_syntax_next(self);
		if (!cgl_syntax_is(self,')')) {
			node->inc = cgl_syntax_expr_build(self);
		}
		cgl_syntax_next(self);
		node->cmd = cgl_syntax_command_build(self);
		return (struct cgl_node_command_t*) node;
	}
}

unsigned char cgl_syntax_declare_is(struct cgl_syntax_t* self) {
	int index = cgl_syntax_type_is(self);
	if (index <= 0) {
		return 0;
	}
	if (!cgl_syntax_look(self, index, CGL_LEXICAL_ID)) {
		return 0;
	}
	return 1;
}

unsigned char cgl_syntax_declare_test(struct cgl_syntax_t* self) {
	self->memory_size += sizeof(struct cgl_node_declare_t);
	if (cgl_syntax_type_test(self)) {
		return 1;
	}
	if (!cgl_syntax_can(self, CGL_LEXICAL_ID)) {
		return 1;
	}
	if (cgl_syntax_is(self, '=')) {
		cgl_syntax_next(self);
		if (cgl_syntax_expr_test(self)) {
			return 1;
		}
	}
	return 0;
}

struct cgl_node_declare_t* cgl_syntax_declare_build(struct cgl_syntax_t* self) {
	struct cgl_node_declare_t* node = (struct cgl_node_declare_t*) self->memory;
	self->memory += sizeof(struct cgl_node_declare_t);
	node->type = cgl_syntax_type_build(self);
	node->name = cgl_syntax_next(self);
	if (cgl_syntax_is(self, '=')) {
		cgl_syntax_next(self);
		node->value = cgl_syntax_expr_build(self);
	}
	return node;
}

unsigned char cgl_syntax_expression_test(struct cgl_syntax_t* self) {
	self->memory_size += sizeof(struct cgl_node_expression_t);
	if (cgl_syntax_expr_test(self)) {
		return 1;
	}
	return 0;
}

struct cgl_node_expression_t* cgl_syntax_expression_build(struct cgl_syntax_t* self) {
	struct cgl_node_expression_t* node = (struct cgl_node_expression_t*) self->memory;
	self->memory += sizeof(struct cgl_node_expression_t);
	node->vtable = &cgl_node_expression_vtable;
	node->left = cgl_syntax_expr_build(self);
	return node;
}

unsigned char cgl_syntax_expr_test(struct cgl_syntax_t* self) {
	return cgl_syntax_ternary_test(self);
}

struct cgl_node_value_t* cgl_syntax_expr_build(struct cgl_syntax_t* self) {
	return cgl_syntax_ternary_build(self);
}

unsigned char cgl_syntax_ternary_test(struct cgl_syntax_t* self) {
	if (cgl_syntax_and_test(self)) {
		return 1;
	}
	while (cgl_syntax_is(self, '?')) {
		cgl_syntax_next(self);
		if (cgl_syntax_and_test(self)) {
			return 1;
		}
		if (!cgl_syntax_is(self, ':')) {
			return 1;
		}
		cgl_syntax_next(self);
		if (cgl_syntax_and_test(self)) {
			return 1;
		}
		self->memory_size += sizeof(struct cgl_node_ternary_t);
	}
	return 0;
}

struct cgl_node_value_t* cgl_syntax_ternary_build(struct cgl_syntax_t* self) {
	struct cgl_node_value_t* left = cgl_syntax_and_build(self);
	while (cgl_syntax_is(self, '?')) {
		struct cgl_node_ternary_t* node = (struct cgl_node_ternary_t*) self->memory;
		self->memory += sizeof(struct cgl_node_ternary_t);
		node->left = left;
		cgl_syntax_next(self);
		node->right = cgl_syntax_and_build(self);
		cgl_syntax_next(self);
		node->center = cgl_syntax_and_build(self);
		left = (struct cgl_node_value_t*) node;
	}
	return left;
}

unsigned char cgl_syntax_and_test(struct cgl_syntax_t* self) {
	if (cgl_syntax_or_test(self)) {
		return 1;
	}
	while (cgl_syntax_is(self, CGL_LEXICAL_AND_WORD)) {
		cgl_syntax_next(self);
		if (cgl_syntax_or_test(self)) {
			return 1;
		}
		self->memory_size += sizeof(struct cgl_node_and_t);
	}
	return 0;
}

struct cgl_node_value_t* cgl_syntax_and_build(struct cgl_syntax_t* self) {
	struct cgl_node_value_t* left = cgl_syntax_or_build(self);
	while (cgl_syntax_is(self, CGL_LEXICAL_AND_WORD)) {
		cgl_syntax_next(self);
		struct cgl_node_and_t* node = (struct cgl_node_and_t*) self->memory;
		self->memory += sizeof(struct cgl_node_and_t);
		node->left = left;
		node->right = cgl_syntax_or_build(self);
		left = (struct cgl_node_value_t*) node;
	}
	return left;
}

unsigned char cgl_syntax_or_test(struct cgl_syntax_t* self) {
	if (cgl_syntax_compare_test(self)) {
		return 1;
	}
	while (cgl_syntax_is(self, CGL_LEXICAL_OR_WORD)) {
		cgl_syntax_next(self);
		if (cgl_syntax_compare_test(self)) {
			return 1;
		}
		self->memory_size += sizeof(struct cgl_node_or_t);
	}
	return 0;
}

struct cgl_node_value_t* cgl_syntax_or_build(struct cgl_syntax_t* self) {
	struct cgl_node_value_t* left = cgl_syntax_compare_build(self);
	while (cgl_syntax_is(self, CGL_LEXICAL_OR_WORD)) {
		cgl_syntax_next(self);
		struct cgl_node_or_t* node = (struct cgl_node_or_t*) self->memory;
		self->memory += sizeof(struct cgl_node_or_t);
		node->left = left;
		node->right = cgl_syntax_compare_build(self);
		left = (struct cgl_node_value_t*) node;
	}
	return left;
}

unsigned char cgl_syntax_compare_test(struct cgl_syntax_t* self) {
	if (cgl_syntax_sum_test(self)) {
		return 1;
	}
	for (;;) {
		if (cgl_syntax_is(self, CGL_LEXICAL_EQUAL_EQUAL)) {
			cgl_syntax_next(self);
			if (cgl_syntax_sum_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct cgl_node_eq_t);
		} else if (cgl_syntax_is(self, CGL_LEXICAL_DIFERENT)) {
			cgl_syntax_next(self);
			if (cgl_syntax_sum_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct cgl_node_neq_t);
		} else if (cgl_syntax_is(self, '<')) {
			cgl_syntax_next(self);
			if (cgl_syntax_sum_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct cgl_node_lt_t);
		} else if (cgl_syntax_is(self, CGL_LEXICAL_LOWER_EQUAL)) {
			cgl_syntax_next(self);
			if (cgl_syntax_sum_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct cgl_node_le_t);
		} else if (cgl_syntax_is(self, '>')) {
			cgl_syntax_next(self);
			if (cgl_syntax_sum_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct cgl_node_gt_t);
		} else if (cgl_syntax_is(self, CGL_LEXICAL_GREATER_EQUAL)) {
			cgl_syntax_next(self);
			if (cgl_syntax_sum_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct cgl_node_ge_t);
		} else {
			break;
		}
	}
	return 0;
}

struct cgl_node_value_t* cgl_syntax_compare_build(struct cgl_syntax_t* self) {
	struct cgl_node_value_t* left = cgl_syntax_sum_build(self);
	for (;;) {
		if (cgl_syntax_is(self, CGL_LEXICAL_EQUAL_EQUAL)) {
			cgl_syntax_next(self);
			struct cgl_node_eq_t* node = (struct cgl_node_eq_t*) self->memory;
			self->memory += sizeof(struct cgl_node_eq_t);
            node->vtable = &cgl_node_eq_vtable;
			node->left = left;
			node->right = cgl_syntax_sum_build(self);
			left = (struct cgl_node_value_t*) node;
		} else if (cgl_syntax_is(self, CGL_LEXICAL_DIFERENT)) {
			cgl_syntax_next(self);
			struct cgl_node_neq_t* node = (struct cgl_node_neq_t*) self->memory;
			self->memory += sizeof(struct cgl_node_neq_t);
			node->left = left;
			node->right = cgl_syntax_sum_build(self);
			left = (struct cgl_node_value_t*) node;
		} else if (cgl_syntax_is(self, '<')) {
			cgl_syntax_next(self);
			struct cgl_node_lt_t* node = (struct cgl_node_lt_t*) self->memory;
			self->memory += sizeof(struct cgl_node_lt_t);
			node->left = left;
			node->right = cgl_syntax_sum_build(self);
			left = (struct cgl_node_value_t*) node;
		} else if (cgl_syntax_is(self, CGL_LEXICAL_LOWER_EQUAL)) {
			cgl_syntax_next(self);
			struct cgl_node_le_t* node = (struct cgl_node_le_t*) self->memory;
			self->memory += sizeof(struct cgl_node_le_t);
			node->left = left;
			node->right = cgl_syntax_sum_build(self);
			left = (struct cgl_node_value_t*) node;
		} else if (cgl_syntax_is(self, '>')) {
			cgl_syntax_next(self);
			struct cgl_node_gt_t* node = (struct cgl_node_gt_t*) self->memory;
			self->memory += sizeof(struct cgl_node_gt_t);
			node->left = left;
			node->right = cgl_syntax_sum_build(self);
			left = (struct cgl_node_value_t*) node;
		} else if (cgl_syntax_is(self, CGL_LEXICAL_GREATER_EQUAL)) {
			cgl_syntax_next(self);
			struct cgl_node_ge_t* node = (struct cgl_node_ge_t*) self->memory;
			self->memory += sizeof(struct cgl_node_ge_t);
			node->left = left;
			node->right = cgl_syntax_sum_build(self);
			left = (struct cgl_node_value_t*) node;
		} else {
			break;
		}
	}
	return left;
}

unsigned char cgl_syntax_sum_test(struct cgl_syntax_t* self) {
	if (cgl_syntax_mul_test(self)) {
		return 1;
	}
	for (;;) {
		if (cgl_syntax_is(self, '+')) {
			cgl_syntax_next(self);
			if (cgl_syntax_mul_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct cgl_node_sum_t);
		} else if (cgl_syntax_is(self, '-')) {
			cgl_syntax_next(self);
			if (cgl_syntax_mul_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct cgl_node_sub_t);
		} else {
			break;
		}
	}
	return 0;
}

struct cgl_node_value_t* cgl_syntax_sum_build(struct cgl_syntax_t* self) {
	struct cgl_node_value_t* left = cgl_syntax_mul_build(self);
	for (;;) {
		if (cgl_syntax_is(self, '+')) {
			cgl_syntax_next(self);
			struct cgl_node_sum_t* node = (struct cgl_node_sum_t*) self->memory;
			self->memory += sizeof(struct cgl_node_sum_t);
			node->vtable = &cgl_node_sum_vtable;
			node->left = left;
			node->right = cgl_syntax_mul_build(self);
			left = (struct cgl_node_value_t*) node;
		} else if (cgl_syntax_is(self, '-')) {
			cgl_syntax_next(self);
			struct cgl_node_sub_t* node = (struct cgl_node_sub_t*) self->memory;
			self->memory += sizeof(struct cgl_node_sub_t);
			node->vtable = &cgl_node_sub_vtable;
			node->left = left;
			node->right = cgl_syntax_mul_build(self);
			left = (struct cgl_node_value_t*) node;
		} else {
			break;
		}
	}
	return left;
}

unsigned char cgl_syntax_mul_test(struct cgl_syntax_t* self) {
	if (cgl_syntax_unary_test(self)) {
		return 1;
	}
	for (;;) {
		if (cgl_syntax_is(self, '*')) {
			cgl_syntax_next(self);
			if (cgl_syntax_unary_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct cgl_node_mul_t);
		} else if (cgl_syntax_is(self, '/')) {
			cgl_syntax_next(self);
			if (cgl_syntax_unary_test(self)) {
				return 1;
			}
			self->memory_size += sizeof(struct cgl_node_div_t);
		} else {
			break;
		}
	}
	return 0;
}

struct cgl_node_value_t* cgl_syntax_mul_build(struct cgl_syntax_t* self) {
	struct cgl_node_value_t* left = cgl_syntax_unary_build(self);
	for (;;) {
		if (cgl_syntax_is(self, '*')) {
			cgl_syntax_next(self);
			struct cgl_node_mul_t* node = (struct cgl_node_mul_t*) self->memory;
			self->memory += sizeof(struct cgl_node_mul_t);
			node->vtable = &cgl_node_mul_vtable;
			node->left = left;
			node->right = cgl_syntax_unary_build(self);
			left = (struct cgl_node_value_t*) node;
		} else if (cgl_syntax_is(self, '/')) {
			cgl_syntax_next(self);
			struct cgl_node_div_t* node = (struct cgl_node_div_t*) self->memory;
			self->memory += sizeof(struct cgl_node_div_t);
			node->vtable = &cgl_node_div_vtable;
			node->left = left;
			node->right = cgl_syntax_unary_build(self);
			left = (struct cgl_node_value_t*) node;
		} else {
			break;
		}
	}
	return left;
}

unsigned char cgl_syntax_unary_test(struct cgl_syntax_t* self) {
	if (cgl_syntax_is(self, '!')) {
		cgl_syntax_next(self);
		if (cgl_syntax_unary_test(self)) {
			return 1;
		}
		self->memory_size += sizeof(struct cgl_node_not_t);
		return 0;
	} else if (cgl_syntax_is(self, '-')) {
		cgl_syntax_next(self);
		if (cgl_syntax_unary_test(self)) {
			return 1;
		}
		self->memory_size += sizeof(struct cgl_node_neg_t);
		return 0;
	} else if (cgl_syntax_is(self, CGL_LEXICAL_SUM_SUM)) {
		cgl_syntax_next(self);
		if (cgl_syntax_literal_test(self)) {
			return 1;
		}
		self->memory_size += sizeof(struct cgl_node_preinc_t);
		return 0;
	} else if (cgl_syntax_is(self, CGL_LEXICAL_SUB_SUB)) {
		cgl_syntax_next(self);
		if (cgl_syntax_literal_test(self)) {
			return 1;
		}
		self->memory_size += sizeof(struct cgl_node_predec_t);
		return 0;
	} else {
		return cgl_syntax_literal_test(self);
	}
}

struct cgl_node_value_t* cgl_syntax_unary_build(struct cgl_syntax_t* self) {
	if (cgl_syntax_is(self, '!')) {
		cgl_syntax_next(self);
		struct cgl_node_not_t* node = (struct cgl_node_not_t*) self->memory;
		self->memory += sizeof(struct cgl_node_not_t);
		node->left = cgl_syntax_literal_build(self);
		return (struct cgl_node_value_t*) node;
	} else if (cgl_syntax_is(self, '-')) {
		cgl_syntax_next(self);
		struct cgl_node_neg_t* node = (struct cgl_node_neg_t*) self->memory;
		self->memory += sizeof(struct cgl_node_neg_t);
		node->vtable = &cgl_node_neg_vtable;
		node->left = cgl_syntax_literal_build(self);
		return (struct cgl_node_value_t*) node;
	} else if (cgl_syntax_is(self, CGL_LEXICAL_SUM_SUM)) {
		cgl_syntax_next(self);
		struct cgl_node_preinc_t* node = (struct cgl_node_preinc_t*) self->memory;
		self->memory += sizeof(struct cgl_node_preinc_t);
		node->left = cgl_syntax_literal_build(self);
		return (struct cgl_node_value_t*) node;
	} else if (cgl_syntax_is(self, CGL_LEXICAL_SUB_SUB)) {
		cgl_syntax_next(self);
		struct cgl_node_predec_t* node = (struct cgl_node_predec_t*) self->memory;
		self->memory += sizeof(struct cgl_node_predec_t);
		node->left = cgl_syntax_literal_build(self);
		return (struct cgl_node_value_t*) node;
	} else {
		return cgl_syntax_literal_build(self);
	}
}

unsigned char cgl_syntax_literal_test(struct cgl_syntax_t* self) {
	if (cgl_syntax_is(self, '(')) {
		cgl_syntax_next(self);
		if (cgl_syntax_expr_test(self)) {
			return 1;
		}
		if (!cgl_syntax_is(self, ')')) {
			return 1;
		}
		cgl_syntax_next(self);
		return 0;
	} else if (cgl_syntax_is(self, CGL_LEXICAL_NUMBER)) {
		self->memory_size += sizeof(struct cgl_node_int_t);
		cgl_syntax_next(self);
		return 0;
	} else if (cgl_syntax_is(self, CGL_LEXICAL_STRING)) {
		self->memory_size += sizeof(struct cgl_node_str_t);
		cgl_syntax_next(self);
		return 0;
	} else if (cgl_syntax_is(self, CGL_LEXICAL_TRUE_WORD)) {
		self->memory_size += sizeof(struct cgl_node_bool_t);
		cgl_syntax_next(self);
		return 0;
	} else if (cgl_syntax_is(self, CGL_LEXICAL_FALSE_WORD)) {
		self->memory_size += sizeof(struct cgl_node_bool_t);
		cgl_syntax_next(self);
		return 0;
	} else if (cgl_syntax_is(self, CGL_LEXICAL_ID)) {
		self->memory_size += sizeof(struct cgl_node_id_t);
		cgl_syntax_next(self);
		while (cgl_syntax_is(self, '[')) {
			cgl_syntax_next(self);
			self->memory_size += sizeof(struct cgl_node_array_t);
			if (cgl_syntax_expr_test(self)) {
				return 1;
			}
			if (!cgl_syntax_is(self, ']')) {
				return 1;
			}
			cgl_syntax_next(self);
		}
		for (;;) {
			if (cgl_syntax_is(self, CGL_LEXICAL_SUM_SUM)) {
				cgl_syntax_next(self);
				self->memory_size += sizeof(struct cgl_node_posinc_t);
			} else if (cgl_syntax_is(self, CGL_LEXICAL_SUB_SUB)) {
				cgl_syntax_next(self);
				self->memory_size += sizeof(struct cgl_node_posdec_t);
			} else {
				break;
			}
		}
		while (cgl_syntax_is(self, '=')) {
			cgl_syntax_next(self);
			self->memory_size += sizeof(struct cgl_node_assign_t);
			if (cgl_syntax_expr_test(self)) {
				return 1;
			}
		}
		return 0;
	} else {
		return 1;
	}
}

struct cgl_node_value_t* cgl_syntax_literal_build(struct cgl_syntax_t* self) {
    struct lng_token_t* name = self->token;
	if (cgl_syntax_is(self, '(')) {
		cgl_syntax_next(self);
		struct cgl_node_value_t* value = cgl_syntax_expr_build(self);
		cgl_syntax_next(self);
		return value;
	} else if (cgl_syntax_is(self, CGL_LEXICAL_NUMBER)) {
		struct cgl_node_int_t* node = (struct cgl_node_int_t*) self->memory;
		self->memory += sizeof(struct cgl_node_int_t);
		node->vtable = &cgl_node_int_vtable;
		node->value = cgl_io_string_to_int((char*) self->text+name->begin, name->size);
		cgl_syntax_next(self);
		return (struct cgl_node_value_t*) node;
	} else if (cgl_syntax_is(self, CGL_LEXICAL_STRING)) {
		struct cgl_node_str_t* node = (struct cgl_node_str_t*) self->memory;
		self->memory += sizeof(struct cgl_node_str_t);
		node->value = 0;
		cgl_syntax_next(self);
		return (struct cgl_node_value_t*) node;
	} else if (cgl_syntax_is(self, CGL_LEXICAL_TRUE_WORD)) {
		struct cgl_node_bool_t* node = (struct cgl_node_bool_t*) self->memory;
		self->memory += sizeof(struct cgl_node_bool_t);
		node->value = 0;
		cgl_syntax_next(self);
		return (struct cgl_node_value_t*) node;
	} else if (cgl_syntax_is(self, CGL_LEXICAL_FALSE_WORD)) {
		struct cgl_node_bool_t* node = (struct cgl_node_bool_t*) self->memory;
		self->memory += sizeof(struct cgl_node_bool_t);
		node->value = 0;
		cgl_syntax_next(self);
		return (struct cgl_node_value_t*) node;
	} else if (cgl_syntax_is(self, CGL_LEXICAL_ID)) {
		struct cgl_node_value_t* left;
		struct cgl_node_id_t* node = (struct cgl_node_id_t*) self->memory;
		self->memory += sizeof(struct cgl_node_id_t);
		node->name = 0;
		left = (struct cgl_node_value_t*) node;
		cgl_syntax_next(self);
		while (cgl_syntax_is(self, '[')) {
			cgl_syntax_next(self);
			struct cgl_node_array_t* assign = (struct cgl_node_array_t*) self->memory;
			self->memory += sizeof(struct cgl_node_array_t);
			assign->left = (struct cgl_node_value_t*) left;
			assign->value = cgl_syntax_expr_build(self);
			left = (struct cgl_node_value_t*) assign;
			cgl_syntax_next(self);
		}
		for (;;) {
			if (cgl_syntax_is(self, CGL_LEXICAL_SUM_SUM)) {
				cgl_syntax_next(self);
				struct cgl_node_posinc_t* assign = (struct cgl_node_posinc_t*) self->memory;
				self->memory += sizeof(struct cgl_node_posinc_t);
				assign->left = (struct cgl_node_value_t*) left;
				left = (struct cgl_node_value_t*) assign;
			} else if (cgl_syntax_is(self, CGL_LEXICAL_SUB_SUB)) {
				cgl_syntax_next(self);
				struct cgl_node_posdec_t* assign = (struct cgl_node_posdec_t*) self->memory;
				self->memory += sizeof(struct cgl_node_posdec_t);
				assign->left = (struct cgl_node_value_t*) left;
				left = (struct cgl_node_value_t*) assign;
			} else {
				break;
			}
		}
		while (cgl_syntax_is(self, '=')) {
			cgl_syntax_next(self);
			struct cgl_node_assign_t* assign = (struct cgl_node_assign_t*) self->memory;
			self->memory += sizeof(struct cgl_node_assign_t);
			assign->left = (struct cgl_node_value_t*) left;
			assign->value = cgl_syntax_expr_build(self);
			left = (struct cgl_node_value_t*) assign;
		}
		return (struct cgl_node_value_t*) node;
	} else {
		assert(0);
		return 0;
	}
}
