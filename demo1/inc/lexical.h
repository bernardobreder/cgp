#ifndef LEXICAL_H
#define LEXICAL_H

#include "platform.h"

#define CGL_LEXICAL_EOF 0
#define CGL_LEXICAL_FIRST CGL_LEXICAL_ID
#define CGL_LEXICAL_ID 300
#define CGL_LEXICAL_STRING 301
#define CGL_LEXICAL_NUMBER 302
#define CGL_LEXICAL_HEX 303
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

#define CGL_LEXICAL_AND_WORD 316
#define CGL_LEXICAL_BOOLEAN_WORD 317
#define CGL_LEXICAL_CLASS_WORD 318
#define CGL_LEXICAL_DO_WORD 319
#define CGL_LEXICAL_DOUBLE_WORD 320
#define CGL_LEXICAL_END_WORD 321
#define CGL_LEXICAL_EXTENDS_WORD 322
#define CGL_LEXICAL_FOR_WORD 323
#define CGL_LEXICAL_FALSE_WORD 324
#define CGL_LEXICAL_FLOAT_WORD 325
#define CGL_LEXICAL_IMPLEMENTS_WORD 326
#define CGL_LEXICAL_IMPORT_WORD 327
#define CGL_LEXICAL_INTERFACE_WORD 328
#define CGL_LEXICAL_INSTANCEOF_WORD 329
#define CGL_LEXICAL_INT_WORD 330
#define CGL_LEXICAL_IF_WORD 331
#define CGL_LEXICAL_LONG_WORD 332
#define CGL_LEXICAL_OR_WORD 333
#define CGL_LEXICAL_PACKAGE_WORD 334
#define CGL_LEXICAL_PUBLIC_WORD 335
#define CGL_LEXICAL_PRIVATE_WORD 336
#define CGL_LEXICAL_PROTECTED_WORD 337
#define CGL_LEXICAL_REPEAT_WORD 338
#define CGL_LEXICAL_STRING_WORD 339
#define CGL_LEXICAL_STATIC_WORD 340
#define CGL_LEXICAL_SYNCRONIZED_WORD 341
#define CGL_LEXICAL_TRUE_WORD 342
#define CGL_LEXICAL_THROW_WORD 343
#define CGL_LEXICAL_THROWS_WORD 344
#define CGL_LEXICAL_VOID_WORD 345
#define CGL_LEXICAL_WHILE_WORD 346
#define CGL_LEXICAL_RETURN_WORD 347

struct lng_token_t {
	uint64 begin;
	uint32 size;
	uint16 type;
	uint64 line;
	uint64 column;
	byte* word;
};

struct lng_token_t* cgl_lexical(const unsigned char* text, unsigned int* tokenSize);

void cgl_lexical_test();

#endif
