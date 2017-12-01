#include <stdio.h>

#include "Conventions.hpp"

void* CopyMemory(void *destination, const void *source, size_t length)
{
	return memcpy(destination, source, length);
}

int CompareMemory(const void *ptr1, const void *ptr2, size_t length)
{
	return memcmp(ptr1, ptr2, length);
}

Length GetStringLength(const char* string)
{
	return strlen(string);
}
