#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "cgl.h"

struct cgl_lng_bool_t CGL_LNG_BOOL_TRUE = { CGL_LNG_BOOL_CLASSID, 1 };

struct cgl_lng_bool_t CGL_LNG_BOOL_FALSE = { CGL_LNG_BOOL_CLASSID, 0 };

struct cgl_lng_type_byte_t CGL_LNG_TYPE_BYTE;

struct cgl_lng_type_int_t CGL_LNG_TYPE_INT;

struct cgl_lng_type_long_t CGL_LNG_TYPE_LONG;

struct cgl_lng_type_float_t CGL_LNG_TYPE_FLOAT;

struct cgl_lng_type_double_t CGL_LNG_TYPE_DOUBLE;

struct cgl_lng_type_bool_t CGL_LNG_TYPE_BOOL;

struct cgl_lng_type_string_t CGL_LNG_TYPE_STRING;

static struct cgl_lng_block_t* cgl_syntax_cmds(struct cgl_syntax_t* c);

static struct cgl_lng_cmd_t* cgl_syntax_cmd(struct cgl_syntax_t* c);

static struct cgl_lng_declare_t* cgl_syntax_declare(struct cgl_syntax_t* c);

static struct cgl_lng_return_t* cgl_syntax_return(struct cgl_syntax_t* c);

static struct cgl_lng_if_t* cgl_syntax_if(struct cgl_syntax_t* c);

static struct cgl_lng_while_t* cgl_syntax_while(struct cgl_syntax_t* c);

static struct cgl_lng_repeat_t* cgl_syntax_repeat(struct cgl_syntax_t* c);

static struct cgl_lng_for_t* cgl_syntax_for(struct cgl_syntax_t* c);

static struct cgl_lng_block_t* cgl_syntax_block(struct cgl_syntax_t* c);

static struct cgl_lng_switch_t* cgl_syntax_switch(struct cgl_syntax_t* c);

static struct cgl_lng_case_t* cgl_syntax_case(struct cgl_syntax_t* c);

static struct cgl_lng_expression_t* cgl_syntax_expression(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_exp(struct cgl_syntax_t* c);

static struct cgl_lng_type_t* cgl_syntax_type(struct cgl_syntax_t* c);

static struct cgl_lng_type_t* cgl_syntax_type_primitive(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_ternary(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_and(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_inclusive(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_exclusive(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_and_bit(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_equal(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_or(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_compare(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_shift(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_sum_sub(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_mul_div_mod(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_unary(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_unary_not(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_unary_pre_inc(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_unary_pre_dec(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_literal(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_literal_int(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_literal_str(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_literal_hex(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_literal_id(struct cgl_syntax_t* c);

static struct cgl_lng_exp_t* cgl_syntax_literal_wrap(struct cgl_syntax_t* c);

static unsigned int cgl_hex_to_uint(unsigned char* text, unsigned int len);

struct cgl_lng_token_t* cgl_lng_lexical(const unsigned char* text, unsigned int* tokenSize) {
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
				switch (c) {
				case '>' : {
					switch (pc[1]) {
					case '=' : {
						pc++;
						break;
					}
					case '>' : {
						pc++;
						break;
					}
					}
					break;
				}
				case '<' : {
					switch (pc[1]) {
					case '=' : {
						pc++;
						break;
					}
					case '<' : {
						pc++;
						break;
					}
					}
					break;
				}
				case '!' : {
					switch (pc[1]) {
					case '=' : {
						pc++;
						break;
					}
					}
					break;
				}
				case '=' : {
					switch (pc[1]) {
					case '=' : {
						pc++;
						break;
					}
					}
					break;
				}
				case '+' : {
					switch (pc[1]) {
					case '=' : {
						pc++;
						break;
					}
					}
					break;
				}
				case '-' : {
					switch (pc[1]) {
					case '=' : {
						pc++;
						break;
					}
					}
					break;
				}
				case '*' : {
					switch (pc[1]) {
					case '=' : {
						pc++;
						break;
					}
					}
					break;
				}
				case '/' : {
					switch (pc[1]) {
					case '=' : {
						pc++;
						break;
					}
					}
					break;
				}
				}
				size++;
			}
			c = *(++pc);
		}
	}
	*tokenSize = size;
	struct cgl_lng_token_t* tokens = (struct cgl_lng_token_t*) malloc((size + 1) * sizeof(struct cgl_lng_token_t));
	if (!tokens) {
		return 0;
	}
	tokens[size].begin = 0;
	tokens[size].size = 0;
	tokens[size].type = CGL_LEXICAL_EOF;
	tokens[size].line = 0;
	tokens[size].column = 0;
	struct cgl_lng_token_t* token = tokens;
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
			unsigned short type = CGL_LEXICAL_ID;
			int begin = pc - text;
			while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '-') {
				c = *(++pc);
			}
			unsigned int size = (pc - text) - begin;
			{
				const unsigned char* name = text + begin;
				switch (size) {
				case 2 : {
					if (name[0] == 'd' && name[1] == 'o') {
						type = CGL_LEXICAL_DO;
					} else if (name[0] == 'i' && name[1] == 'f') {
						type = CGL_LEXICAL_IF;
					} else if (name[0] == 'o' && name[1] == 'r') {
						type = CGL_LEXICAL_OR;
					}
					break;
				}
				case 3 : {
					if (name[0] == 'f' && name[1] == 'o' && name[2] == 'r') {
						type = CGL_LEXICAL_FOR;
					} else if (name[0] == 'e' && name[1] == 'n' && name[2] == 'd') {
						type = CGL_LEXICAL_END;
					} else if (name[0] == 'v' && name[1] == 'a' && name[2] == 'r') {
						type = CGL_LEXICAL_VAR;
					} else if (name[0] == 'i' && name[1] == 'n' && name[2] == 't') {
						type = CGL_LEXICAL_TYPE_INT;
					} else if (name[0] == 'a' && name[1] == 'n' && name[2] == 't') {
						type = CGL_LEXICAL_AND;
					}
					break;
				}
				case 4 : {
					if (name[0] == 'e' && name[1] == 'l' && name[2] == 's' && name[3] == 'e') {
						type = CGL_LEXICAL_ELSE;
					} else if (name[0] == 'c' && name[1] == 'a' && name[2] == 's' && name[3] == 'e') {
						type = CGL_LEXICAL_CASE;
					} else if (name[0] == 'b' && name[1] == 'y' && name[2] == 't' && name[3] == 'e') {
						type = CGL_LEXICAL_TYPE_BYTE;
					} else if (name[0] == 'l' && name[1] == 'o' && name[2] == 'n' && name[3] == 'g') {
						type = CGL_LEXICAL_TYPE_LONG;
					} else if (name[0] == 't' && name[1] == 'r' && name[2] == 'u' && name[3] == 'e') {
						type = CGL_LEXICAL_TRUE;
					}
					break;
				}
				case 5 : {
					if (name[0] == 'w' && name[1] == 'h' && name[2] == 'i' && name[3] == 'l' && name[4] == 'e') {
						type = CGL_LEXICAL_WHILE;
					} else if (name[0] == 'b' && name[1] == 'r' && name[2] == 'e' && name[3] == 'a' && name[4] == 'k') {
						type = CGL_LEXICAL_BREAK;
					} else if (name[0] == 'f' && name[1] == 'l' && name[2] == 'o' && name[3] == 'a' && name[4] == 't') {
						type = CGL_LEXICAL_TYPE_FLOAT;
					} else if (name[0] == 'f' && name[1] == 'a' && name[2] == 'l' && name[3] == 's' && name[4] == 'e') {
						type = CGL_LEXICAL_FALSE;
					}
					break;
				}
				case 6 : {
					if (name[0] == 's' && name[1] == 't' && name[2] == 'r' && name[3] == 'i' && name[4] == 'n' && name[5] == 'g') {
						type = CGL_LEXICAL_TYPE_STRING;
					} else if (name[0] == 'r' && name[1] == 'e' && name[2] == 'p' && name[3] == 'e' && name[4] == 'a' && name[5] == 't') {
						type = CGL_LEXICAL_REPEAT;
					} else if (name[0] == 'r' && name[1] == 'e' && name[2] == 't' && name[3] == 'u' && name[4] == 'r' && name[5] == 'n') {
						type = CGL_LEXICAL_RETURN;
					} else if (name[0] == 's' && name[1] == 'w' && name[2] == 'i' && name[3] == 't' && name[4] == 'c' && name[5] == 'h') {
						type = CGL_LEXICAL_SWITCH;
					} else if (name[0] == 'd' && name[1] == 'o' && name[2] == 'u' && name[3] == 'b' && name[4] == 'l' && name[5] == 'e') {
						type = CGL_LEXICAL_TYPE_DOUBLE;
					}
					break;
				}
				case 7 : {
					if (name[0] == 'd' && name[1] == 'e' && name[2] == 'f' && name[3] == 'a' && name[4] == 'u' && name[5] == 'l' && name[6] == 't') {
						type = CGL_LEXICAL_DEFAULT;
					}
					break;
				}
				case 8 : {
					if (name[0] == 'c' && name[1] == 'o' && name[2] == 'n' && name[3] == 't' && name[4] == 'i' && name[5] == 'n' && name[6] == 'u' && name[7] == 'e') {
						type = CGL_LEXICAL_CONTINUE;
					} else if (name[0] == 'f' && name[1] == 'u' && name[2] == 'n' && name[3] == 'c' && name[4] == 't' && name[5] == 'i' && name[6] == 'o' && name[7] == 'n') {
						type = CGL_LEXICAL_FUNCTION;
					}
					break;
				}
				}

			}
			token->begin = begin;
			token->size = size;
			token->type = type;
			token->line = lin;
			token->column = col;
			col += token->size;
			token++;
			pc--;
		} else if (c == '0' && pc[1] == 'x') {
			unsigned short type = CGL_LEXICAL_HEX_VALUE;
			int begin = pc - text;
			pc++;
			c = *(++pc);
			while ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
				c = *(++pc);
			}
			token->begin = begin;
			token->size = (pc - text) - begin;
			token->type = type;
			token->line = lin;
			token->column = col;
			col += token->size;
			token++;
			pc--;
		} else if ((c >= '0' && c <= '9') || c == '-') {
			unsigned short type = CGL_LEXICAL_INT_VALUE;
			int begin = pc - text;
			do {
				c = *(++pc);
			} while (c >= '0' && c <= '9');
			token->begin = begin;
			token->size = (pc - text) - begin;
			token->type = type;
			token->line = lin;
			token->column = col;
			col += token->size;
			token++;
			pc--;
		} else if (c == '\"' || c == '\'') {
			int clin = lin;
			int ccol = col;
			unsigned short type = CGL_LEXICAL_STRING_VALUE;
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
			token->type = type;
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
			switch (c) {
			case '>' : {
				switch (pc[1]) {
				case '=' : {
					token->begin = pc++ - text;
					token->size = 2;
					token->type = CGL_LEXICAL_GREATER_EQUAL;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				case '>' : {
					token->begin = pc++ - text;
					token->size = 2;
					token->type = CGL_LEXICAL_GREATER_GREATER;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				default : {
					token->begin = pc - text;
					token->size = 1;
					token->type = c;
					token->line = lin;
					token->column = col;
					token++;
					col++;
				}
				}
				break;
			}
			case '<' : {
				switch (pc[1]) {
				case '=' : {
					token->begin = pc++ - text;
					token->size = 2;
					token->type = CGL_LEXICAL_LOWER_EQUAL;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				case '<' : {
					token->begin = pc++ - text;
					token->size = 2;
					token->type = CGL_LEXICAL_LOWER_LOWER;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				default : {
					token->begin = pc - text;
					token->size = 1;
					token->type = c;
					token->line = lin;
					token->column = col;
					token++;
					col++;
				}
				}
				break;
			}
			case '!' : {
				switch (pc[1]) {
				case '=' : {
					token->begin = pc++ - text;
					token->size = 2;
					token->type = CGL_LEXICAL_DIFERENT;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				default : {
					token->begin = pc - text;
					token->size = 1;
					token->type = c;
					token->line = lin;
					token->column = col;
					token++;
					col++;
				}
				}
				break;
			}
			case '=' : {
				switch (pc[1]) {
				case '=' : {
					token->begin = pc++ - text;
					token->size = 2;
					token->type = CGL_LEXICAL_EQUAL_EQUAL;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				default : {
					token->begin = pc - text;
					token->size = 1;
					token->type = c;
					token->line = lin;
					token->column = col;
					token++;
					col++;
				}
				}
				break;
			}
			case '+' : {
				switch (pc[1]) {
				case '=' : {
					token->begin = pc++ - text;
					token->size = 2;
					token->type = CGL_LEXICAL_SUM_EQUAL;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				case '+' : {
					token->begin = pc++ - text;
					token->size = 2;
					token->type = CGL_LEXICAL_SUM_SUM;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				default : {
					token->begin = pc - text;
					token->size = 1;
					token->type = c;
					token->line = lin;
					token->column = col;
					token++;
					col++;
				}
				}
				break;
			}
			case '-' : {
				switch (pc[1]) {
				case '=' : {
					token->begin = pc++ - text;
					token->size = 2;
					token->type = CGL_LEXICAL_SUB_EQUAL;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				case '-' : {
					token->begin = pc++ - text;
					token->size = 2;
					token->type = CGL_LEXICAL_SUB_SUB;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				default : {
					token->begin = pc - text;
					token->size = 1;
					token->type = c;
					token->line = lin;
					token->column = col;
					token++;
					col++;
				}
				}
				break;
			}
			case '*' : {
				switch (pc[1]) {
				case '=' : {
					token->begin = pc++ - text;
					token->size = 2;
					token->type = CGL_LEXICAL_MUL_EQUAL;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				default : {
					token->begin = pc - text;
					token->size = 1;
					token->type = c;
					token->line = lin;
					token->column = col;
					token++;
					col++;
				}
				}
				break;
			}
			case '/' : {
				switch (pc[1]) {
				case '=' : {
					token->begin = pc++ - text;
					token->size = 2;
					token->type = CGL_LEXICAL_DIV_EQUAL;
					token->line = lin;
					token->column = col;
					token++;
					col += 2;
					break;
				}
				default : {
					token->begin = pc - text;
					token->size = 1;
					token->type = c;
					token->line = lin;
					token->column = col;
					token++;
					col++;
				}
				}
				break;
			}
			default : {
				token->begin = pc - text;
				token->size = 1;
				token->type = c;
				token->line = lin;
				token->column = col;
				token++;
				col++;
			}
			}
		}
		c = *(++pc);
	}
	return tokens;
}

struct cgl_lng_node_t* cgl_syntax(const unsigned char* text, struct cgl_lng_token_t* token) {
	struct cgl_syntax_t c;
	c.error = 0;
	c.memory = 0;
	c.memory_size = 0;
	c.text = (unsigned char*) text;
	c.token = token;
	c.stack_index = 0;
	c.stack_max = 32 * 1024;
	cgl_syntax_cmds(&c);
	if (c.error) {
		return 0;
	}
	void* data = malloc(c.memory_size);
	if (!data) {
		return 0;
	}
	struct cgl_lng_node_t* stack[c.stack_max];
	c.stack = (struct cgl_lng_node_t**) &stack;
	c.stack_index = 0;
	c.token = token;
	c.memory = data;
	return (struct cgl_lng_node_t*) cgl_syntax_cmds(&c);
}

static struct cgl_lng_block_t* cgl_syntax_cmds(struct cgl_syntax_t* c) {
	struct cgl_lng_block_t* node;
	struct cgl_lng_node_t** stack = c->stack;
	if (c->memory) {
		node = (struct cgl_lng_block_t*) c->memory;
		node->classid = CGL_LNG_BLOCK_CLASSID;
		c->memory += sizeof(struct cgl_lng_block_t);
	} else {
		node = (void*) 1;
		c->memory_size += sizeof(struct cgl_lng_block_t);
		c->memory_size += sizeof(struct cgl_lng_cmd_t*);
	}
	while (c->token->type != CGL_LEXICAL_EOF) {
		struct cgl_lng_cmd_t* cmd = cgl_syntax_cmd(c);
		if (c->memory) {
			*(c->stack++) = (struct cgl_lng_node_t*) cmd;
		} else {
			if (!cmd) {
				return 0;
			}
			if (c->stack_index++ >= c->stack_max) {
				c->error = "";
				return 0;
			}
			c->memory_size += sizeof(struct cgl_lng_cmd_t*);
		}
	}
	unsigned int cmd_size = c->stack - stack;
	if (c->memory) {
		node->cmd_size = cmd_size;
		node->cmds = (struct cgl_lng_cmd_t**) c->memory;
		c->memory += (cmd_size + 1) * sizeof(struct cgl_lng_cmd_t*);
		memcpy(node->cmds, stack, cmd_size * sizeof(struct cgl_lng_cmd_t*));
		node->cmds[cmd_size] = 0;
	}
	c->stack -= cmd_size;
	return node;
}

static struct cgl_lng_cmd_t* cgl_syntax_cmd(struct cgl_syntax_t* c) {
	switch (c->token->type) {
	case CGL_LEXICAL_DO : {
		return (struct cgl_lng_cmd_t*) cgl_syntax_block(c);
	}
	case CGL_LEXICAL_VAR : {
		return (struct cgl_lng_cmd_t*) cgl_syntax_declare(c);
	}
	case CGL_LEXICAL_IF : {
		return (struct cgl_lng_cmd_t*) cgl_syntax_if(c);
	}
	case CGL_LEXICAL_WHILE : {
		return (struct cgl_lng_cmd_t*) cgl_syntax_while(c);
	}
	case CGL_LEXICAL_REPEAT : {
		return (struct cgl_lng_cmd_t*) cgl_syntax_repeat(c);
	}
	case CGL_LEXICAL_RETURN : {
		return (struct cgl_lng_cmd_t*) cgl_syntax_return(c);
	}
	case CGL_LEXICAL_FOR : {
		return (struct cgl_lng_cmd_t*) cgl_syntax_for(c);
	}
	case CGL_LEXICAL_SWITCH : {
		return (struct cgl_lng_cmd_t*) cgl_syntax_switch(c);
	}
	default : {
		return (struct cgl_lng_cmd_t*) cgl_syntax_expression(c);
	}
	}
}

static struct cgl_lng_block_t* cgl_syntax_block(struct cgl_syntax_t* c) {
	struct cgl_lng_block_t* node;
	struct cgl_lng_node_t** stack = c->stack;
	if (c->memory) {
		node = (struct cgl_lng_block_t*) c->memory;
		node->classid = CGL_LNG_BLOCK_CLASSID;
		c->memory += sizeof(struct cgl_lng_block_t);
	} else {
		node = (void*) 1;
		c->memory_size += sizeof(struct cgl_lng_block_t);
		c->memory_size += sizeof(struct cgl_lng_cmd_t*);
	}
	c->token++;
	while (c->token->type != CGL_LEXICAL_END) {
		struct cgl_lng_cmd_t* cmd = cgl_syntax_cmd(c);
		if (c->memory) {
			*(c->stack++) = (struct cgl_lng_node_t*) cmd;
		} else {
			if (!cmd) {
				return 0;
			}
			if (c->stack_index++ >= c->stack_max) {
				c->error = "";
				return 0;
			}
			c->memory_size += sizeof(struct cgl_lng_cmd_t*);
		}
	}
	c->token++;
	unsigned int cmd_size = c->stack - stack;
	if (c->memory) {
		node->cmd_size = cmd_size;
		node->cmds = (struct cgl_lng_cmd_t**) c->memory;
		c->memory += (cmd_size + 1) * sizeof(struct cgl_lng_cmd_t*);
		memcpy(node->cmds, stack, cmd_size * sizeof(struct cgl_lng_cmd_t*));
		node->cmds[cmd_size] = 0;
	}
	c->stack -= cmd_size;
	return node;
}

static struct cgl_lng_if_t* cgl_syntax_if(struct cgl_syntax_t* c) {
	struct cgl_lng_if_t* node;
	if (c->memory) {
		node = (struct cgl_lng_if_t*) c->memory;
		node->classid = CGL_LNG_IF_CLASSID;
		c->memory += sizeof(struct cgl_lng_if_t);
	} else {
		node = (void*) 1;
		c->memory_size += sizeof(struct cgl_lng_if_t);
	}
	c->token++;
	struct cgl_lng_exp_t* expr = cgl_syntax_exp(c);
	if (c->memory) {
		node->expr = expr;
	} else {
		if (!expr) {
			return 0;
		}
	}
	struct cgl_lng_cmd_t* cmd = cgl_syntax_cmd(c);
	if (c->memory) {
		node->cmd = cmd;
	} else {
		if (!cmd) {
			return 0;
		}
	}
	return node;
}

static struct cgl_lng_while_t* cgl_syntax_while(struct cgl_syntax_t* c) {
	struct cgl_lng_while_t* node;
	if (c->memory) {
		node = (struct cgl_lng_while_t*) c->memory;
		node->classid = CGL_LNG_WHILE_CLASSID;
		c->memory += sizeof(struct cgl_lng_while_t);
	} else {
		node = (void*) 1;
		c->memory_size += sizeof(struct cgl_lng_while_t);
	}
	c->token++;
	struct cgl_lng_exp_t* expr = cgl_syntax_exp(c);
	if (c->memory) {
		node->expr = expr;
	} else {
		if (!expr) {
			return 0;
		}
	}
	struct cgl_lng_cmd_t* cmd = cgl_syntax_cmd(c);
	if (c->memory) {
		node->cmd = cmd;
	} else {
		if (!cmd) {
			return 0;
		}
	}
	return node;
}

static struct cgl_lng_repeat_t* cgl_syntax_repeat(struct cgl_syntax_t* c) {
	struct cgl_lng_repeat_t* node;
	if (c->memory) {
		node = (struct cgl_lng_repeat_t*) c->memory;
		node->classid = CGL_LNG_REPEAT_CLASSID;
		c->memory += sizeof(struct cgl_lng_repeat_t);
	} else {
		node = (void*) 1;
		c->memory_size += sizeof(struct cgl_lng_repeat_t);
	}
	c->token++;
	struct cgl_lng_cmd_t* cmd = cgl_syntax_cmd(c);
	if (c->memory) {
		node->cmd = cmd;
	} else {
		if (!cmd) {
			return 0;
		}
	}
	if (!c->memory) {
		if (c->token->type != CGL_LEXICAL_WHILE) {
			c->error = "";
			return 0;
		}
	}
	c->token++;
	struct cgl_lng_exp_t* expr = cgl_syntax_exp(c);
	if (c->memory) {
		node->expr = expr;
	} else {
		if (!expr) {
			return 0;
		}
	}
	return node;
}

static struct cgl_lng_declare_t* cgl_syntax_declare(struct cgl_syntax_t* c) {
	struct cgl_lng_declare_t* node;
	if (c->memory) {
		node = (struct cgl_lng_declare_t*) c->memory;
		node->classid = CGL_LNG_DECLARE_CLASSID;
		c->memory += sizeof(struct cgl_lng_declare_t);
	} else {
		node = (void*) 1;
		c->memory_size += sizeof(struct cgl_lng_declare_t);
	}
	c->token++;
	cgl_syntax_type(c);
	if (!c->memory) {
		if (c->token->type != CGL_LEXICAL_ID) {
			c->error = "";
			return 0;
		}
	}
	if (c->memory) {
		node->name = c->token++;
	}
	if (c->token->type == '=') {
		c->token++;
		struct cgl_lng_exp_t* expr = cgl_syntax_exp(c);
		if (c->memory) {
			node->value = expr;
		} else {
			if (!expr) {
				return 0;
			}
		}
	}
	return node;
}

static struct cgl_lng_return_t* cgl_syntax_return(struct cgl_syntax_t* c) {
	struct cgl_lng_return_t* node;
	if (c->memory) {
		node = (struct cgl_lng_return_t*) c->memory;
		node->classid = CGL_LNG_RETURN_CLASSID;
		c->memory += sizeof(struct cgl_lng_return_t);
	} else {
		node = (void*) 1;
		c->memory_size += sizeof(struct cgl_lng_return_t);
	}
	c->token++;
	struct cgl_lng_exp_t* expr = cgl_syntax_exp(c);
	if (c->memory) {
		node->expr = expr;
	} else {
		if (!expr) {
			return 0;
		}
	}
	return node;
}

static struct cgl_lng_for_t* cgl_syntax_for(struct cgl_syntax_t* c) {
	if (c->memory) {

	} else {
		c->memory_size += sizeof(struct cgl_lng_for_t);
	}
	c->token++;
	if (c->token->type != '(') {
		c->error = "";
		return 0;
	}
	c->token++;
	if (c->token->type != ';') {
		cgl_syntax_cmd(c);
	}
	if (c->token->type != ';') {
		c->error = "";
		return 0;
	}
	c->token++;
	if (c->token->type != ')') {
		cgl_syntax_exp(c);
	}
	if (c->token->type != ')') {
		c->error = "";
		return 0;
	}
	c->token++;
	cgl_syntax_cmd(c);
	cgl_syntax_cmd(c);
}

static struct cgl_lng_switch_t* cgl_syntax_switch(struct cgl_syntax_t* c) {
	if (c->memory) {

	} else {
		c->memory_size += sizeof(struct cgl_lng_switch_t);
	}
	c->token++;
	if (c->token->type != CGL_LEXICAL_SWITCH) {
		c->error = "";
		return 0;
	}
	c->token++;
	while (c->token->type == CGL_LEXICAL_CASE) {
		cgl_syntax_case(c);
	}
	if (c->token->type == CGL_LEXICAL_DEFAULT) {
		c->token++;
		cgl_syntax_cmd(c);
	}
	if (c->token->type != CGL_LEXICAL_END) {
		return 0;
	}
	c->token++;
}

static struct cgl_lng_case_t* cgl_syntax_case(struct cgl_syntax_t* c) {
	if (c->memory) {

	} else {
		c->memory_size += sizeof(struct cgl_lng_case_t);
	}
	c->token++;
	cgl_syntax_exp(c);
	cgl_syntax_cmd(c);
}

static struct cgl_lng_expression_t* cgl_syntax_expression(struct cgl_syntax_t* c) {
	struct cgl_lng_expression_t* node;
	if (c->memory) {
		node = (struct cgl_lng_expression_t*) c->memory;
		node->classid = CGL_LNG_EXPRESSION_CLASSID;
		c->memory += sizeof(struct cgl_lng_expression_t);
	} else {
		node = (void*) 1;
		c->memory_size += sizeof(struct cgl_lng_expression_t);
	}
	struct cgl_lng_exp_t* expr = cgl_syntax_exp(c);
	if (c->memory) {
		node->expr = expr;
	} else {
		if (!expr) {
			return 0;
		}
	}
	return node;
}

static struct cgl_lng_type_t* cgl_syntax_type(struct cgl_syntax_t* c) {
	struct cgl_lng_type_t* type = cgl_syntax_type_primitive(c);
	if (!type) {
		return 0;
	}
//	while (c->token->type == '[') {
//		c->token++;
//		if (c->token->type != ']') {
//			c->error = "";
//			return 0;
//		}
//		c->token++;
//	}
	return type;
}

static struct cgl_lng_type_t* cgl_syntax_type_primitive(struct cgl_syntax_t* c) {
	switch (c->token->type) {
	case CGL_LEXICAL_TYPE_INT : {
		return (struct cgl_lng_type_t*) &CGL_LNG_TYPE_INT;
	}
	case CGL_LEXICAL_TYPE_BYTE : {
		return (struct cgl_lng_type_t*) &CGL_LNG_TYPE_BYTE;
	}
	case CGL_LEXICAL_TYPE_FLOAT : {
		return (struct cgl_lng_type_t*) &CGL_LNG_TYPE_FLOAT;
	}
	case CGL_LEXICAL_TYPE_LONG : {
		return (struct cgl_lng_type_t*) &CGL_LNG_TYPE_LONG;
	}
	case CGL_LEXICAL_TYPE_DOUBLE : {
		return (struct cgl_lng_type_t*) &CGL_LNG_TYPE_DOUBLE;
	}
	case CGL_LEXICAL_TYPE_STRING : {
		return (struct cgl_lng_type_t*) &CGL_LNG_TYPE_STRING;
	}
	default : {
		c->error = "";
		return 0;
	}
	}
}

static struct cgl_lng_exp_t* cgl_syntax_exp(struct cgl_syntax_t* c) {
	return cgl_syntax_ternary(c);
}

static struct cgl_lng_exp_t* cgl_syntax_ternary(struct cgl_syntax_t* c) {
	struct cgl_lng_exp_t* left = cgl_syntax_or(c);
	if (c->token->type == '?') {
		if (c->memory) {

		} else {
			c->memory_size += sizeof(struct cgl_lng_ternary_t);
		}
		c->token++;
		cgl_syntax_exp(c);
		if (c->token->type != ':') {
			c->error = "";
			return 0;
		}
		c->token++;
		cgl_syntax_exp(c);
	}
	return left;
}

static struct cgl_lng_exp_t* cgl_syntax_or(struct cgl_syntax_t* c) {
	struct cgl_lng_exp_t* left = cgl_syntax_and(c);
	if (c->token->type == CGL_LEXICAL_OR) {
		if (c->memory) {

		} else {
			c->memory_size += sizeof(struct cgl_lng_or_t);
		}
		c->token++;
		cgl_syntax_exp(c);
	}
	return left;
}

static struct cgl_lng_exp_t* cgl_syntax_and(struct cgl_syntax_t* c) {
	struct cgl_lng_exp_t* left = cgl_syntax_inclusive(c);
	if (c->token->type == CGL_LEXICAL_AND) {
		if (c->memory) {

		} else {
			c->memory_size += sizeof(struct cgl_lng_and_t);
		}
		c->token++;
		cgl_syntax_exp(c);
	}
	return left;
}

static struct cgl_lng_exp_t* cgl_syntax_inclusive(struct cgl_syntax_t* c) {
	struct cgl_lng_exp_t* left = cgl_syntax_exclusive(c);
	if (c->token->type == '|') {
		if (c->memory) {

		} else {
			c->memory_size += sizeof(struct cgl_lng_or_bit_t);
		}
		c->token++;
		cgl_syntax_exp(c);
	}
	return left;
}

static struct cgl_lng_exp_t* cgl_syntax_exclusive(struct cgl_syntax_t* c) {
	struct cgl_lng_exp_t* left = cgl_syntax_and_bit(c);
	if (c->token->type == '^') {
		if (c->memory) {

		} else {
			c->memory_size += sizeof(struct cgl_lng_exclusive_t);
		}
		c->token++;
		cgl_syntax_exp(c);
	}
	return left;
}

static struct cgl_lng_exp_t* cgl_syntax_and_bit(struct cgl_syntax_t* c) {
	struct cgl_lng_exp_t* left = cgl_syntax_equal(c);
	if (c->token->type == '&') {
		if (c->memory) {

		} else {
			c->memory_size += sizeof(struct cgl_lng_and_bit_t);
		}
		c->token++;
		cgl_syntax_exp(c);
	}
	return left;
}

static struct cgl_lng_exp_t* cgl_syntax_equal(struct cgl_syntax_t* c) {
	struct cgl_lng_exp_t* left = cgl_syntax_compare(c);
	switch (c->token->type) {
	case CGL_LEXICAL_EQUAL_EQUAL :
		if (c->memory) {

		} else {
			c->memory_size += sizeof(struct cgl_lng_equal_t);
		}
		c->token++;
		cgl_syntax_exp(c);
		break;
	case CGL_LEXICAL_DIFERENT :
		if (c->memory) {

		} else {
			c->memory_size += sizeof(struct cgl_lng_notequal_t);
		}
		c->token++;
		cgl_syntax_exp(c);
		break;
	}
	return left;
}

static struct cgl_lng_exp_t* cgl_syntax_compare(struct cgl_syntax_t* c) {
	struct cgl_lng_exp_t* left = cgl_syntax_shift(c);
	switch (c->token->type) {
	case '>' :
		if (c->memory) {

		} else {
			c->memory_size += sizeof(struct cgl_lng_greater_t);
		}
		c->token++;
		cgl_syntax_exp(c);
		break;
	case '<' :
		if (c->memory) {

		} else {
			c->memory_size += sizeof(struct cgl_lng_lower_t);
		}
		c->token++;
		cgl_syntax_exp(c);
		break;
	case CGL_LEXICAL_GREATER_EQUAL :
		if (c->memory) {

		} else {
			c->memory_size += sizeof(struct cgl_lng_greater_equal_t);
		}
		c->token++;
		cgl_syntax_exp(c);
		break;
	case CGL_LEXICAL_LOWER_EQUAL :
		if (c->memory) {

		} else {
			c->memory_size += sizeof(struct cgl_lng_lower_equal_t);
		}
		c->token++;
		cgl_syntax_exp(c);
		break;
	}
	return left;
}

static struct cgl_lng_exp_t* cgl_syntax_shift(struct cgl_syntax_t* c) {
	struct cgl_lng_exp_t* left = cgl_syntax_sum_sub(c);
	switch (c->token->type) {
	case CGL_LEXICAL_GREATER_GREATER :
		if (c->memory) {

		} else {
			c->memory_size += sizeof(struct cgl_lng_shift_right_t);
		}
		c->token++;
		cgl_syntax_exp(c);
		break;
	case CGL_LEXICAL_LOWER_LOWER :
		if (c->memory) {

		} else {
			c->memory_size += sizeof(struct cgl_lng_shift_left_t);
		}
		c->token++;
		cgl_syntax_exp(c);
		break;
	}
	return left;
}

static struct cgl_lng_exp_t* cgl_syntax_sum_sub(struct cgl_syntax_t* c) {
	struct cgl_lng_exp_t* left = cgl_syntax_mul_div_mod(c);
	switch (c->token->type) {
	case '+' :
		if (c->memory) {

		} else {
			c->memory_size += sizeof(struct cgl_lng_sum_t);
		}
		c->token++;
		cgl_syntax_exp(c);
		break;
	case '-' :
		if (c->memory) {

		} else {
			c->memory_size += sizeof(struct cgl_lng_sub_t);
		}
		c->token++;
		cgl_syntax_exp(c);
		break;
	}
	return left;
}

static struct cgl_lng_exp_t* cgl_syntax_mul_div_mod(struct cgl_syntax_t* c) {
	struct cgl_lng_exp_t* left = cgl_syntax_unary(c);
	switch (c->token->type) {
	case '*' :
		if (c->memory) {

		} else {
			c->memory_size += sizeof(struct cgl_lng_mul_t);
		}
		c->token++;
		cgl_syntax_exp(c);
		break;
	case '/' :
		if (c->memory) {

		} else {
			c->memory_size += sizeof(struct cgl_lng_div_t);
		}
		c->token++;
		cgl_syntax_exp(c);
		break;
	case '%' :
		if (c->memory) {

		} else {
			c->memory_size += sizeof(struct cgl_lng_mod_t);
		}
		c->token++;
		cgl_syntax_exp(c);
		break;
	}
	return left;
}

static struct cgl_lng_exp_t* cgl_syntax_unary(struct cgl_syntax_t* c) {
	switch (c->token->type) {
	case '!' :
		return cgl_syntax_unary_not(c);
	case CGL_LEXICAL_SUM_SUM :
		return cgl_syntax_unary_pre_inc(c);
	case CGL_LEXICAL_SUB_SUB :
		return cgl_syntax_unary_pre_dec(c);
	default :
		return cgl_syntax_literal(c);
	}
}

static struct cgl_lng_exp_t* cgl_syntax_unary_not(struct cgl_syntax_t* c) {
	struct cgl_lng_not_t* node;
	if (c->memory) {
		node = (struct cgl_lng_not_t*) c->memory;
		node->classid = CGL_LNG_NOT_CLASSID;
		c->memory += sizeof(struct cgl_lng_not_t);
	} else {
		node = (void*) 1;
		c->memory_size += sizeof(struct cgl_lng_not_t);
	}
	c->token++;
	struct cgl_lng_exp_t* left = cgl_syntax_exp(c);
	if (c->memory) {
		node->left = left;
	} else {
		if (!left) {
			return 0;
		}
	}
	return (struct cgl_lng_exp_t*) node;
}

static struct cgl_lng_exp_t* cgl_syntax_unary_pre_inc(struct cgl_syntax_t* c) {
	struct cgl_lng_pre_inc_t* node;
	if (c->memory) {
		node = (struct cgl_lng_pre_inc_t*) c->memory;
		node->classid = CGL_LNG_PRE_INC_CLASSID;
		c->memory += sizeof(struct cgl_lng_pre_inc_t);
	} else {
		node = (void*) 1;
		c->memory_size += sizeof(struct cgl_lng_pre_inc_t);
	}
	c->token++;
	struct cgl_lng_exp_t* left = cgl_syntax_exp(c);
	if (c->memory) {
		node->left = left;
	} else {
		if (!left) {
			return 0;
		}
	}
	return (struct cgl_lng_exp_t*) node;
}

static struct cgl_lng_exp_t* cgl_syntax_unary_pre_dec(struct cgl_syntax_t* c) {
	struct cgl_lng_pre_dec_t* node;
	if (c->memory) {
		node = (struct cgl_lng_pre_dec_t*) c->memory;
		node->classid = CGL_LNG_PRE_DEC_CLASSID;
		c->memory += sizeof(struct cgl_lng_pre_dec_t);
	} else {
		node = (void*) 1;
		c->memory_size += sizeof(struct cgl_lng_pre_dec_t);
	}
	c->token++;
	struct cgl_lng_exp_t* left = cgl_syntax_exp(c);
	if (c->memory) {
		node->left = left;
	} else {
		if (!left) {
			return 0;
		}
	}
	return (struct cgl_lng_exp_t*) node;
}

static struct cgl_lng_exp_t* cgl_syntax_literal(struct cgl_syntax_t* c) {
	switch (c->token->type) {
	case CGL_LEXICAL_TRUE :
		return (struct cgl_lng_exp_t*) &CGL_LNG_BOOL_TRUE;
	case CGL_LEXICAL_FALSE :
		return (struct cgl_lng_exp_t*) &CGL_LNG_BOOL_FALSE;
	case CGL_LEXICAL_INT_VALUE :
		return cgl_syntax_literal_int(c);
	case CGL_LEXICAL_HEX_VALUE :
		return cgl_syntax_literal_hex(c);
	case CGL_LEXICAL_STRING_VALUE :
		return cgl_syntax_literal_str(c);
	case CGL_LEXICAL_ID :
		return cgl_syntax_literal_id(c);
	case '(' :
		return cgl_syntax_literal_wrap(c);
	default :
		c->error = "expected <number>\n";
		return 0;
	}
}

static struct cgl_lng_exp_t* cgl_syntax_literal_int(struct cgl_syntax_t* c) {
	struct cgl_lng_int_t* node;
	if (c->memory) {
		node = (struct cgl_lng_int_t*) c->memory;
		node->classid = CGL_LNG_INT_CLASSID;
		c->memory += sizeof(struct cgl_lng_int_t);
	} else {
		node = (void*) 1;
		c->memory_size += sizeof(struct cgl_lng_int_t);
	}
	if (c->memory) {
		node->token = c->token;
		node->value = cgl_lng_nuchars_to_int(c->text + node->token->begin, node->token->size);
	}
	c->token++;
	return (struct cgl_lng_exp_t*) node;
}

static struct cgl_lng_exp_t* cgl_syntax_literal_str(struct cgl_syntax_t* c) {
	struct cgl_lng_str_t* node;
	if (c->memory) {
		node = (struct cgl_lng_str_t*) c->memory;
		node->classid = CGL_LNG_STR_CLASSID;
		c->memory += sizeof(struct cgl_lng_str_t);
	} else {
		node = (void*) 1;
		c->memory_size += sizeof(struct cgl_lng_str_t);
	}
	if (c->memory) {
		node->token = c->token;
	}
	c->token++;
	return (struct cgl_lng_exp_t*) node;
}

static struct cgl_lng_exp_t* cgl_syntax_literal_hex(struct cgl_syntax_t* c) {
	struct cgl_lng_int_t* node;
	if (c->memory) {
		node = (struct cgl_lng_int_t*) c->memory;
		node->classid = CGL_LNG_INT_CLASSID;
		c->memory += sizeof(struct cgl_lng_int_t);
	} else {
		node = (void*) 1;
		c->memory_size += sizeof(struct cgl_lng_int_t);
	}
	if (c->memory) {
		node->token = c->token;
		node->value = cgl_hex_to_uint(c->text + node->token->begin, node->token->size);
	}
	c->token++;
	return (struct cgl_lng_exp_t*) node;
}

static struct cgl_lng_exp_t* cgl_syntax_literal_id(struct cgl_syntax_t* c) {
	struct cgl_lng_id_t* node;
	if (c->memory) {
		node = (struct cgl_lng_id_t*) c->memory;
		node->classid = CGL_LNG_ID_CLASSID;
		c->memory += sizeof(struct cgl_lng_id_t);
	} else {
		node = (void*) 1;
		c->memory_size += sizeof(struct cgl_lng_id_t);
	}
	if (c->memory) {
		node->token = c->token;
	}
	c->token++;
	return (struct cgl_lng_exp_t*) node;
}

static struct cgl_lng_exp_t* cgl_syntax_literal_wrap(struct cgl_syntax_t* c) {
	c->token++;
	struct cgl_lng_exp_t* value = cgl_syntax_exp(c);
	if (!value) {
		return 0;
	}
	if (c->token->type != ')') {
		c->error = "expected ')'\n";
		return 0;
	}
	c->token++;
	return value;
}

static unsigned int cgl_hex_to_uint(unsigned char* text, unsigned int len) {
	text += 2;
	len -= 2;
	unsigned int value = 0;
	if (len) while (len) {
		char c = *(text++);
		if (c != '0') {
			unsigned int pow16;
			switch (c) {
			case '1' :
				pow16 = 1;
				break;
			case '2' :
				pow16 = 2;
				break;
			case '3' :
				pow16 = 3;
				break;
			case '4' :
				pow16 = 4;
				break;
			case '5' :
				pow16 = 5;
				break;
			case '6' :
				pow16 = 6;
				break;
			case '7' :
				pow16 = 7;
				break;
			case '8' :
				pow16 = 8;
				break;
			case '9' :
				pow16 = 9;
				break;
			case 'a' :
			case 'A' :
				pow16 = 10;
				break;
			case 'b' :
			case 'B' :
				pow16 = 11;
				break;
			case 'c' :
			case 'C' :
				pow16 = 12;
				break;
			case 'd' :
			case 'D' :
				pow16 = 13;
				break;
			case 'e' :
			case 'E' :
				pow16 = 14;
				break;
			case 'f' :
			case 'F' :
				pow16 = 15;
				break;
			default :
				return 0xFFFFFFFF;
			}
			value += pow16 << ((len - 1) * 4);
		}
		len--;
	}
	return value;
}

//static int cgl_str_to_int(unsigned char* text, unsigned int len) {
//	if (len <= 0) {
//		return 0;
//	}
//	int value = 0;
//	unsigned char isNegative = 0;
//	if (text[0] == '-') {
//		isNegative = 1;
//		text++;
//		len--;
//	}
//	while (len) {
//		char c = *(text++);
//		if (c != '0') {
//			unsigned int pow10;
//			switch (len) {
//			case 1 :
//				pow10 = 1;
//				break;
//			case 2 :
//				pow10 = 10;
//				break;
//			case 3 :
//				pow10 = 100;
//				break;
//			case 4 :
//				pow10 = 1000;
//				break;
//			case 5 :
//				pow10 = 10000;
//				break;
//			case 6 :
//				pow10 = 100000;
//				break;
//			case 7 :
//				pow10 = 1000000;
//				break;
//			case 8 :
//				pow10 = 10000000;
//				break;
//			case 9 :
//				pow10 = 100000000;
//				break;
//			case 10 :
//				pow10 = 1000000000;
//				break;
//			default :
//				return 2147483647;
//			}
//			value += (c - '0') * pow10;
//		}
//		len--;
//	}
//	if (isNegative) {
//		value = -value;
//	}
//	return value;
//}

int cgl_lng_nuchars_to_int(unsigned char *s, unsigned int size) {
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
