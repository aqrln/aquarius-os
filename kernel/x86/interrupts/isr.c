#include <kernel.h>
#include "isr.h"

#include <io.h>
#include <panic.h>
#include <tty.h>

isr_t interrupt_handlers[256];

void isr_handler(registers_t regs)
{
	isr_t handler = interrupt_handlers[(u8)(regs.int_no)];
	if (handler) {
		handler(&regs);
	} else {
		switch (regs.int_no) {
			case 0:
				panic("[#0] Division by zero");
				break;

			case 1:
				panic("[#1] Debug exception");
				break;

			case 2:
				panic("[#2] Non-Maskable Interrupt");
				break;

			case 3:
				panic("[#3] Breakpoint exception");
				break;

			case 4:
				panic("[#4] Overflow exception");
				break;

			case 5:
				panic("[#5] Bound range exceeded");
				break;

			case 6:
				panic("[#6] Invalid opcode");
				break;

			case 7:
				panic("[#7] No math coprocessor");
				break;

			case 8:
				panic("[#8] Double fault exception");
				break;

			case 9:
				panic("[#9] Coprocessor segment overrun");
				break;

			case 10:
				panic("[#10] Invalid TSS");
				break;

			case 11:
				tty_puts("\n\n[error code: ");
				tty_puthex(regs.err_code);
				tty_putc(']');
				panic("[#11] Segment not present");
				break;

			case 12:
				panic("[#12] Stack fault exception");
				break;

			case 13:
				tty_puts("\n\n[error code: ");
				tty_puthex(regs.err_code);
				tty_putc(']');
				panic("[#13] General protection fault");
				break;

			case 14:
				panic("[#14] Page fault exception");
				break;

			case 15:
				panic("[#15] Reserved");
				break;

			case 16:
				panic("[#16] FPU error");
				break;

			case 17:
				panic("[#17] Alignment check exception");
				break;

			case 18:
				panic("[#18] Machine check exception");
				break;

			case 19:
				panic("[#19] SIMD Floating Point exception");
				break;

			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
			case 27:
			case 28:
			case 29:
			case 30:
			case 31:
				panic("[#20-31] Reserved");
				break;

			default:
				warning("isr: null handler");
				break;
		}
	}
}

void irq_handler(registers_t regs)
{
	// Send an EOI (end of interrupt) signal to the PICs.
	// If this interrupt involved the slave.
	if (regs.int_no >= 40) {
		// Send reset signal to slave.
		outb(0xA0, 0x20);
	}
	// Send reset signal to master. (As well as slave, if necessary).
	outb(0x20, 0x20);

	if (interrupt_handlers[regs.int_no] != 0) {
		isr_t handler = interrupt_handlers[regs.int_no];
		handler(&regs);
	}
}

void register_interrupt_handler(u8 n, isr_t handler)
{
  interrupt_handlers[n] = handler;
}
