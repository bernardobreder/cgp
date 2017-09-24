#ifndef CGL_LEXASM_H
#define CGL_LEXASM_H

#include "platform.h"

struct cgl_token_asm_t {
	uint36 begin;
	uint32 size;
	uint16 type;
	uint36 line;
};

#define CGL_LEXASM_EOF 0
#define CGL_LEXASM_ID 300
#define CGL_LEXASM_STRING 301
#define CGL_LEXASM_NUMBER 302
#define CGL_LEXASM_HEX 303

#define CGL_LEXASM_PROLOG 500
#define CGL_LEXASM_LEAVE 501
#define CGL_LEXASM_RET 502
#define CGL_LEXASM_IMOVA 503
#define CGL_LEXASM_IMOVB 504
#define CGL_LEXASM_ISUM 505
#define CGL_LEXASM_ISUB 506
#define CGL_LEXASM_IMUL 507
#define CGL_LEXASM_IDIV 508

#define CGL_LEXASM_SMAX 400
#define CGL_LEXASM_SDEF 401
#define CGL_LEXASM_SMOVA 402
#define CGL_LEXASM_SMOVB 403
#define CGL_LEXASM_SLEN 405

int cgl_asm_eval(const unsigned char* text);

unsigned char* cgl_asm_to_vm(const unsigned char* text, const struct cgl_token_asm_t* tokens, unsigned int token_size, unsigned int* byte_size);

int cgl_asm(const unsigned char* text, const struct cgl_token_asm_t* tokens, unsigned int token_size);

struct cgl_token_asm_t* cgl_lexasm(const unsigned char* text, unsigned int* token_size);

void cgl_lexasm_test();

#endif
