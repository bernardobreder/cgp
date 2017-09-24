#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <lightning.h>
#include "cgl_treemap.h"
#include "cgl_asm.h"
#include "cgl_treemap_stack_str.h"

struct cgl_asm_list_t {
	void* data;
	struct cgl_asm_node_t* next;
};

struct cgl_asm_method_t {
	char* name;
	unsigned int name_size;
	jit_node_t* label;
};

struct cgl_asm_label_t {
	char* name;
	unsigned int name_size;
	jit_node_t* label;
};

struct cgl_asm_call_t {
	jit_node_t* call;
	jit_node_t* label;
};

struct cgl_asm_token_t {
	unsigned int begin;
	unsigned int size;
	unsigned short type;
	unsigned int line;
	unsigned int column;
};

struct cgl_asm_execute_t {
	unsigned char* text;
	struct cgl_lng_token_t* token;
};

#define CGL_ASM_LEXICAL_EOF 0
#define CGL_ASM_LEXICAL_ID 1
#define CGL_ASM_LEXICAL_STRING 3
#define CGL_ASM_LEXICAL_INT 4
#define CGL_ASM_LEXICAL_LONG 5
#define CGL_ASM_LEXICAL_FLOAT 6
#define CGL_ASM_LEXICAL_DOUBLE 7

cgl_asm_t cgl_asm_lexical(const unsigned char* text, unsigned int* tokenSize) {
	unsigned size = 0;
	{
		unsigned char* pc = (unsigned char*) text;
		unsigned char c = *pc;
		while (c) {
			if (c <= ' ') {
			} else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
				while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-') {
					c = *(++pc);
				}
				size++;
				pc--;
			} else if (c == '0' && pc[1] == 'x') {
				pc++;
				c = *(++pc);
				while ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
					c = *(++pc);
				}
				size++;
				pc--;
			} else if ((c >= '0' && c <= '9') || c == '-') {
				c = *(++pc);
				while (c >= '0' && c <= '9') {
					c = *(++pc);
				}
				size++;
				pc--;
			} else if (c == '\"' || c == '\'') {
				c = *(++pc);
				while ((c != '\"' && c != '\'') || pc[-1] == '\\') {
					if (c <= 0x7F) {
						c = *(++pc);
					} else if ((c >> 5) == 0x6) {
						pc++;
						c = *(++pc);
					} else {
						pc += 2;
						c = *(++pc);
					}
				}
				size++;
			} else if (c == '/' && pc[1] == '*') {
				pc += 3;
				c = *pc;
				while (c != '/' || pc[-1] != '*') {
					if (c <= 0x7F) {
						c = *(++pc);
					} else if ((c >> 5) == 0x6) {
						pc++;
						c = *(++pc);
					} else {
						pc += 2;
						c = *(++pc);
					}
				}
			} else if (c == '/' && pc[1] == '/') {
				pc += 2;
				c = *pc;
				while (c != '\n') {
					if (c <= 0x7F) {
						c = *(++pc);
					} else if ((c >> 5) == 0x6) {
						pc++;
						c = *(++pc);
					} else {
						pc += 2;
						c = *(++pc);
					}
				}
			} else {
				size++;
			}
			c = *(++pc);
		}
	}
	*tokenSize = size;
	struct cgl_asm_token_t* tokens = (struct cgl_asm_token_t*) malloc((size + 1) * sizeof(struct cgl_asm_token_t));
	if (!tokens) {
		return 0;
	}
	tokens[size].begin = 0;
	tokens[size].size = 0;
	tokens[size].type = CGL_ASM_LEXICAL_EOF;
	tokens[size].line = 0;
	tokens[size].column = 0;
	struct cgl_asm_token_t* token = tokens;
	unsigned int lin = 1;
	unsigned int col = 1;
	unsigned char* pc = (unsigned char*) text;
	unsigned char c = *pc;
	while (c) {
		if (c <= ' ') {
			if (c == '\n') {
				lin++;
				col = 1;
			} else {
				col++;
			}
		} else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
			int begin = pc - text;
			while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-') {
				c = *(++pc);
			}
			token->begin = begin;
			token->size = (pc - text) - begin;
			token->type = CGL_ASM_LEXICAL_ID;
			token->line = lin;
			token->column = col;
			col += token->size;
			token++;
			pc--;
		} else if (c == '0' && pc[1] == 'x') {
			int begin = pc - text;
			pc++;
			c = *(++pc);
			while ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
				c = *(++pc);
			}
			token->begin = begin;
			token->size = (pc - text) - begin;
			token->type = CGL_ASM_LEXICAL_INT;
			token->line = lin;
			token->column = col;
			col += token->size;
			token++;
			pc--;
		} else if ((c >= '0' && c <= '9') || c == '-') {
			int begin = pc - text;
			do {
				c = *(++pc);
			} while (c >= '0' && c <= '9');
			token->begin = begin;
			token->size = (pc - text) - begin;
			token->type = CGL_ASM_LEXICAL_INT;
			token->line = lin;
			token->column = col;
			col += token->size;
			token++;
			pc--;
		} else if (c == '\"' || c == '\'') {
			int clin = lin;
			int ccol = col;
			int begin = pc - text;
			c = *(++pc);
			col++;
			while ((c != '\"' && c != '\'') || pc[-1] == '\\') {
				if (c <= 0x7F) {
					if (c == '\n') {
						lin++;
						col = 0;
					}
					c = *(++pc);
				} else if ((c >> 5) == 0x6) {
					pc++;
					c = *(++pc);
				} else {
					pc += 2;
					c = *(++pc);
				}
				col++;
			}
			token->begin = begin;
			token->size = (pc - text) - begin + 1;
			token->type = CGL_ASM_LEXICAL_STRING;
			token->line = clin;
			token->column = ccol;
			token++;
		} else if (c == '/' && pc[1] == '*') {
			pc += 3;
			col += 3;
			c = *pc;
			while (c != '/' || pc[-1] != '*') {
				if (c <= 0x7F) {
					if (c == '\n') {
						lin++;
						col = 0;
					}
					c = *(++pc);
				} else if ((c >> 5) == 0x6) {
					pc++;
					c = *(++pc);
				} else {
					pc += 2;
					c = *(++pc);
				}
				col++;
			}
		} else if (c == '/' && pc[1] == '/') {
			pc += 2;
			c = *pc;
			while (c != '\n') {
				if (c <= 0x7F) {
					c = *(++pc);
				} else if ((c >> 5) == 0x6) {
					pc++;
					c = *(++pc);
				} else {
					pc += 2;
					c = *(++pc);
				}
			}
			lin++;
			col = 1;
		} else {
			token->begin = pc - text;
			token->size = 1;
			token->type = c;
			token->line = lin;
			token->column = col;
			token++;
			col++;
		}
		c = *(++pc);
	}
	token = tokens;
	while (token->type != CGL_ASM_LEXICAL_EOF) {
		char* name = text + token->begin;
		int size = token->size;
		token++;
		switch (token->size) {
		case 10 :
			if (!strncmp(name, "movi_reg_a", size)) {
				if (token->type != CGL_ASM_LEXICAL_INT) {
					return 0;
				}
			} else if (!strncmp(name, "movi_reg_b", size)) {
				if (token->type != CGL_ASM_LEXICAL_INT) {
					return 0;
				}
			} else if (!strncmp(name, "movi_reg_c", size)) {
				if (token->type != CGL_ASM_LEXICAL_INT) {
					return 0;
				}
			}
			break;
		}
	}
	struct cgl_treemap_t* names = cgl_treemap_new_vchar(0, 0);
	init_jit(0);
	jit_state_t* _jit = jit_new_state();
	jit_node_t* main_node = jit_note(0, 0);
	cgl_treemap_free(names);
	jit_emit();
	cgl_asm_t program = (cgl_asm_t) jit_address(main_node);
	jit_clear_state();
	printf("%d\n", program());
	jit_destroy_state();
	finish_jit();
	return program;
}

int cgl_asm_nuchars_to_int(unsigned char *s, unsigned int size) {
	int sign = 1;
	int n = *s;
	if (n == '+' || n == '-') {
		sign = n == '+' ? 1 : -1;
		s++;
	}
	int c = *s;
	for (n = 0; c && n < size; s++) {
		n = (n * 10) + (c - '0');
		c = *s;
	}
	return sign * n;
}

//cgl_asm_t cgl_asm(unsigned char* text, unsigned int size, unsigned char** error) {
//	unsigned char* bytes = (unsigned char*) malloc(size);
//	if (!bytes) {
//		return 0;
//	}
//	unsigned char* aux = bytes;
//	memcpy(bytes, text, size);
//	struct cgl_treemap_stack_str_t* names = cgl_treemap_stack_str_new_alloc();
//	init_jit(0);
//	jit_state_t* _jit = jit_new_state();
//	jit_node_t* label_main = jit_note(0, 0);
//	{
//		unsigned char* op = aux;
//		char c = *op++;
//		while (c) {
//			if (c <= ' ') {
//				c = *op++;
//			} else if (c == ':') {
//				unsigned char* label_name = op - 1;
//				unsigned int label_size = op - label_name - 1;
//				cgl_treemap_stack_str_put_alloc(names, (char* )label_name, label_size, jit_label());
//			} else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$') {
//				unsigned char* opcode = op - 1;
//				do {
//					c = *op++;
//				} while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '$');
//				unsigned int opcode_size = op - opcode;
//				switch (opcode_size) {
//				case 10 :
//					if (!strncmp("mov_reg_ra", opcode, opcode_size)) {
//
//					}
//					if (begin[0] == 'm' && begin[1] == 'o' && begin[2] == 'v' && begin[3] == '_' && begin[4] == 'r' && begin[5] == 'e' && begin[6] == 'g' && begin[7] == '_' && begin[8] == 'r'
//							&& begin[9] == 'a') {
//						while ((c < '0' || c > '9') && c != '-') {
//							c = *op++;
//						}
//						unsigned char* arg1 = op - 1;
//						if (c == '-') {
//							c = *op++;
//						}
//						while (c >= '0' && c <= '9') {
//							c = *op++;
//						}
//						unsigned int arg1_size = arg1 - op;
//						int value = cgl_nuchars_to_int(arg1, arg1_size);
//						jit_movi(JIT_R0, value);
//					}
//				}
//			} else {
//				jit_clear_state();
//				jit_destroy_state();
//				finish_jit();
//				return 0;
//			}
//		}
//	}
////	{
////		jit_state_t* _jit;
////		jit_node_t* main_node;
////		jit_node_t* labels[1024];
////		unsigned int label_index = 0;
////		struct cgl_asm_call_t calls[1024];
////		struct cgl_asm_call_t* callc = calls;
////		init_jit(0);
////		_jit = jit_new_state();
////		unsigned char flag = 1;
////		unsigned char erro = 0;
////		char c = *op++;
////		while (c) {
////			switch (c) {
////			case '\t' :
////				break;
////			case ':' :
////				break;
////			default :
////				if (!(((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '$'))) {
////					return 0;
////				}
////				while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '$') {
////					c = *op++;
////				}
////			}
////		}
////		{
////			int n, patch_size = callc - calls;
////			callc = calls;
////			for (n = 0; n < patch_size; n++) {
////				jit_patch_at(callc->call, callc->label);
////				callc++;
////			}
////		}
////		jit_emit();
////		cgl_asm_t program = (cgl_asm_t) jit_address(main_node);
////		jit_clear_state();
////		printf("%d\n", program());
////		jit_destroy_state();
////		finish_jit();
////		return program;
////	}
//}
