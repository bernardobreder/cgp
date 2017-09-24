#pragma once

#include "cgException.h"
#include "cgToken.h"

class cgLexer
{
public:

	cgToken* _first;

	cgToken* _next;

	cgToken* _last;

	cgException* exception;

	cgLexer(const char* text);
	
	~cgLexer();

};

