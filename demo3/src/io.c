//#include <stdlib.h>
//#include <stdio.h>
//#include <math.h>
//#include <string.h>
//#include "cgl.h"
//
//void cgl_int_write(unsigned char* bytes, int value) {
//	unsigned char neg = value < 0;
//	unsigned int uvalue = neg ? abs(value) : value;
//	*bytes++ = (neg ? 0x80 : 0) + ((uvalue >> 24) & 0x7F);
//	*bytes++ = (uvalue >> 16) & 0xFF;
//	*bytes++ = (uvalue >> 8) & 0xFF;
//	*bytes++ = (uvalue) & 0xFF;
//}
//
//int cgl_int_read(const unsigned char* bytes) {
//	unsigned char i1 = *(bytes++);
//	unsigned char i2 = *(bytes++);
//	unsigned char i3 = *(bytes++);
//	unsigned char i4 = *(bytes++);
//	if (i1 & 0x80) {
//		return -(((i1 - 0x80) << 24) + (i2 << 16) + (i3 << 8) + i4);
//	} else {
//		return (i1 << 24) + (i2 << 16) + (i3 << 8) + i4;
//	}
//}
//
//void cgl_long_write(unsigned char* bytes, unsigned long value) {
//	*bytes++ = (value >> 56) & 0xFF;
//	*bytes++ = (value >> 48) & 0xFF;
//	*bytes++ = (value >> 40) & 0xFF;
//	*bytes++ = (value >> 32) & 0xFF;
//	*bytes++ = (value >> 24) & 0xFF;
//	*bytes++ = (value >> 16) & 0xFF;
//	*bytes++ = (value >> 8) & 0xFF;
//	*bytes++ = (value) & 0xFF;
//}
//
//unsigned long cgl_long_read(unsigned char* bytes) {
//	return ((long) (*bytes++) << 56) + ((long) (*bytes++) << 48) + ((long) (*bytes++) << 40) + ((long) (*bytes++) << 32) + ((*bytes++) << 24) + ((*bytes++) << 16) + ((*bytes++) << 8) + (*bytes++);
//}
//
//void cgl_double_write(unsigned char* bytes, double v) {
//	int ebits = 11, fbits = 52;
//	int bias = (1 << (ebits - 1)) - 1;
//	unsigned char s;
//	double f;
//	int e;
//	if (isnan(v)) {
//		e = (1 << bias) - 1;
//		f = 1;
//		s = 0;
//	} else if (isinf(v)) {
//		e = (1 << bias) - 1;
//		f = 0;
//		s = (v < 0) ? 1 : 0;
//	} else if (v == 0) {
//		e = 0;
//		f = 0;
//		s = (isinf(1 / v) == -1) ? 1 : 0;
//	} else {
//		s = v < 0;
//		v = v < 0 ? -v : v;
//		if (v >= pow(2, 1 - bias)) {
//			double ln2 = 0.6931471805599453;
//			double aux = floor(log(v) / ln2);
//			double ln = aux > bias ? bias : aux;
//			e = (int) (ln + bias);
//			f = v * pow(2, fbits - ln) - pow(2, fbits);
//		} else {
//			e = 0;
//			f = v / pow(2, 1 - bias - fbits);
//		}
//	}
//	unsigned char bits[64];
//	int i, n, m, index = 0;
//	for (i = 0; i < fbits; i++) {
//		bits[index++] = ((int) f) % 2 == 1 ? 1 : 0;
//		f = floor(f / 2);
//	}
//	for (i = 0; i < ebits; i++) {
//		bits[index++] = e % 2 == 1 ? 1 : 0;
//		e = (int) floor(e / 2);
//	}
//	bits[index] = s;
//	for (n = 0; n < 8; n++) {
//		int value = 0;
//		for (m = 0; m < 8; m++) {
//			if (bits[n * 8 + m]) {
//				value += pow(2, m);
//			}
//		}
//		*bytes++ = (unsigned char) (value & 0xff);
//	}
//}
//
//double cgl_double_read(const unsigned char* bytes) {
//	int ebits = 11, fbits = 52;
//	unsigned char bits[64];
//	int n, i, j, index = 0;
//	for (i = 0; i < 8; i++) {
//		unsigned char b = *bytes++;
//		for (j = 0; j < 8; j++) {
//			bits[index++] = b % 2 == 1 ? 1 : 0;
//			b = b >> 1;
//		}
//	}
//	int bias = (1 << (ebits - 1)) - 1;
//	int s = bits[63] ? -1 : 1;
//	int e = 0;
//	for (n = 62; n >= 64 - ebits - 1; n--) {
//		if (bits[n]) {
//			e += pow(2, ebits - 1 - 62 + n);
//		}
//	}
//	long f = 0;
//	int imax = 64 - ebits - 2;
//	for (n = imax; n >= 0; n--) {
//		if (bits[n]) {
//			f += pow(2, n);
//		}
//	}
//	if (e == (1 << ebits) - 1) {
//		return f != 0 ? NAN : s * 1. / 0.;
//	} else if (e > 0) {
//		return s * pow(2, e - bias) * (1 + f / pow(2, fbits));
//	} else if (f != 0) {
//		return s * pow(2, -(bias - 1)) * (f / pow(2, fbits));
//	} else {
//		return s * 0;
//	}
//}
//
//int cgl_nuchars_to_int(unsigned char *s, unsigned int size) {
//	int sign = 1;
//	int n = *s;
//	if (n == '+' || n == '-') {
//		sign = n == '+' ? 1 : -1;
//		s++;
//	}
//	int c = *s;
//	for (n = 0; c && n < size; s++) {
//		n = (n * 10) + (c - '0');
//		c = *s;
//	}
//	return sign * n;
//}
//
//int cgl_chars_to_int(char *s) {
//	int sign = 1;
//	int n = *s;
//	if (n == '+' || n == '-') {
//		sign = n == '+' ? 1 : -1;
//		s++;
//	}
//	int c = *s;
//	for (n = 0; c; s++) {
//		n = (n * 10) + (c - '0');
//		c = *s;
//	}
//	return sign * n;
//}
//
//char *cgl_uint_to_string(unsigned int value, char *digits, int base) {
//	char *s, *p;
//	s = "0123456789abcdefghijklmnopqrstuvwxyz";
//	if (base == 0) base = 10;
//	if (digits == NULL || base < 2 || base > 36) return NULL;
//	if (value < (unsigned int) base) {
//		digits[0] = s[value];
//		digits[1] = '\0';
//	} else {
//		for (p = cgl_uint_to_string(value / ((unsigned int) base), digits, base); *p; p++)
//			;
//		cgl_uint_to_string(value % ((unsigned int) base), p, base);
//	}
//	return digits;
//}
//
//char *cgl_int_to_string(int value, char *digits, int base) {
//	char *d;
//	unsigned int u;
//	d = digits;
//	if (base == 0) base = 10;
//	if (digits == NULL || base < 2 || base > 36) return NULL;
//	if (value < 0) {
//		*d++ = '-';
//		u = -((unsigned) value);
//	} else u = value;
//	cgl_uint_to_string(u, d, base);
//	return digits;
//}
