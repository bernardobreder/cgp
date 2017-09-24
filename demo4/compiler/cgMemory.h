#pragma once

class cgMemory
{

public:

	unsigned char* _first;

	unsigned char* _next;

	size_t _size, _has;

	cgMemory(void* buffer, size_t size);

	~cgMemory();

	void* alloc(size_t size);

	size_t size();

	void reset();

};

