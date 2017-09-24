#include "cgUtil.h"

#ifdef WIN32

#include <windows.h>
#include <string.h>

#else

#include <string.h>

#endif

void* cgUtilMemoryAlloc(size_t size)
{
#ifdef WIN32 
	return HeapAlloc(GetProcessHeap(), 0, size);
#else 
	return malloc(size);
#endif
}

void* cgUtilMemoryFree(void* data) {
	HeapFree(GetProcessHeap(), 0, data);
}

char* cgUtilCharsDup(const char* text)
{
#ifdef WIN32 
	return _strdup(text);
#else 
	return strdup(text);
#endif
}

inline char* cgUtilCharsCopy(char* dest, const char* src, size_t length)
{
#ifdef WIN32 
	CopyMemory(dest, src, length);
#else 
	strcpy
#endif
}