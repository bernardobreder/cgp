#pragma once

class cgException
{

public:

	char* _text;

	cgException* _next;
	
	cgException(char* text);

	~cgException();
	
};

