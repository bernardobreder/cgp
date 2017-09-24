#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "platform.h"
#include "lexical.h"
#include "syntax.h"
#include "node.h"
#include "compile.h"
#include "cg_io.h"
#include "vm.h"

void cgl_node_unit_head(struct cgl_node_unit_t* self, struct cgl_compile_t* compile) {
	self->package->vtable->head((struct cgl_node_t*) self->package, compile);
	struct cgl_node_import_t* import = self->imports;
	while (import->vtable) {
		import->vtable->head((struct cgl_node_t*) import, compile);
		import++;
	}
	self->clazz->vtable->head((struct cgl_node_t*) self->clazz, compile);
}

void cgl_node_unit_body(struct cgl_node_unit_t* self, struct cgl_compile_t* compile) {
	self->package->vtable->body((struct cgl_node_t*) self->package, compile);
	struct cgl_node_import_t* import = self->imports;
	while (import->vtable) {
		import->vtable->body((struct cgl_node_t*) import, compile);
		import++;
	}
	self->clazz->vtable->body((struct cgl_node_t*) self->clazz, compile);
}

unsigned long cgl_node_unit_size(struct cgl_node_unit_t* self, struct cgl_compile_t* compile) {
	unsigned int result = 1;
	if (self->package && self->package->name->size > 0) {
		struct lng_token_t* pkgname = self->package->name;
		struct lng_token_t* classname = self->clazz->name;
		result += 4 + pkgname->size + 1 + classname->size;
	} else {
		struct lng_token_t* classname = self->clazz->name;
		result += 4 + classname->size;
	}
	{
		result += 4;
		struct cgl_node_field_t* field = self->clazz->fields;
		while (field->vtable) {
			result += field->vtable->size((struct cgl_node_t*) field, compile);
			field++;
		}
	}
	{
		result += 4;
		struct cgl_node_method_t* method = self->clazz->methods;
		while (method->vtable) {
			result += 4 + method->name->size;
			method++;
		}
	}
	{
		struct cgl_node_method_t* method = self->clazz->methods;
		while (method->vtable) {
			result += method->vtable->size((struct cgl_node_t*) method, compile);
			method++;
		}
	}
	result += 1;
	return result;
}

void cgl_node_unit_build(struct cgl_node_unit_t* self, struct cgl_compile_t* compile) {
	char* text = (char*) compile->syntax->text;
	*(compile->bytes_next++) = 0xBB;
	{
		if (self->package && self->package->name->size > 0) {
			struct lng_token_t* pkgname = self->package->name;
			struct lng_token_t* classname = self->clazz->name;
			uint32 size = pkgname->size + 1 + classname->size;
			cgl_io_uint32_write(compile->bytes_next, size);
			compile->bytes_next += 4;
			cgl_io_chars_write(compile->bytes_next, text + pkgname->begin, pkgname->size);
			compile->bytes_next += pkgname->size;
			*(compile->bytes_next++) = '.';
			cgl_io_chars_write(compile->bytes_next, text + classname->begin, classname->size);
			compile->bytes_next += classname->size;
		} else {
			struct lng_token_t* classname = self->clazz->name;
			uint32 size = classname->size;
			cgl_io_uint64_write(compile->bytes_next, size);
			compile->bytes_next += 4;
			cgl_io_chars_write(compile->bytes_next, text + classname->begin, size);
			compile->bytes_next += size;
		}
	}
	{
		int32 size = self->clazz->field_count;
		cgl_io_uint32_write(compile->bytes_next, size);
		compile->bytes_next += 4;
	}
	{
		struct cgl_node_field_t* field = self->clazz->fields;
		while (field->vtable) {
			field->vtable->build((struct cgl_node_t*) field, compile);
			field++;
		}
	}
	{
		int32 size = self->clazz->method_count;
		cgl_io_uint32_write(compile->bytes_next, size);
		compile->bytes_next += 4;
	}
	{
		struct cgl_node_method_t* method = self->clazz->methods;
		while (method->vtable) {
			struct lng_token_t* name = method->name;
			int32 size = name->size;
			cgl_io_uint32_write(compile->bytes_next, size);
			compile->bytes_next += 4;
			cgl_io_chars_write(compile->bytes_next, text + name->begin, size);
			compile->bytes_next += size;
			method++;
		}
	}
	{
		struct cgl_node_method_t* method = self->clazz->methods;
		while (method->vtable) {
			method->vtable->build((struct cgl_node_t*) method, compile);
			method++;
		}
	}
	*(compile->bytes_next++) = 0xFF;
}

void cgl_node_package_head(struct cgl_node_package_t* self, struct cgl_compile_t* compile) {
}

void cgl_node_package_body(struct cgl_node_package_t* self, struct cgl_compile_t* compile) {
}

unsigned long cgl_node_package_size(struct cgl_node_unit_t* self, struct cgl_compile_t* compile) {
	unsigned int result = 1;
	if (self->package && self->package->name->size > 0) {
		struct lng_token_t* pkgname = self->package->name;
		struct lng_token_t* classname = self->clazz->name;
		result += pkgname->size + 1 + classname->size;
	} else {
		struct lng_token_t* classname = self->clazz->name;
		result += classname->size;
	}
	return result;
}

void cgl_node_package_build(struct cgl_node_package_t* self, struct cgl_compile_t* compile) {
}

void cgl_node_import_head(struct cgl_node_import_t* self, struct cgl_compile_t* compile) {
}

unsigned long cgl_node_import_size(struct cgl_node_unit_t* self, struct cgl_compile_t* compile) {
	return 0;
}

void cgl_node_import_body(struct cgl_node_import_t* self, struct cgl_compile_t* compile) {
}

void cgl_node_import_build(struct cgl_node_import_t* self, struct cgl_compile_t* compile) {
}

void cgl_node_class_head(struct cgl_node_class_t* self, struct cgl_compile_t* compile) {
	struct cgl_node_field_t* field = self->fields;
	while (field->vtable) {
		field->vtable->head((struct cgl_node_t*) field, compile);
		field++;
	}
	struct cgl_node_method_t* method = self->methods;
	while (method->vtable) {
		method->vtable->head((struct cgl_node_t*) method, compile);
		method++;
	}
}

void cgl_node_class_body(struct cgl_node_class_t* self, struct cgl_compile_t* compile) {
	struct cgl_node_field_t* field = self->fields;
	while (field->vtable) {
		field->vtable->body((struct cgl_node_t*) field, compile);
		field++;
	}
	struct cgl_node_method_t* method = self->methods;
	while (method->vtable) {
		method->vtable->body((struct cgl_node_t*) method, compile);
		method++;
	}
}

unsigned long cgl_node_class_size(struct cgl_node_unit_t* self, struct cgl_compile_t* compile) {
	return 0;
}

void cgl_node_class_build(struct cgl_node_class_t* self, struct cgl_compile_t* compile) {
}

void cgl_node_field_head(struct cgl_node_field_t* self, struct cgl_compile_t* compile) {
}

void cgl_node_field_body(struct cgl_node_field_t* self, struct cgl_compile_t* compile) {
}

unsigned long cgl_node_field_size(struct cgl_node_unit_t* self, struct cgl_compile_t* compile) {
	return 0;
}

void cgl_node_field_build(struct cgl_node_field_t* self, struct cgl_compile_t* compile) {
}

void cgl_node_method_head(struct cgl_node_method_t* self, struct cgl_compile_t* compile) {
	self->type->vtable->head((struct cgl_node_t*) self->type, compile);
	struct cgl_node_param_t* param = self->parameters;
	while (param->vtable) {
		param->vtable->head((struct cgl_node_t*) param, compile);
		param++;
	}
	self->cmd->vtable->head((struct cgl_node_t*) self->cmd, compile);
}

void cgl_node_method_body(struct cgl_node_method_t* self, struct cgl_compile_t* compile) {
	self->type->vtable->body((struct cgl_node_t*) self->type, compile);
	struct cgl_node_param_t* param = self->parameters;
	while (param->vtable) {
		param->vtable->body((struct cgl_node_t*) param, compile);
		param++;
	}
	self->cmd->vtable->body((struct cgl_node_t*) self->cmd, compile);
}

unsigned long cgl_node_method_size(struct cgl_node_method_t* self, struct cgl_compile_t* compile) {
	return self->cmd->vtable->size((struct cgl_node_t*) self->cmd, compile);
}

void cgl_node_method_build(struct cgl_node_method_t* self, struct cgl_compile_t* compile) {
	self->cmd->vtable->build((struct cgl_node_t*) self->cmd, compile);
}

void cgl_node_type_int_head(struct cgl_node_type_int_t* self, struct cgl_compile_t* compile) {
}

void cgl_node_type_int_body(struct cgl_node_type_int_t* self, struct cgl_compile_t* compile) {
}

unsigned long cgl_node_type_int_size(struct cgl_node_unit_t* self, struct cgl_compile_t* compile) {
	return 0;
}

void cgl_node_type_int_build(struct cgl_node_type_int_t* self, struct cgl_compile_t* compile) {
}

void cgl_node_param_head(struct cgl_node_param_t* self, struct cgl_compile_t* compile) {
	self->type->vtable->head((struct cgl_node_t*) self->type, compile);
}

void cgl_node_param_body(struct cgl_node_param_t* self, struct cgl_compile_t* compile) {
	self->type->vtable->body((struct cgl_node_t*) self->type, compile);
}

unsigned long cgl_node_param_size(struct cgl_node_unit_t* self, struct cgl_compile_t* compile) {
	return 0;
}

void cgl_node_param_build(struct cgl_node_param_t* self, struct cgl_compile_t* compile) {
}

void cgl_node_block_head(struct cgl_node_block_t* self, struct cgl_compile_t* compile) {
	if (self->cmds) {
		struct cgl_node_command_t** cmds = self->cmds;
		while (*cmds) {
			struct cgl_node_command_t* cmd = *(cmds++);
			cmd->vtable->head((struct cgl_node_t*) cmd, compile);
		}
	}
}

void cgl_node_block_body(struct cgl_node_block_t* self, struct cgl_compile_t* compile) {
	if (self->cmds) {
		struct cgl_node_command_t** cmds = self->cmds;
		while (*cmds) {
			struct cgl_node_command_t* cmd = *(cmds++);
			cmd->vtable->body((struct cgl_node_t*) cmd, compile);
		}
	}
}

unsigned long cgl_node_block_size(struct cgl_node_block_t* self, struct cgl_compile_t* compile) {
	unsigned long result = 0;
	if (self->cmds) {
		struct cgl_node_command_t** cmds = self->cmds;
		while (*cmds) {
			struct cgl_node_command_t* cmd = *(cmds++);
			result += cmd->vtable->size((struct cgl_node_t*) cmd, compile);
		}
	}
	return result;
}

void cgl_node_block_build(struct cgl_node_block_t* self, struct cgl_compile_t* compile) {
	if (self->cmds) {
		struct cgl_node_command_t** cmds = self->cmds;
		while (*cmds) {
			struct cgl_node_command_t* cmd = *(cmds++);
			cmd->vtable->build((struct cgl_node_t*) cmd, compile);
		}
	}
}

void cgl_node_return_head(struct cgl_node_return_t* self, struct cgl_compile_t* compile) {
	self->left->vtable->head((struct cgl_node_t*) self->left, compile);
}

void cgl_node_return_body(struct cgl_node_return_t* self, struct cgl_compile_t* compile) {
	self->left->vtable->body((struct cgl_node_t*) self->left, compile);
}

unsigned long cgl_node_return_size(struct cgl_node_return_t* self, struct cgl_compile_t* compile) {
	return 1 + self->left->vtable->size((struct cgl_node_t*) self->left, compile);
}

void cgl_node_return_build(struct cgl_node_return_t* self, struct cgl_compile_t* compile) {
    compile->reg = 0;
	self->left->vtable->build((struct cgl_node_t*) self->left, compile);
    cgl_compile_opcode_stack_return(compile->bytes_next);
}

void cgl_node_expression_head(struct cgl_node_expression_t* self, struct cgl_compile_t* compile) {
	self->left->vtable->head((struct cgl_node_t*) self->left, compile);
}

void cgl_node_expression_body(struct cgl_node_expression_t* self, struct cgl_compile_t* compile) {
	self->left->vtable->body((struct cgl_node_t*) self->left, compile);
}

unsigned long cgl_node_expression_size(struct cgl_node_expression_t* self, struct cgl_compile_t* compile) {
	return 0;
}

void cgl_node_expression_build(struct cgl_node_expression_t* self, struct cgl_compile_t* compile) {
	self->left->vtable->build((struct cgl_node_t*) self->left, compile);
}

void cgl_node_int_head(struct cgl_node_int_t* self, struct cgl_compile_t* compile) {
}

void cgl_node_int_body(struct cgl_node_int_t* self, struct cgl_compile_t* compile) {
}

unsigned long cgl_node_int_size(struct cgl_node_int_t* self, struct cgl_compile_t* compile) {
	return 5;
}

void cgl_node_int_build(struct cgl_node_int_t* self, struct cgl_compile_t* compile) {
    if (compile->reg == 0) {
        cgl_compile_opcode_int_load_a(compile->bytes_next, self->value);
    } else {
        cgl_compile_opcode_int_load_c(compile->bytes_next, self->value);
    }
}

void cgl_node_neg_head(struct cgl_node_neg_t* self, struct cgl_compile_t* compile) {
	self->left->vtable->head((struct cgl_node_t*) self->left, compile);
}

void cgl_node_neg_body(struct cgl_node_neg_t* self, struct cgl_compile_t* compile) {
	self->left->vtable->body((struct cgl_node_t*) self->left, compile);
}

unsigned long cgl_node_neg_size(struct cgl_node_neg_t* self, struct cgl_compile_t* compile) {
	return self->left->vtable->size((struct cgl_node_t*) self->left, compile) + 5 + 1;
}

void cgl_node_neg_build(struct cgl_node_neg_t* self, struct cgl_compile_t* compile) {
	compile->reg = 0;
	self->left->vtable->build((struct cgl_node_t*) self->left, compile);
	cgl_compile_opcode_int_load_c(compile->bytes_next, -1);
	cgl_compile_opcode_int_mul(compile->bytes_next);
}

void cgl_node_id_head(struct cgl_node_id_t* self, struct cgl_compile_t* compile) {
}

void cgl_node_id_body(struct cgl_node_id_t* self, struct cgl_compile_t* compile) {
}

unsigned long cgl_node_id_size(struct cgl_node_id_t* self, struct cgl_compile_t* compile) {
	return 0;
}

void cgl_node_id_build(struct cgl_node_id_t* self, struct cgl_compile_t* compile) {
}

void cgl_node_sum_head(struct cgl_node_sum_t* self, struct cgl_compile_t* compile) {
	self->left->vtable->head((struct cgl_node_t*) self->left, compile);
	self->right->vtable->head((struct cgl_node_t*) self->right, compile);
}

void cgl_node_sum_body(struct cgl_node_sum_t* self, struct cgl_compile_t* compile) {
	self->left->vtable->body((struct cgl_node_t*) self->left, compile);
	self->right->vtable->body((struct cgl_node_t*) self->right, compile);
}

unsigned long cgl_node_sum_size(struct cgl_node_sum_t* self, struct cgl_compile_t* compile) {
	if (self->right->vtable->classid != CGL_NODE_INT_TYPE) {
		return self->left->vtable->size((struct cgl_node_t*) self->left, compile) + self->right->vtable->size((struct cgl_node_t*) self->right, compile) + 4;
	} else {
		return self->left->vtable->size((struct cgl_node_t*) self->left, compile) + self->right->vtable->size((struct cgl_node_t*) self->right, compile) + 1;
	}
}

void cgl_node_sum_build(struct cgl_node_sub_t* self, struct cgl_compile_t* compile) {
	compile->reg = 0;
	self->left->vtable->build((struct cgl_node_t*) self->left, compile);
	if (self->right->vtable->classid != CGL_NODE_INT_TYPE) {
		cgl_compile_opcode_stack_push_a(compile->bytes_next);
	}
	compile->reg = 1;
	self->right->vtable->build((struct cgl_node_t*) self->right, compile);
	if (self->right->vtable->classid != CGL_NODE_INT_TYPE) {
		cgl_compile_opcode_int_mov_a_to_c(compile->bytes_next);
		cgl_compile_opcode_stack_pop_a(compile->bytes_next);
	}
	cgl_compile_opcode_int_sum(compile->bytes_next);
}

void cgl_node_sub_head(struct cgl_node_sub_t* self, struct cgl_compile_t* compile) {
	self->left->vtable->head((struct cgl_node_t*) self->left, compile);
	self->right->vtable->head((struct cgl_node_t*) self->right, compile);
}

void cgl_node_sub_body(struct cgl_node_sub_t* self, struct cgl_compile_t* compile) {
	self->left->vtable->body((struct cgl_node_t*) self->left, compile);
	self->right->vtable->body((struct cgl_node_t*) self->right, compile);
}

unsigned long cgl_node_sub_size(struct cgl_node_sub_t* self, struct cgl_compile_t* compile) {
	if (self->right->vtable->classid != CGL_NODE_INT_TYPE) {
		return self->left->vtable->size((struct cgl_node_t*) self->left, compile) + self->right->vtable->size((struct cgl_node_t*) self->right, compile) + 4;
	} else {
		return self->left->vtable->size((struct cgl_node_t*) self->left, compile) + self->right->vtable->size((struct cgl_node_t*) self->right, compile) + 1;
	}
}

void cgl_node_sub_build(struct cgl_node_sub_t* self, struct cgl_compile_t* compile) {
	compile->reg = 0;
	self->left->vtable->build((struct cgl_node_t*) self->left, compile);
	if (self->right->vtable->classid != CGL_NODE_INT_TYPE) {
		cgl_compile_opcode_stack_push_a(compile->bytes_next);
	}
	compile->reg = 1;
	self->right->vtable->build((struct cgl_node_t*) self->right, compile);
	if (self->right->vtable->classid != CGL_NODE_INT_TYPE) {
		cgl_compile_opcode_int_mov_a_to_c(compile->bytes_next);
		cgl_compile_opcode_stack_pop_a(compile->bytes_next);
	}
	cgl_compile_opcode_int_sub(compile->bytes_next);
}

void cgl_node_mul_head(struct cgl_node_mul_t* self, struct cgl_compile_t* compile) {
	self->left->vtable->head((struct cgl_node_t*) self->left, compile);
	self->right->vtable->head((struct cgl_node_t*) self->right, compile);
}

void cgl_node_mul_body(struct cgl_node_mul_t* self, struct cgl_compile_t* compile) {
	self->left->vtable->body((struct cgl_node_t*) self->left, compile);
	self->right->vtable->body((struct cgl_node_t*) self->right, compile);
}

unsigned long cgl_node_mul_size(struct cgl_node_mul_t* self, struct cgl_compile_t* compile) {
	if (self->right->vtable->classid != CGL_NODE_INT_TYPE) {
		return self->left->vtable->size((struct cgl_node_t*) self->left, compile) + self->right->vtable->size((struct cgl_node_t*) self->right, compile) + 4;
	} else {
		return self->left->vtable->size((struct cgl_node_t*) self->left, compile) + self->right->vtable->size((struct cgl_node_t*) self->right, compile) + 1;
	}
}

void cgl_node_mul_build(struct cgl_node_mul_t* self, struct cgl_compile_t* compile) {
	compile->reg = 0;
	self->left->vtable->build((struct cgl_node_t*) self->left, compile);
	if (self->right->vtable->classid != CGL_NODE_INT_TYPE) {
		cgl_compile_opcode_stack_push_a(compile->bytes_next);
	}
	compile->reg = 1;
	self->right->vtable->build((struct cgl_node_t*) self->right, compile);
	if (self->right->vtable->classid != CGL_NODE_INT_TYPE) {
		cgl_compile_opcode_int_mov_a_to_c(compile->bytes_next);
		cgl_compile_opcode_stack_pop_a(compile->bytes_next);
	}
	cgl_compile_opcode_int_mul(compile->bytes_next);
}

void cgl_node_div_head(struct cgl_node_div_t* self, struct cgl_compile_t* compile) {
	self->left->vtable->head((struct cgl_node_t*) self->left, compile);
	self->right->vtable->head((struct cgl_node_t*) self->right, compile);
}

void cgl_node_div_body(struct cgl_node_div_t* self, struct cgl_compile_t* compile) {
	self->left->vtable->body((struct cgl_node_t*) self->left, compile);
	self->right->vtable->body((struct cgl_node_t*) self->right, compile);
}

unsigned long cgl_node_div_size(struct cgl_node_div_t* self, struct cgl_compile_t* compile) {
	if (self->right->vtable->classid != CGL_NODE_INT_TYPE) {
		return self->left->vtable->size((struct cgl_node_t*) self->left, compile) + self->right->vtable->size((struct cgl_node_t*) self->right, compile) + 4;
	} else {
		return self->left->vtable->size((struct cgl_node_t*) self->left, compile) + self->right->vtable->size((struct cgl_node_t*) self->right, compile) + 1;
	}
}

void cgl_node_div_build(struct cgl_node_div_t* self, struct cgl_compile_t* compile) {
	compile->reg = 0;
	self->left->vtable->build((struct cgl_node_t*) self->left, compile);
	if (self->right->vtable->classid != CGL_NODE_INT_TYPE) {
		cgl_compile_opcode_stack_push_a(compile->bytes_next);
	}
	compile->reg = 1;
	self->right->vtable->build((struct cgl_node_t*) self->right, compile);
	if (self->right->vtable->classid != CGL_NODE_INT_TYPE) {
		cgl_compile_opcode_int_mov_a_to_c(compile->bytes_next);
		cgl_compile_opcode_stack_pop_a(compile->bytes_next);
	}
	cgl_compile_opcode_int_div(compile->bytes_next);
}


void cgl_node_eq_head(struct cgl_node_eq_t* self, struct cgl_compile_t* compile) {
    self->left->vtable->head((struct cgl_node_t*) self->left, compile);
	self->right->vtable->head((struct cgl_node_t*) self->right, compile);
}

void cgl_node_eq_body(struct cgl_node_eq_t* self, struct cgl_compile_t* compile) {
    self->left->vtable->body((struct cgl_node_t*) self->left, compile);
	self->right->vtable->body((struct cgl_node_t*) self->right, compile);
}

unsigned long cgl_node_eq_size(struct cgl_node_eq_t* self, struct cgl_compile_t* compile) {
    unsigned long result = 0;
    result += self->left->vtable->size((struct cgl_node_t*) self->left, compile);
    result += self->right->vtable->size((struct cgl_node_t*) self->right, compile);
    result += 1;
	return result;
}

void cgl_node_eq_build(struct cgl_node_eq_t* self, struct cgl_compile_t* compile) {
    compile->reg = 0;
	self->left->vtable->build((struct cgl_node_t*) self->left, compile);
	if (self->right->vtable->classid != CGL_NODE_INT_TYPE) {
		cgl_compile_opcode_stack_push_a(compile->bytes_next);
	}
	compile->reg = 1;
	self->right->vtable->build((struct cgl_node_t*) self->right, compile);
	if (self->right->vtable->classid != CGL_NODE_INT_TYPE) {
		cgl_compile_opcode_int_mov_a_to_c(compile->bytes_next);
		cgl_compile_opcode_stack_pop_a(compile->bytes_next);
	}
	cgl_compile_opcode_int_cmp_eq(compile->bytes_next);
}

void cgl_node_if_head(struct cgl_node_if_t* self, struct cgl_compile_t* compile) {
    self->cond->vtable->head((struct cgl_node_t*) self->cond, compile);
	self->cmd->vtable->head((struct cgl_node_t*) self->cmd, compile);
}

void cgl_node_if_body(struct cgl_node_if_t* self, struct cgl_compile_t* compile) {
    self->cond->vtable->body((struct cgl_node_t*) self->cond, compile);
	self->cmd->vtable->body((struct cgl_node_t*) self->cmd, compile);
}

unsigned long cgl_node_if_size(struct cgl_node_if_t* self, struct cgl_compile_t* compile) {
    unsigned long result = 0;
    result += self->cmd->vtable->size((struct cgl_node_t*) self->cmd, compile);
	return result;
}

void cgl_node_if_build(struct cgl_node_if_t* self, struct cgl_compile_t* compile) {
    unsigned long cmd_size = self->cmd->vtable->size((struct cgl_node_t*) self->cmd, compile);
    compile->reg = 0;
	self->cond->vtable->build((struct cgl_node_t*) self->cond, compile);
    cgl_compile_opcode_jmp_ne(compile->bytes_next, cmd_size);
    self->cmd->vtable->build((struct cgl_node_t*) self->cmd, compile);
}
