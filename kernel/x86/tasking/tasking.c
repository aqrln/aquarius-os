#include <kernel.h>
#include "tasking.h"

#include <heap.h>
#include <panic.h>
#include <tty.h>
#include <descriptor_tables.h>
#include <paging.h>
#include <fs.h>
#include <mem.h>

static task_t * current_task;
static u32 next_id;

void init_tasking(void)
{
	idt_set_gate(32, (u32)new_irq0 , 0x08, 0x8E);
	next_id = 0;
}

void KillTask(task_t *task)
{
	task->prev->next = task->next;
	task->next->prev = task->prev;
	// TODO: clean up memory
}

void KillCurrent(void)
{
	task_t *next = current_task->next;
	if (next == current_task) {
		panic("No processes left.");
	}
	KillTask(current_task);
	current_task = next;
}

/*
   Loads a new process
*/
task_t *LoadProcess(char *fname)
{
	// Load the executable
	fs_node_t *file = open(fname, O_READ);
	if (!file) {
		tty_attr(LIGHT_RED);
		tty_puts("File not found!\n");
		tty_attr(WHITE);
		return 0;
	}
	u32 size = file->size;
	//u8 *buffer = (u8*) kmalloc(size);
	//fs_read(file, 0, 0xFFFFFFFF, buffer);
	//close(file);

	// create a new task structure
	task_t *next_task = (task_t *) kmalloc(sizeof(task_t));
	next_task->eax = next_task->ebx = next_task->ecx = next_task->edx = 0;
	next_task->esi = next_task->edi = 0;
	next_task->eip = 0x80000000;
	next_task->eflags = 0x200; // IF
	next_task->id = next_id++;
	next_task->suspended = false;
	
	// insert it into the ready queue
	if (current_task == NULL) {
		current_task = next_task;
		next_task->next = next_task;
		next_task->prev = next_task;
	} else {
		task_t *task;
		for (task = current_task; task->next != current_task; task = task->next) /* null body */ ;
		next_task->next = current_task;
		next_task->prev = task;
		task->next = next_task;
	}

	// create and switch to a new address space
	u32 curr_cr3 = read_cr3();
	next_task->cr3 = new_pagedir();
	write_cr3(next_task->cr3);

	// allocate memory for the process
	u32 npages = size / 0x1000; // number of pages to allocate
	if (size & 0xFFF) {
		// size of the file cannot be divided into integer number of 0x1000-byte chunks
		npages++;
	}
	u32 addr, i;
	for (addr = 0x80000000, i = 0; i < npages; i++, addr += 0x1000) {
		// map a new page
		map_p(addr, PDE_PRESENT | PDE_READWRITE | PDE_USER);
	}

	// copy the executable image
	//memcpy((void*)0x80000000, buffer, size);
	fs_read(file, 0, size, (u8*)0x80000000);
	close(file);

	// return to a previous address space
	write_cr3(curr_cr3);

	// Cleanup
	//kfree(buffer);

	return next_task;
}

u32 switch_task(irq0_regs_t context)
{
	task_t *next_task = current_task;
	do {
		next_task = next_task->next;
		if (next_task == current_task)
			return 0;
	} while (next_task->suspended == true);

	current_task->eax = context.eax;
	current_task->ebx = context.ebx;
	current_task->ecx = context.ecx;
	current_task->edx = context.edx;
	current_task->esi = context.esi;
	current_task->edi = context.edi;
	current_task->esp = context.useresp;
	current_task->ebp = context.ebp;
	current_task->eip = context.eip;
	current_task->eflags = context.eflags;

	context.eax = next_task->eax;
	context.ebx = next_task->ebx;
	context.ecx = next_task->ecx;
	context.edx = next_task->edx;
	context.esi = next_task->esi;
	context.edi = next_task->edi;
	context.useresp = next_task->esp;
	context.ebp = next_task->ebp;
	context.eip = next_task->eip;
	context.eflags = next_task->eflags;

	current_task = next_task;

	return next_task->cr3;
}
