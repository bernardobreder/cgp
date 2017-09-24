#include "cgUtil.h"
#include "cgLexer.h"

cgLexer::cgLexer(const char* text)
{
	char* _cur = (char*)text;
	while (*_cur) {
		char c = *_cur;
		while (c <= 32) {
			if (c <= 0) return;
			c = *(++_cur);
		}
		char c2 = _cur[1];
		char c3 = _cur[2];
		if (c == '\"') {
			char* begin = _cur;
			c = *(++_cur);
			while (c != '\"') {
				c = *(++_cur);
			}
			_cur++;
			char* end = _cur;
			size_t length = (end - 1) - (begin + 1);
			char* word = (char*)cgUtilMemoryAlloc(length + 1);
			cgUtilCharsCopy(word, begin + 1, length);
			word[length] = 0;
			_last = new cgToken(cgTokenTypeString, word, length, _last);
		}
		else if (c == '\'') {
			char* begin = _cur;
			c = *(++_cur);
			_cur++;
			char* end = _cur;
			char* word = (char*)cgUtilMemoryAlloc(2);
			word[0] = begin[1];
			word[1] = 0;
			_last = new cgToken(cgTokenTypeCharacter, word, 1, _last);
		}
		else if (c == '/' && c2 == '*') {
			_cur += 2;
			c = _cur[0]; c2 = _cur[1];
			while (c != '*' || c2 != '/') {
				_cur++;
				c = _cur[0]; c2 = _cur[1];
			}
			_cur += 2;
		}
		else if (c == '/' && c2 == '/') {
			_cur += 2;
			c = *_cur;
			while (c != '\n') c = *(++_cur);
			_cur++;
		}
		else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$') {
			char* begin = _cur;
			c = *(++_cur);
			while ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_' || c == '$') c = *(++_cur);
			unsigned char type = cgTokenTypeId;
			char* end = _cur;
			size_t length = (end - 1) - (begin + 1);
			switch (length) {
			case 2: {
				switch (begin[0]) {
				case 'd': if (begin[1] == 'o') type = cgTokenTypeDo; break;
				case 'i': if (begin[1] == 'f') type = cgTokenTypeIf; break;
				}
				break;
			}
			case 3: {
				switch (begin[0]) {
				case 'f': if (begin[1] == 'o' && begin[2] == 'r') type = cgTokenTypeFor; break;
				case 'i': if (begin[1] == 'n' && begin[2] == 't') type = cgTokenTypeInt; break;
				}
				break;
			}
			case 4: {
				switch (begin[0]) {
				case 'c': if (begin[1] == 'h' && begin[2] == 'a' && begin[3] == 'r') type = cgTokenTypeChar; break;
				case 'e': {
					if (begin[1] == 'l' && begin[2] == 's' && begin[3] == 'e') type = cgTokenTypeElse;
					else if (begin[1] == 'n' && begin[2] == 'u' && begin[3] == 'm') type = cgTokenTypeEnum;
					break;
				}
				case 'g':{
					if (begin[1] == 'o' && begin[2] == 't' && begin[3] == 'o') type = cgTokenTypeGoto;
					break;
				}
				case 'l':{
					if (begin[1] == 'o' && begin[2] == 'n' && begin[3] == 'g') type = cgTokenTypeLong;
					break;
				}
				case 't':{
					if (begin[1] == 'r' && begin[2] == 'u' && begin[3] == 'e') type = cgTokenTypeTrue;
					break;
				}
				case 'v':{
					if (begin[1] == 'o' && begin[2] == 'i' && begin[3] == 'd') type = cgTokenTypeVoid;
					break;
				}
				}
				break;
			}
			case 5: {
				switch (begin[0]) {
				case 'b':{
					if (begin[1] == 'r' && begin[2] == 'e' && begin[3] == 'a' && begin[4] == 'k') type = cgTokenTypeBreak;
					break;
				}
				case 'c':{
					if (begin[1] == 'o' && begin[2] == 'n' && begin[3] == 's' && begin[4] == 't') type = cgTokenTypeConst;
					break;
				}
				case 'f':{
					if (begin[1] == 'a' && begin[2] == 'l' && begin[3] == 's' && begin[4] == 'e') type = cgTokenTypeFalse;
					else if (begin[1] == 'l' && begin[2] == 'o' && begin[3] == 'a' && begin[4] == 't') type = cgTokenTypeFloat;
					break;
				}
				case 's':{
					if (begin[1] == 'h' && begin[2] == 'o' && begin[3] == 'r' && begin[4] == 't') type = cgTokenTypeShort;
					break;
				}
				case 'w':{
					if (begin[1] == 'h' && begin[2] == 'i' && begin[3] == 'l' && begin[4] == 'e') type = cgTokenTypeWhile;
					break;
				}
				}
				break;
			}
			case 6: {
				switch (begin[0]) {
				case 'd':{
					if (begin[1] == 'o' && begin[2] == 'u' && begin[3] == 'b' && begin[4] == 'l' && begin[5] == 'e') type = cgTokenTypeDouble;
					break;
				}
				case 'r':{
					if (begin[1] == 'e' && begin[2] == 't' && begin[3] == 'u' && begin[4] == 'r' && begin[5] == 'n') type = cgTokenTypeReturn;
					break;
				}
				case 's':{
					if (begin[1] == 'i' && begin[2] == 'z' && begin[3] == 'e' && begin[4] == 'o' && begin[5] == 'f') type = cgTokenTypeSizeof;
					else if (begin[1] == 't' && begin[2] == 'a' && begin[3] == 't' && begin[4] == 'i' && begin[5] == 'c') type = cgTokenTypeStatic;
					else if (begin[1] == 't' && begin[2] == 'r' && begin[3] == 'u' && begin[4] == 'c' && begin[5] == 't') type = cgTokenTypeStruct;
					else if (begin[1] == 'w' && begin[2] == 'i' && begin[3] == 't' && begin[4] == 'c' && begin[5] == 'h') type = cgTokenTypeSwitch;
					break;
				}
				}
				break;
			}
			case 8: {
				switch (begin[0]) {
				case 'c':{
					if (begin[1] == 'o' && begin[2] == 'n' && begin[3] == 't' && begin[4] == 'i' && begin[5] == 'n' && begin[6] == 'u' && begin[7] == 'e') type = cgTokenTypeContinue;
					break;
				}
				case 'v':{
					if (begin[1] == 'o' && begin[2] == 'l' && begin[3] == 'a' && begin[4] == 't' && begin[5] == 'i' && begin[6] == 'l' && begin[7] == 'e') type = cgTokenTypeVolatile;
					break;
				}
				}
				break;
			}
			}
			char* word = (char*)cgUtilMemoryAlloc(length + 1);
			cgUtilCharsCopy(word, begin + 1, length);
			word[length] = 0;
			_last = new cgToken(type, word, length, _last);
		}
		else if ((c >= '0' && c <= '9') || (c == '.' && c2 >= '0' && c2 <= '9')) {
			char* begin = _cur;
			if (c == '0' && c2 == 'x') {
				_cur += 2;
				c = *_cur;
				while (c >= '0' && c <= '9') {
					c = *(++_cur);
				}
			}
			else {
				c = *(++_cur);
				bool dot = false;
				while ((c >= '0' && c <= '9') || c == '.') {
					if (c == '.') {
						if (dot) break;
						dot = true;
					}
					c = *(++_cur);
				}
			}
			char* end = _cur;
			size_t length = end - begin;
			char* word = (char*)cgUtilMemoryAlloc(length + 1);
			cgUtilCharsCopy(word, begin + 1, length);
			word[length] = 0;
			_last = new cgToken(cgTokenTypeNumber, word, length, _last);
		}
		else {
			char* begin = _cur;
			unsigned char type;
			type = *(_cur++);
			length = 1;
			switch (c) {
			case '.': {
				switch (c2) {
				case '.': {
					switch (c3) {
					case '.':
						type = CUL_LEXICAL_DOT3;
						length = 3;
						_cur += 2;
						break;
					}
					break;
				}
				}
				break;
			}
			case '=': {
				switch (c2) {
				case '=': {
					type = CUL_LEXICAL_EQUAL;
					length = 2;
					_cur++;
					break;
				}
				}
				break;
			}
			case '!': {
				switch (c2) {
				case '=': {
					type = CUL_LEXICAL_NOTEQUAL;
					length = 2;
					_cur++;
					break;
				}
				}
				break;
			}
			case '+': {
				switch (c2) {
				case '+': {
					type = CUL_LEXICAL_SUMSUM;
					length = 2;
					_cur++;
					break;
				}
				case '=': {
					type = CUL_LEXICAL_SUM_EQUAL;
					length = 2;
					_cur++;
					break;
				}
				}
				break;
			}
			case '-': {
				switch (c2) {
				case '-': {
					type = CUL_LEXICAL_SUBSUB;
					length = 2;
					_cur++;
					break;
				}
				case '=': {
					type = CUL_LEXICAL_SUB_EQUAL;
					length = 2;
					_cur++;
					break;
				}
				case '>': {
					type = CUL_LEXICAL_POINTER;
					length = 2;
					_cur++;
					break;
				}
				}
				break;
			}
			case '*': {
				switch (c2) {
				case '=': {
					type = CUL_LEXICAL_MUL_EQUAL;
					length = 2;
					_cur++;
					break;
				}
				}
				break;
			}
			case '/': {
				switch (c2) {
				case '=': {
					type = CUL_LEXICAL_DIV_EQUAL;
					length = 2;
					_cur++;
					break;
				}
				}
				break;
			}
			case '>': {
				switch (c2) {
				case '=': {
					type = CUL_LEXICAL_GREATER_EQUAL;
					length = 2;
					_cur++;
					break;
				}
				case '>': {
					type = CUL_LEXICAL_SHIFT_RIGHT;
					length = 2;
					_cur++;
					switch (c3) {
					case '=': {
						type = CUL_LEXICAL_SHIFT_RIGHT_EQUAL;
						length = 3;
						_cur++;
						break;
					}
					}
					break;
				}
				}
				break;
			}
			case '<': {
				switch (c2) {
				case '=': {
					type = CUL_LEXICAL_LOWER_EQUAL;
					length = 2;
					_cur++;
					break;
				}
				case '<': {
					type = CUL_LEXICAL_SHIFT_LEFT;
					length = 2;
					_cur++;
					switch (c3) {
					case '=': {
						type = CUL_LEXICAL_SHIFT_LEFT_EQUAL;
						length = 3;
						_cur++;
						break;
					}
					}
					break;
				}
				}
				break;
			}
			case '&': {
				switch (c2) {
				case '&': {
					type = CUL_LEXICAL_AND;
					length = 2;
					_cur++;
					switch (c3) {
					case '=': {
						type = CUL_LEXICAL_AND_EQUAL;
						length = 3;
						_cur++;
						break;
					}
					}
					break;
				}
				}
				break;
			}
			case '|': {
				switch (c2) {
				case '|': {
					type = CUL_LEXICAL_OR;
					length = 2;
					_cur++;
					switch (c3) {
					case '=': {
						type = CUL_LEXICAL_OR_EQUAL;
						length = 3;
						_cur++;
						break;
					}
					}
					break;
				}
				}
				break;
			}
			case '^': {
				switch (c2) {
				case '=': {
					type = CUL_LEXICAL_XOR_EQUAL;
					length = 2;
					_cur++;
					break;
				}
				}
				break;
			}
			}
			token = new CULToken(word, length, type);
		}
		if (token) {
			_tokens.push_back(token);
		}
	}
}

cgLexer::~cgLexer() {
	for (CULToken *token : _tokens) {
		delete token;
	}
}

void cgLexer::next() {
	if (_index >= _tokens.size()) return;
	_index++;
}

unsigned char cgLexer::look() {
	if (_index >= _tokens.size()) return CUL_LEXICAL_EOF;
	return _tokens[_index]->_type;
}

unsigned char cgLexer::eof() {
	return _index >= _tokens.size();
}

bool cgLexer::look(unsigned char code) {
	if (_index >= _tokens.size()) return false;
	return _tokens[_index]->_type == code;
}

bool cgLexer::look(unsigned char code, unsigned char next) {
	if (_index + next > _tokens.size()) return false;
	return _tokens[_index + next]->_type == code;
}

CULToken* cgLexer::token() {
	if (_index >= _tokens.size()) return &_eof;
	return _tokens[_index];
}

CULToken* cgLexer::token(long next) {
	if (_index + next >= _tokens.size()) return &_eof;
	return _tokens[_index + next];
}

int cgLexer::utf() {
	int c = *_cur;
	if (c <= 0x7F) {
		return c;
	}
	else if ((c >> 5) == 0x6) {
		int i2 = *(++_cur);
		if (i2 < 0) return -1;
		return (((c & 0x1F) << 6) + (i2 & 0x3F));
	}
	else {
		int i2 = *(++_cur);
		if (i2 < 0) return -1;
		int i3 = *(++_cur);
		if (i3 < 0) return -1;
		return (((c & 0xF) << 12) + ((i2 & 0x3F) << 6) + (i3 & 0x3F));
	}
}