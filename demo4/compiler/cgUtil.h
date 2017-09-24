#pragma once

inline void* cgUtilMemoryAlloc(size_t size);

inline void* cgUtilMemoryFree(void* data);

inline char* cgUtilCharsDup(const char* text);

inline char* cgUtilCharsCopy(char* dest, const char* src, size_t length);
