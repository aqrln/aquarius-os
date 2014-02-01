#include <kernel.h>
#include "msr.h"

void ReadMSR(u32 msr, u32 *hi, u32 *lo)
{
	asm ("rdmsr" : "=a"(*lo), "=d"(*hi) : "c"(msr));
}

void WriteMSR(u32 msr, u32 hi, u32 lo)
{
	asm ("wrmsr" :: "a"(lo), "d"(hi), "c"(msr));
}
