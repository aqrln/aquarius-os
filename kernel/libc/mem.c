#include <kernel.h>
#include "mem.h"

void memcpy(void* dest, void* src, u32 len)
{
	register u8 *d = (u8 *) dest, *s = (u8 *) src;
	while (len--)
		*d++ = *s++;
}

void memset(void *dest, u8 val, u32 len)
{
	register u8* d = (u8 *) dest;
	while (len--)
		*d++ = val;
}

void zeromeml(u32 *mem, u32 count)
{
	while (--count)
		*mem++ = 0;
}
