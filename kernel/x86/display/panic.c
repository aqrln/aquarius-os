#include <kernel.h>
#include "panic.h"
#include <tty.h>

extern u32 usermode_started;

void panic_vanilla(char *reason, char *file, u32 line)
{
	if (!usermode_started) {
		asm ("cli");
		tty_attr(RED_BG | BLACK);
		tty_puts("\nKernel Panic: ");
		tty_puts(reason);
		tty_puts("\nError encountered at ");
		tty_puts(file);
		tty_puts(", line ");
		tty_putdec(line);
		tty_putc('\n');
		asm ("hlt");
		for (;;);
	} else {
		tty_attr(RED_BG | BLACK);
		tty_puts("\nFatal error: ");
		tty_puts(reason);
		KillCurrent();
	}
}

void assert_panic(char *file, u32 line, char *fault)
{
	asm ("cli");
	tty_attr(RED_BG | BLACK);
	tty_puts("\nKernel panic: assertion failed at ");
	tty_puts(file);
	tty_puts(", line ");
	tty_putdec(line);
	tty_puts(": ");
	tty_puts(fault);
	tty_putc('\n');
	asm ("hlt");
	for (;;);
}

void warning(char *msg)
{
	tty_puts("\nwarning: ");
	tty_puts(msg);
	tty_putc('\n');
}
