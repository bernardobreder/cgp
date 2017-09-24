#ifndef CGL_IO_H
#define CGL_IO_H

#include "platform.h"

#ifndef	SEEK_END
#define SEEK_END	(2)
#endif

#define cgl_io_int32_read_inline(VALUE, BYTES) \
if ((*(BYTES) & 0x80)) { \
VALUE = -((*((BYTES)++) & 0x7F) << 24); \
VALUE -= (*((BYTES)++) << 16); \
VALUE -= (*((BYTES)++) << 8); \
VALUE -= *((BYTES)++); \
} else { \
VALUE = (*((BYTES)++) << 24); \
VALUE += (*((BYTES)++) << 16); \
VALUE += (*((BYTES)++) << 8); \
VALUE += *((BYTES)++); \
}

#define cgl_io_uint32_read_inline(VALUE, BYTES) \
VALUE = (*((BYTES)++) << 24); \
VALUE += (*((BYTES)++) << 16); \
VALUE += (*((BYTES)++) << 8); \
VALUE += *((BYTES)++);

#define cgl_io_uint32_write_inline(BYTES, VALUE) \
*(BYTES)++ = ((VALUE) >> 24); \
*(BYTES)++ = ((VALUE) >> 16) & 0xFF; \
*(BYTES)++ = ((VALUE) >> 8) & 0xFF; \
*(BYTES)++ = (VALUE) & 0xFF;

#define cgl_io_int32_write_inline(BYTES, VALUE) \
if (VALUE < 0) { \
	*(BYTES)++ = (((-VALUE) >> 24) & 0x7F) + 0x80; \
	*(BYTES)++ = ((-VALUE) >> 16) & 0xFF; \
	*(BYTES)++ = ((-VALUE) >> 8) & 0xFF; \
	*(BYTES)++ = (-VALUE) & 0xFF; \
} else { \
	*(BYTES)++ = ((VALUE) >> 24); \
	*(BYTES)++ = ((VALUE) >> 16) & 0xFF; \
	*(BYTES)++ = ((VALUE) >> 8) & 0xFF; \
	*(BYTES)++ = (VALUE) & 0xFF; \
}

#define cgl_io_uint64_read_inline(VALUE, BYTES) \
VALUE = ((uint64)(*((BYTES)++)) << 56); \
VALUE += ((uint64)(*((BYTES)++)) << 48); \
VALUE += ((uint64)(*((BYTES)++)) << 40); \
VALUE += ((uint64)(*((BYTES)++)) << 32); \
VALUE += (*((BYTES)++) << 24); \
VALUE += (*((BYTES)++) << 16); \
VALUE += (*((BYTES)++) << 8); \
VALUE += *((BYTES)++);

#define cgl_io_uint64_write_inline(BYTES, VALUE) \
*(BYTES)++ = (((uint64)(VALUE)) >> 56); \
*(BYTES)++ = (((uint64)(VALUE)) >> 48); \
*(BYTES)++ = (((uint64)(VALUE)) >> 40); \
*(BYTES)++ = (((uint64)(VALUE)) >> 32); \
*(BYTES)++ = (((uint32)(VALUE)) >> 24); \
*(BYTES)++ = (((uint32)(VALUE)) >> 16) & 0xFF; \
*(BYTES)++ = (((uint32)(VALUE)) >> 8) & 0xFF; \
*(BYTES)++ = ((uint32)(VALUE)) & 0xFF;

char* cgl_io_tmp(const unsigned char* bytes, int36 size);

unsigned char* cgl_io_file(const char* filename);

void cgl_io_int_write(unsigned char* bytes, int value);

void cgl_io_uint32_write(unsigned char* bytes, unsigned int value);

unsigned int cgl_io_uint_read(const unsigned char* bytes);

void cgl_io_uint64_write(unsigned char* bytes, uint64 value);

uint64 cgl_io_ulong_read(unsigned char* bytes);

void cgl_io_double_write(unsigned char* bytes, double v);

double cgl_io_double_read(const unsigned char* bytes);

int cgl_io_string_to_int(char *s, long size);

uint32 cgl_io_string_to_uint(char *s, long size);

void cgl_io_chars_write(unsigned char* bytes, char* text, int64 len);

unsigned char* cgl_io_chars_read(unsigned char* bytes, int32 len);

char* cgl_io_uint_to_string(unsigned int value, char *digits, int base);

char* cgl_io_int_to_string(int value, char *digits, int base);

void cgl_io_double_to_integer(double x, int* exp, long long* frac);

double cgl_io_integer_to_double(int exp, long long frac);

#endif
