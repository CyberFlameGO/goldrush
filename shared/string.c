#include <stddef.h>
#include "string.h"

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void* memcpy(void *destination, const void *source, size_t len)
{
	// a naive memory copy
	unsigned char *dst_ptr = (unsigned char *)destination;
	const unsigned char *src_ptr = (const unsigned char *)source;
	for (size_t i = 0; i < len; i++)
	{
		dst_ptr[i] = src_ptr[i];
	}
	return destination;
}

void* memset(void *destination, int value, size_t len)
{
	// a naive memset()
	unsigned char *dst_ptr = (unsigned char *)destination;
	for (size_t i = 0; i < len; i++)
	{
		dst_ptr[i] = (unsigned char) value;
	}
	return destination;
}