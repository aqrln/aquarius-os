#include <kernel.h>
#include "multiboot.h"

#include <tty.h>
#include <descriptor_tables.h>
#include <timer.h>
#include <paging.h>
#include <heap.h>
#include <panic.h>
#include <tasking.h>
#include <fs.h>
#include <initrd.h>
#include <mem.h>

extern u32 phys_heap_end, phys_ram;

u32 usermode_started = 0;

void hal_main(multiboot_info_t* mbi)
{
	init_descriptor_tables();

	init_timer(100);

	tty_init();
	tty_puts("Console is initialized.\n");

	assert(mbi->mods_count > 0);
	u32 initrd_location = *((u32 *)(mbi->mods_addr));
	u32 initrd_end = *((u32 *)(mbi->mods_addr + 4));

	phys_heap_end = initrd_end;

	phys_ram = (mbi->mem_lower + mbi->mem_upper) * 1024;

	tty_puts("Initializing paging and VMM...\n");
	init_paging();
	tty_puts("Initializing heap...\n");
	init_heap();

	tty_puts("Initializing the filesystem...\n");
	init_fs(initrd_location);

	tty_puts("Starting the task manager...\n");
	init_tasking();

	tty_puts("Initializing system calls...\n");
	InitSyscalls();

	tty_puts("OK.\n\n");

	task_t *task = LoadProcess("/initrd/system");
	usermode_started = 1;
	write_cr3(task->cr3);
	void (*entry_point)(void) = (void (*)(void))0x80000000;
	entry_point();

	for (;;);
}

