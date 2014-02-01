#include "aq.h"

int main()
{
	attr(LIGHT_BLUE);
	Write("/initrd/ contents:\n");
	attr(WHITE);
	Write("\tsystem\n\tautostart\n\tls\n\tdock\n\thello\n");
	return 0;
}
