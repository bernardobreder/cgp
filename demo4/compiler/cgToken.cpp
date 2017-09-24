#include "cgUtil.h"
#include "cgToken.h"

cgToken::cgToken(unsigned char type, const char* word, size_t length, cgToken* next) {
	_word = cgUtilCharsDup(word);
	_length = length;
	_type = type;
	_next = next;
}

cgToken::~cgToken()
{
	if (_word) delete _word;
	if (_next) delete _next;
}

unsigned char cgToken::type() {
	return _type;
}

const char* cgToken::word() {
	return _word;
}

long cgToken::length() {
	return _length;
}

bool cgToken::eof() {
	return _type == 0;
}