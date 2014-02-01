#include <aquarius.h>
#include "aq.h"

void Write(char *text)
{
	SystemCall(0, text, 0);
}

void LoadProcess(char *fname)
{
	SystemCall(1, fname, 0);
}

char inportb(short port)
{
	int p = port, b;
	SystemCall(3, &p, &b);
	return (char)b;
}

void outportb(short port, char value)
{
	int p = port, b = value;
	SystemCall(4, &p, &b);
}


void attr(int a)
{
	SystemCall(6, (void*)a, 0);
}

