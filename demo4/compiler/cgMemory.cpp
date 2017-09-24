#include <assert.h>
#include "cgUtil.h"
#include "cgMemory.h"

cgMemory::cgMemory(void* buffer, size_t size)
{
	_size = _has = size;
	_first = _next = (unsigned char*)buffer;
}


cgMemory::~cgMemory()
{
	cgUtilMemoryFree(_first);
	_first = _next = nullptr;
	_size = _has = 0;
}

void* cgMemory::alloc(size_t size)
{
	void* data = _next;
	_next += size;
	_has -= size;
	assert(_has >= 0);
	return data;
}

size_t cgMemory::size()
{
	return _size;
}

void cgMemory::reset()
{
	_next = _first;
	_has = _size;
}