#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <lightning.h>
#include "cgl_treemap.h"
#include "cgl_asm.h"

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

//struct cgl_asm_token_t {
//	unsigned int begin;
//	unsigned int size;
//	unsigned short type;
//	unsigned int line;
//	unsigned int column;
//};
//
//struct cgl_asm_token_t* cgl_lng_lexical(const unsigned char* text, unsigned int* tokenSize) {
//	unsigned size = 0;
//	{
//		unsigned char* pc = (unsigned char*) text;
//		unsigned char c = *pc;
//		while (c) {
//			if (c <= ' ') {
//			} else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
//				while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-') {
//					c = *(++pc);
//				}
//				size++;
//				pc--;
//			} else if (c == '0' && pc[1] == 'x') {
//				pc++;
//				c = *(++pc);
//				while ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
//					c = *(++pc);
//				}
//				size++;
//				pc--;
//			} else if ((c >= '0' && c <= '9') || c == '-') {
//				c = *(++pc);
//				while (c >= '0' && c <= '9') {
//					c = *(++pc);
//				}
//				size++;
//				pc--;
//			} else if (c == '\"' || c == '\'') {
//				c = *(++pc);
//				while ((c != '\"' && c != '\'') || pc[-1] == '\\') {
//					if (c <= 0x7F) {
//						c = *(++pc);
//					} else if ((c >> 5) == 0x6) {
//						pc++;
//						c = *(++pc);
//					} else {
//						pc += 2;
//						c = *(++pc);
//					}
//				}
//				size++;
//			} else if (c == '/' && pc[1] == '*') {
//				pc += 3;
//				c = *pc;
//				while (c != '/' || pc[-1] != '*') {
//					if (c <= 0x7F) {
//						c = *(++pc);
//					} else if ((c >> 5) == 0x6) {
//						pc++;
//						c = *(++pc);
//					} else {
//						pc += 2;
//						c = *(++pc);
//					}
//				}
//			} else if (c == '/' && pc[1] == '/') {
//				pc += 2;
//				c = *pc;
//				while (c != '\n') {
//					if (c <= 0x7F) {
//						c = *(++pc);
//					} else if ((c >> 5) == 0x6) {
//						pc++;
//						c = *(++pc);
//					} else {
//						pc += 2;
//						c = *(++pc);
//					}
//				}
//			} else {
//				size++;
//			}
//			c = *(++pc);
//		}
//	}
//	*tokenSize = size;
//	struct cgl_asm_token_t* tokens = (struct cgl_asm_token_t*) malloc((size + 1) * sizeof(struct cgl_asm_token_t));
//	if (!tokens) {
//		return 0;
//	}
//	tokens[size].begin = 0;
//	tokens[size].size = 0;
//	tokens[size].type = CGL_ASM_LEXICAL_EOF;
//	tokens[size].line = 0;
//	tokens[size].column = 0;
//	struct cgl_asm_token_t* token = tokens;
//	unsigned int lin = 1;
//	unsigned int col = 1;
//	unsigned char* pc = (unsigned char*) text;
//	unsigned char c = *pc;
//	while (c) {
//		if (c <= ' ') {
//			if (c == '\n') {
//				lin++;
//				col = 1;
//			} else {
//				col++;
//			}
//		} else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
//			unsigned short type = CGL_ASM_LEXICAL_ID;
//			unsigned char* ctext = pc;
//			int begin = pc - text;
//			while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-') {
//				c = *(++pc);
//			}
//			unsigned int size = (pc - text) - begin;
//			if (size == 2) {
//				switch (*(ctext++)) {
//				case 'r' :
//				case 'R' :
//					switch (*(ctext++)) {
//					case '0' :
//						type = CGL_ASM_LEXICAL_RA;
//						break;
//					case '1' :
//						type = CGL_ASM_LEXICAL_RB;
//						break;
//					case '2' :
//						type = CGL_ASM_LEXICAL_RC;
//						break;
//					}
//					break;
//				case 'f' :
//				case 'F' :
//					switch (*(ctext++)) {
//					case '0' :
//						type = CGL_ASM_LEXICAL_FA;
//						break;
//					case '1' :
//						type = CGL_ASM_LEXICAL_FB;
//						break;
//					case '2' :
//						type = CGL_ASM_LEXICAL_FC;
//						break;
//					}
//					break;
//				case 'v' :
//				case 'V' :
//					switch (*(ctext++)) {
//					case '0' :
//						type = CGL_ASM_LEXICAL_VA;
//						break;
//					case '1' :
//						type = CGL_ASM_LEXICAL_VB;
//						break;
//					case '2' :
//						type = CGL_ASM_LEXICAL_VC;
//						break;
//					}
//					break;
//				}
//			}
//			token->begin = begin;
//			token->size = size;
//			token->type = type;
//			token->line = lin;
//			token->column = col;
//			col += token->size;
//			token++;
//			pc--;
//		} else if (c == '0' && pc[1] == 'x') {
//			unsigned short type = CGL_ASM_LEXICAL_HEX;
//			int begin = pc - text;
//			pc++;
//			c = *(++pc);
//			while ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
//				c = *(++pc);
//			}
//			token->begin = begin;
//			token->size = (pc - text) - begin;
//			token->type = type;
//			token->line = lin;
//			token->column = col;
//			col += token->size;
//			token++;
//			pc--;
//		} else if ((c >= '0' && c <= '9') || c == '-') {
//			unsigned short type = CGL_ASM_LEXICAL_INT;
//			int begin = pc - text;
//			do {
//				c = *(++pc);
//			} while (c >= '0' && c <= '9');
//			token->begin = begin;
//			token->size = (pc - text) - begin;
//			token->type = type;
//			token->line = lin;
//			token->column = col;
//			col += token->size;
//			token++;
//			pc--;
//		} else if (c == '\"' || c == '\'') {
//			int clin = lin;
//			int ccol = col;
//			unsigned short type = CGL_ASM_LEXICAL_STRING;
//			int begin = pc - text;
//			c = *(++pc);
//			col++;
//			while ((c != '\"' && c != '\'') || pc[-1] == '\\') {
//				if (c <= 0x7F) {
//					if (c == '\n') {
//						lin++;
//						col = 0;
//					}
//					c = *(++pc);
//				} else if ((c >> 5) == 0x6) {
//					pc++;
//					c = *(++pc);
//				} else {
//					pc += 2;
//					c = *(++pc);
//				}
//				col++;
//			}
//			token->begin = begin;
//			token->size = (pc - text) - begin + 1;
//			token->type = type;
//			token->line = clin;
//			token->column = ccol;
//			token++;
//		} else if (c == '/' && pc[1] == '*') {
//			pc += 3;
//			col += 3;
//			c = *pc;
//			while (c != '/' || pc[-1] != '*') {
//				if (c <= 0x7F) {
//					if (c == '\n') {
//						lin++;
//						col = 0;
//					}
//					c = *(++pc);
//				} else if ((c >> 5) == 0x6) {
//					pc++;
//					c = *(++pc);
//				} else {
//					pc += 2;
//					c = *(++pc);
//				}
//				col++;
//			}
//		} else if (c == '/' && pc[1] == '/') {
//			pc += 2;
//			c = *pc;
//			while (c != '\n') {
//				if (c <= 0x7F) {
//					c = *(++pc);
//				} else if ((c >> 5) == 0x6) {
//					pc++;
//					c = *(++pc);
//				} else {
//					pc += 2;
//					c = *(++pc);
//				}
//			}
//			lin++;
//			col = 1;
//		} else {
//			token->begin = pc - text;
//			token->size = 1;
//			token->type = c;
//			token->line = lin;
//			token->column = col;
//			token++;
//			col++;
//		}
//		c = *(++pc);
//	}
//	return tokens;
//}
//
//unsigned char cgl_asm_syntax(const unsigned char* text, struct cgl_asm_token_t** token) {
//	struct cgl_asm_syntax_t c;
//	c.error = 0;
//	c.memory = 0;
//	c.memory_size = 0;
//	c.text = (unsigned char*) text;
//	c.token = token;
//	c.stack_index = 0;
//	c.stack_max = 32 * 1024;
//	cgl_asm_syntax_cmds(&c);
//	if (c.error) {
//		return 0;
//	}
//	void* data = malloc(c.memory_size);
//	if (!data) {
//		return 0;
//	}
//	struct cgl_lng_node_t* stack[c.stack_max];
//	c.stack = (struct cgl_lng_node_t**) &stack;
//	c.stack_index = 0;
//	c.token = token;
//	c.memory = data;
//	return (struct cgl_lng_node_t*) cgl_asm_syntax_cmds(&c);
//}
//
//static struct cgl_lng_block_t* cgl_asm_syntax_cmds(struct cgl_syntax_t* c) {
//	struct cgl_lng_block_t* node;
//	struct cgl_lng_node_t** stack = c->stack;
//	if (c->memory) {
//		node = (struct cgl_lng_block_t*) c->memory;
//		node->classid = CGL_LNG_BLOCK_CLASSID;
//		c->memory += sizeof(struct cgl_lng_block_t);
//	} else {
//		node = (void*) 1;
//		c->memory_size += sizeof(struct cgl_lng_block_t);
//		c->memory_size += sizeof(struct cgl_lng_cmd_t*);
//	}
//	while (c->token->type != CGL_LEXICAL_EOF) {
//		struct cgl_lng_cmd_t* cmd = cgl_syntax_cmd(c);
//		if (c->memory) {
//			*(c->stack++) = (struct cgl_lng_node_t*) cmd;
//		} else {
//			if (!cmd) {
//				return 0;
//			}
//			if (c->stack_index++ >= c->stack_max) {
//				c->error = "";
//				return 0;
//			}
//			c->memory_size += sizeof(struct cgl_lng_cmd_t*);
//		}
//	}
//	unsigned int cmd_size = c->stack - stack;
//	if (c->memory) {
//		node->cmd_size = cmd_size;
//		node->cmds = (struct cgl_lng_cmd_t**) c->memory;
//		c->memory += (cmd_size + 1) * sizeof(struct cgl_lng_cmd_t*);
//		memcpy(node->cmds, stack, cmd_size * sizeof(struct cgl_lng_cmd_t*));
//		node->cmds[cmd_size] = 0;
//	}
//	c->stack -= cmd_size;
//	return node;
//}
//
//static struct cgl_lng_cmd_t* cgl_asm_syntax_cmd(struct cgl_syntax_t* c) {
//	switch (c->token->type) {
//	case CGL_LEXICAL_DO : {
//		return (struct cgl_lng_cmd_t*) cgl_syntax_block(c);
//	}
//	case CGL_LEXICAL_VAR : {
//		return (struct cgl_lng_cmd_t*) cgl_syntax_declare(c);
//	}
//	case CGL_LEXICAL_IF : {
//		return (struct cgl_lng_cmd_t*) cgl_syntax_if(c);
//	}
//	case CGL_LEXICAL_WHILE : {
//		return (struct cgl_lng_cmd_t*) cgl_syntax_while(c);
//	}
//	case CGL_LEXICAL_REPEAT : {
//		return (struct cgl_lng_cmd_t*) cgl_syntax_repeat(c);
//	}
//	case CGL_LEXICAL_RETURN : {
//		return (struct cgl_lng_cmd_t*) cgl_syntax_return(c);
//	}
//	case CGL_LEXICAL_FOR : {
//		return (struct cgl_lng_cmd_t*) cgl_syntax_for(c);
//	}
//	case CGL_LEXICAL_SWITCH : {
//		return (struct cgl_lng_cmd_t*) cgl_syntax_switch(c);
//	}
//	default : {
//		return (struct cgl_lng_cmd_t*) cgl_syntax_expression(c);
//	}
//	}
//}

int cgl_asm_nuchars_to_int(unsigned char *s, unsigned int size) {
	int sign = 1;
	int n, m, c = *s;
	if (c == '+' || c == '-') {
		sign = c == '+' ? 1 : -1;
		s++;
		c = *s;
	}
	for (m = 0; m < size; m++) {
		n = (n * 10) + (c - '0');
		c = *++s;
	}
	return sign * n;
}

unsigned char* cgl_asm_read_file(char* file, long* size) {
	FILE* f = fopen(file, "r");
	if (!f) {
		return 0;
	}
	if (fseek(f, 0, SEEK_END)) {
		fclose(f);
		return 0;
	}
	long len = ftell(f);
	if (size) {
		*size = len;
	}
	if (fseek(f, 0, SEEK_SET)) {
		fclose(f);
		return 0;
	}
	unsigned char* data = (unsigned char*) malloc(len + 1);
	if (!data) {
		fclose(f);
		return 0;
	}
	data[len] = 0;
	if (fread(data, 1, len, f) != len) {
		free(data);
		fclose(f);
		return 0;
	}
	return data;
}

static void cgl_asm_read_space(unsigned char** text) {
	unsigned char* aux = *text;
	while (*aux <= ' ') {
		aux++;
	}
	*text = aux;
}

static void cgl_asm_read_id(unsigned char* text, unsigned int* size) {
	char c = *text;
	while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '$') {
		c = *(++text);
	}
	if (size) {
		*size = text - text;
	}
}

static void cgl_asm_read_int(unsigned char* text, unsigned int* size) {
	char c = *text;
	while (c >= '0' && c <= '9') {
		c = *(++text);
	}
	if (size) {
		*size = text - text;
	}
}

cgl_asm_t cgl_asm(unsigned char* text, long size) {
	if (size < 0) {
		size = strlen((char*) text);
	}
	unsigned char* bytes = (unsigned char*) malloc(size);
	if (!bytes) {
		return 0;
	}
	unsigned char* aux = bytes;
	memcpy(bytes, text, size);
	unsigned char* op = aux;
	unsigned int begin_state = 0;
	init_jit(0);
	jit_state_t* _jit = jit_new_state();
	jit_node_t* main_node = jit_note(0, 0);
	char c = *op++;
	while (c) {
		if (c <= ' ') {
			c = *op++;
		} else if (c == ':') {
			do {
				c = *op++;
			} while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '$');
		} else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$') {
			unsigned char* opcode = op - 1;
			do {
				c = *op++;
			} while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '$');
			unsigned int opcode_size = op - opcode - 1;
			switch (opcode_size) {
			case 3 : {
				if (opcode[0] == 'e' && opcode[1] == 'n' && opcode[2] == 'd') {
					if (begin_state) {
						begin_state = 0;
						jit_epilog();
					}
				}
				break;
			}
			case 5 : {
				if (opcode[0] == 'b' && opcode[1] == 'e' && opcode[2] == 'g' && opcode[3] == 'i' && opcode[4] == 'n') {
					if (!begin_state) {
						begin_state = 1;
						jit_prolog();
					}
				}
				break;
			}
			case 4 : {
				// bltr a b label_a
				if (opcode[0] == 'b' && opcode[1] == 'l' && opcode[2] == 't' && opcode[3] == 'r') {

				}
				break;
			}
			case 14 : {
				// int_mov_reg_ra, int_mov_reg_rb, int_mov_reg_rc
				if (opcode[0] == 'i' && opcode[1] == 'n' && opcode[2] == 't' && opcode[3] == '_' && opcode[4] == 'm' && opcode[5] == 'o' && opcode[6] == 'v' && opcode[7] == '_' && opcode[8] == 'r'
						&& opcode[9] == 'e' && opcode[10] == 'g' && opcode[11] == '_' && opcode[12] == 'r' && (opcode[13] == 'a' || opcode[13] == 'b' || opcode[13] == 'c')) {
					while (c <= ' ') {
						c = *op++;
					}
					unsigned char* arg = op - 1;
					while (c >= '0' && c <= '9') {
						c = *op++;
					}
					unsigned int arg_size = op - arg - 1;
					switch (opcode[13]) {
					case 'a' :
						jit_movi(JIT_R0, cgl_asm_nuchars_to_int(arg, arg_size));
						break;
					case 'b' :
						jit_movi(JIT_R1, cgl_asm_nuchars_to_int(arg, arg_size));
						break;
					case 'c' :
						jit_movi(JIT_R2, cgl_asm_nuchars_to_int(arg, arg_size));
						break;
					}
				}
				// ret_reg_ra, ret_reg_rb, ret_reg_rc
				else if (opcode[0] == 'r' && opcode[1] == 'e' && opcode[2] == 't' && opcode[3] == '_' && opcode[4] == 'r' && opcode[5] == 'e' && opcode[6] == 'g' && opcode[7] == '_'
						&& opcode[8] == 'r' && (opcode[9] == 'a' || opcode[9] == 'b' || opcode[9] == 'c')) {
					switch (opcode[9]) {
					case 'a' :
						jit_retr(JIT_R0);
						break;
					case 'b' :
						jit_retr(JIT_R1);
						break;
					case 'c' :
						jit_retr(JIT_R2);
						break;
					}
				}
				break;
			}
			}
		} else {
			c = *op++;
		}
	}
	if (begin_state) {
		jit_epilog();
	}
	jit_emit();
	cgl_asm_t program = (cgl_asm_t) jit_address(main_node);
	jit_clear_state();
	printf("%d\n", program());
	jit_destroy_state();
	finish_jit();
	return program;
}
