#include <kernel.h>
#include <msr.h>
#include <tty.h>
#include <descriptor_tables.h>
#include <tasking.h>
#include <isr.h>
#include "syscalls.h"
#include <fs.h>
#include <timer.h>

#define STACK_SIZE 8*1024
//void *KernelStack;
u8 KernelStack[STACK_SIZE];

extern void set_kernel_stack(u32);
extern u32 timer_ticks;

volatile bool kbd_event = false;
void kbd_irq(registers_t *regs)
{
	kbd_event = true;
}

void InitSyscalls(void)
{
	//KernelStack = kmalloc(STACK_SIZE);
	u32 StackTop = (u32)KernelStack + STACK_SIZE - 4;
	set_kernel_stack(StackTop);

	WriteMSR(MSR_IA32_SYSENTER_CS, 0, 0xb); // 0x8 + 0x3
	WriteMSR(MSR_IA32_SYSENTER_ESP, 0, StackTop);
	WriteMSR(MSR_IA32_SYSENTER_EIP, 0, (u32)SyscallEntry);

	register_interrupt_handler(0x80, Int80hSyscall);
	register_interrupt_handler(0x21, kbd_irq);
}

fs_node_t *ird_root;

void SystemCall(u32 id, void *param, void *result)
{
	int i;
	struct dirent *node;

	switch (id) {
		case SYS_PUTS:
			tty_puts((char*)param);
			break;

		case SYS_EXEC:
			LoadProcess((char*)param);
			break;

		case 2:
			if (kbd_event) {
				kbd_event = false;
				*(bool*)result = true;
			} else {
				*(bool*)result = false;
			}
			break;

		case 3:
			*(int*)result = inb(*(int*)param);
			break;

		case 4:
			outb(*(int*)param, *(int*)result);
			break;

		case 5: // read dir
			i = *(int *)param;
			node = fs_readdir(ird_root, i);
			if (!node) {
				*(int *) result = 0;
			} else {
				*(char *)result = node->name;
			}
			break;

		case 6:
			tty_attr((int)param);
			break;

		case 7:
			*(int*)result = timer_ticks;
			break;

		default:
			break;
	}
}

void Int80hSyscall(registers_t *regs)
{
	SystemCall(regs->eax, (void*)regs->esi, (void*)regs->edi);
}
