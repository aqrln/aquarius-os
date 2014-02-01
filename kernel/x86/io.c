#include <kernel.h>
#include "io.h"

void outb(u16 port, u8 value)
{
	asm ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8 inb(u16 port)
{
	u8 ret;
	asm("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

u16 inw(u16 port)
{
	u16 ret;
	asm ("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}
