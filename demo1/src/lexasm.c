#include <stdlib.h>
#include <string.h>
#include "platform.h"
#include "lexasm.h"
#include "cg_io.h"
#include "vm.h"

int cgl_asm_eval(const unsigned char* text) {
	unsigned int token_size;
	struct cgl_token_asm_t* tokens = cgl_lexasm(text, &token_size);
	if (!tokens) {
		return 0;
	}
	uint32 byte_size;
	unsigned char* bytes = cgl_asm_to_vm(text, tokens, token_size, &byte_size);
	if (!bytes) {
		free(tokens);
		return -1;
	}
	int result = cgl_vm(bytes, byte_size);
	free(bytes);
	free(tokens);
	return result;
}

unsigned char* cgl_asm_to_vm(const unsigned char* text, const struct cgl_token_asm_t* tokens, unsigned int token_size, unsigned int* bytes_size) {
	unsigned int byte_size = 1;
	struct cgl_token_asm_t* token = (struct cgl_token_asm_t*) tokens;
	while (token->type != CGL_LEXASM_EOF) {
		switch ((token++)->type) {
		case CGL_LEXASM_PROLOG:
			byte_size++;
			break;
		case CGL_LEXASM_LEAVE:
			byte_size++;
			break;
		case CGL_LEXASM_RET:
			byte_size++;
			break;
		case CGL_LEXASM_IMOVA:
			if (token->type != CGL_LEXASM_NUMBER) {
				return 0;
			}
			token++;
			byte_size += 5;
			break;
		case CGL_LEXASM_IMOVB:
			if (token->type != CGL_LEXASM_NUMBER) {
				return 0;
			}
			token++;
			byte_size += 5;
			break;
		case CGL_LEXASM_ISUM:
			byte_size++;
			break;
		case CGL_LEXASM_ISUB:
			byte_size++;
			break;
		case CGL_LEXASM_IMUL:
			byte_size++;
			break;
		case CGL_LEXASM_IDIV:
			byte_size++;
			break;
		case CGL_LEXASM_SMAX:
			if (token->type != CGL_LEXASM_NUMBER) {
				return 0;
			}
			token++;
			byte_size += 5;
			break;
		case CGL_LEXASM_SDEF:
			if (token->type != CGL_LEXASM_STRING) {
				return 0;
			}
			byte_size += token->size - 2 + 5;
			token++;
			break;
		case CGL_LEXASM_SMOVA:
			if (token->type != CGL_LEXASM_NUMBER) {
				return 0;
			}
			token++;
			byte_size += 5;
			break;
		case CGL_LEXASM_SMOVB:
			if (token->type != CGL_LEXASM_NUMBER) {
				return 0;
			}
			token++;
			byte_size += 5;
			break;
		case CGL_LEXASM_SLEN:
			byte_size++;
			break;
		default:
			return 0;
		}
	}
	unsigned char* bytes = (unsigned char*) calloc(1, byte_size);
	register unsigned char* b = bytes;
	token = (struct cgl_token_asm_t*) tokens;
	int32 int32Value;
	uint32 uint32Value;
	char* string;
	while (token->type != CGL_LEXASM_EOF) {
		switch ((token++)->type) {
		case CGL_LEXASM_PROLOG:
			cgl_vm_op_prolog_inline(b);
			break;
		case CGL_LEXASM_LEAVE:
			cgl_vm_op_leave_inline(b);
			break;
		case CGL_LEXASM_RET:
			cgl_vm_op_return_inline(b);
			break;
		case CGL_LEXASM_IMOVA:
			int32Value = cgl_io_string_to_int((char*) text + token->begin, token->size);
			token++;
			cgl_vm_op_iloada_inline(b, int32Value);
			break;
		case CGL_LEXASM_IMOVB:
			int32Value = cgl_io_string_to_int((char*) text + token->begin, token->size);
			token++;
			cgl_vm_op_iloadc_inline(b, int32Value);
			break;
		case CGL_LEXASM_ISUM:
			cgl_vm_op_isum_inline(b);
			break;
		case CGL_LEXASM_ISUB:
			cgl_vm_op_isub_inline(b);
			break;
		case CGL_LEXASM_IMUL:
			cgl_vm_op_imul_inline(b);
			break;
		case CGL_LEXASM_IDIV:
			cgl_vm_op_idiv_inline(b);
			break;
		case CGL_LEXASM_SMAX:
			uint32Value = cgl_io_string_to_uint((char*) text + token->begin, token->size);
			token++;
			cgl_vm_op_smax_inline(b, uint32Value);
			break;
		case CGL_LEXASM_SDEF:
			uint32Value = token->size - 2;
			string = (char*) text + token->begin + 1;
			token++;
			cgl_vm_op_sdef_inline(b, string, uint32Value);
			break;
		case CGL_LEXASM_SMOVA:
			uint32Value = cgl_io_string_to_uint((char*) text + token->begin, token->size);
			token++;
			cgl_vm_op_sloada_inline(b, uint32Value);
			break;
		case CGL_LEXASM_SMOVB:
			uint32Value = cgl_io_string_to_uint((char*) text + token->begin, token->size);
			token++;
			cgl_vm_op_sloada_inline(b, uint32Value);
			break;
		case CGL_LEXASM_SLEN:
			cgl_vm_op_slen_inline(b);
			break;
		default:
			free(bytes);
			return 0;
		}
	}
	cgl_vm_op_system_eof_inline(b);
	*bytes_size = byte_size;
	return bytes;
}

struct cgl_token_asm_t* cgl_lexasm(const unsigned char* text, unsigned int* token_size) {
	unsigned size = 0;
	register unsigned char* pc = (unsigned char*) text;
	register unsigned char c = *pc;
	while (c) {
		if (c <= ' ') {
		} else if (c >= 'a' && c <= 'z') {
			long begin = pc - text;
			do {
				c = *(++pc);
			} while (c >= 'a' && c <= 'z');
			if ((pc - text) - begin > 0xFFFFFFFF) {
				return 0;
			}
			size++;
			pc--;
		} else if (c == '0' && pc[1] == 'x') {
			long begin = pc - text;
			pc++;
			c = *(++pc);
			while ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F')) {
				c = *(++pc);
			}
			if ((pc - text) - begin > 10) {
				return 0;
			}
			size++;
			pc--;
		} else if (c == '-' || (c >= '0' && c <= '9')) {
			long begin = pc - text;
			do {
				c = *(++pc);
			} while (c >= '0' && c <= '9');
			if ((pc - text) - begin > 20) {
				return 0;
			}
			size++;
			pc--;
		} else if (c == '\"' || c == '\'') {
			long begin = pc - text;
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
			if ((pc - text) - begin > 0xFFFFFFFF) {
				return 0;
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
	if (token_size) {
		*token_size = size;
	}
	struct cgl_token_asm_t* tokens = (struct cgl_token_asm_t*) malloc((size + 1) * sizeof(struct cgl_token_asm_t));
	if (!tokens) {
		return 0;
	}
	tokens[size].type = CGL_LEXASM_EOF;
	struct cgl_token_asm_t* token = tokens;
	unsigned int lin = 1;
	pc = (unsigned char*) text;
	c = *pc;
	while (c) {
		if (c <= ' ') {
			if (c == '\n') {
				lin++;
			}
		} else if (c >= 'a' && c <= 'z') {
			unsigned short type = CGL_LEXASM_ID;
			int36 begin = pc - text;
			do {
				c = *(++pc);
			} while (c >= 'a' && c <= 'z');
			uint32 size = (uint32) ((pc - text) - begin);
			char* word = (char*) text + begin;
			switch (word[0]) {
			case 'i':
				switch (size) {
				case 4:
					if (word[1] == 'a' && word[2] == 'd' && word[3] == 'd') {
						type = CGL_LEXASM_ISUM;
					} else if (word[1] == 's' && word[2] == 'u' && word[3] == 'b') {
						type = CGL_LEXASM_ISUB;
					} else if (word[1] == 'm' && word[2] == 'u' && word[3] == 'l') {
						type = CGL_LEXASM_IMUL;
					} else if (word[1] == 'd' && word[2] == 'i' && word[3] == 'v') {
						type = CGL_LEXASM_IDIV;
					}
					break;
				case 5:
					if (word[1] == 'm' && word[2] == 'o' && word[3] == 'v' && word[4] == 'a') {
						type = CGL_LEXASM_IMOVA;
					} else if (word[1] == 'm' && word[2] == 'o' && word[3] == 'v' && word[4] == 'b') {
						type = CGL_LEXASM_IMOVB;
					}
					break;
				}
				break;
			case 'l':
				switch (size) {
				case 5:
					if (word[1] == 'e' && word[2] == 'a' && word[3] == 'v' && word[4] == 'e') {
						type = CGL_LEXASM_LEAVE;
					}
					break;
				}
				break;
			case 'p':
				switch (size) {
				case 6:
					if (word[1] == 'r' && word[2] == 'o' && word[3] == 'l' && word[4] == 'o' && word[5] == 'g') {
						type = CGL_LEXASM_PROLOG;
					}
					break;
				}
				break;
			case 'r':
				switch (size) {
				case 3:
					if (word[1] == 'e' && word[2] == 't') {
						type = CGL_LEXASM_RET;
					}
					break;
				}
				break;
			case 's':
				switch (size) {
				case 4:
					if (word[1] == 'm' && word[2] == 'a' && word[3] == 'x') {
						type = CGL_LEXASM_SMAX;
					} else if (word[1] == 'd' && word[2] == 'e' && word[3] == 'f') {
						type = CGL_LEXASM_SDEF;
					} else if (word[1] == 'l' && word[2] == 'e' && word[3] == 'n') {
						type = CGL_LEXASM_SLEN;
					}
					break;
				case 5:
					if (word[1] == 'm' && word[2] == 'o' && word[3] == 'v' && word[4] == 'a') {
						type = CGL_LEXASM_SMOVA;
					} else if (word[1] == 'm' && word[2] == 'o' && word[3] == 'v' && word[4] == 'b') {
						type = CGL_LEXASM_SMOVB;
					}
					break;
				}
				break;
			}
			token->begin = begin;
			token->size = size;
			token->type = type;
			token->line = lin;
			token++;
			pc--;
		} else if (c == '0' && pc[1] == 'x') {
			unsigned short type = CGL_LEXASM_HEX;
			long begin = pc - text;
			pc++;
			do {
				c = *(++pc);
			} while ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'));
			token->begin = begin;
			token->size = (uint32) ((pc - text) - begin);
			token->type = type;
			token->line = lin;
			token++;
			pc--;
		} else if (c == '-' || (c >= '0' && c <= '9')) {
			unsigned short type = CGL_LEXASM_NUMBER;
			long begin = pc - text;
			do {
				c = *(++pc);
			} while (c >= '0' && c <= '9');
			token->begin = begin;
			token->size = (uint32) ((pc - text) - begin);
			token->type = type;
			token->line = lin;
			token++;
			pc--;
		} else if (c == '\"' || c == '\'') {
			int clin = lin;
			unsigned short type = CGL_LEXASM_STRING;
			long begin = pc - text;
			c = *(++pc);
			while ((c != '\"' && c != '\'') || pc[-1] == '\\') {
				if (c <= 0x7F) {
					if (c == '\n') {
						lin++;
					}
					c = *(++pc);
				} else if ((c >> 5) == 0x6) {
					pc++;
					c = *(++pc);
				} else {
					pc += 2;
					c = *(++pc);
				}
			}
			token->begin = begin;
			token->size = (uint32) ((pc - text) - begin + 1);
			token->type = type;
			token->line = clin;
			token++;
		} else if (c == '/' && pc[1] == '*') {
			pc += 3;
			c = *pc;
			while (c != '/' || pc[-1] != '*') {
				if (c <= 0x7F) {
					if (c == '\n') {
						lin++;
					}
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
			lin++;
		} else {
			token->begin = pc - text;
			token->size = 1;
			token->type = c;
			token->line = lin;
			token++;
		}
		c = *(++pc);
	}
	return tokens;
}
