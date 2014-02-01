#ifndef TASKING_H
#define TASKING_H

#include <isr.h>

typedef struct {
	//u32 ds;
	u32 edi, esi, ebp, edx, ecx, ebx, eax;
	u32 eip, cs, eflags, useresp, ss;
} irq0_regs_t;

typedef struct task {
	u32 esp, ebp;
	u32 eip, eflags;
	u32 eax, ebx, ecx, edx, esi, edi;
	u32 cr3;

	struct task *prev;
	struct task *next;

	u32 id;
	bool suspended;
} task_t;

extern void new_irq0(void);

void init_tasking(void);
u32 switch_task(irq0_regs_t context);
task_t *LoadProcess(char*);
void KillProcess(task_t *task);
void KillCurrent(void);

u32 read_eip(void);

#endif
