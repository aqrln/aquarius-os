#include <kernel.h>
#include "paging.h"

#include <tty.h>
#include <panic.h>
#include <mem.h>
#include <isr.h>
#include <heap.h>
#include <tasking.h>

u32 *kernel_pagedir;

u32 phys_ram;

static u32 *pf_stack;
static u32 pf_stack_index, pf_stack_depth;

static u32 tmp_page;

static void init_pf_stack(void)
{
	tty_puts("Detected ");
	tty_putdec(phys_ram / (1024 * 1024) + 1);
	tty_puts(" MB of RAM\n");
	pf_stack = (u32 *) ph_alloc_a(phys_ram / 1024);
	pf_stack_depth = phys_ram / 4096;
	pf_stack_index = 0;

	u32 page;
	for (
			page = align_address(phys_ram) - 0x1000;
			page >= 0x400000;
			page -= 0x1000
		) 
	{
		push_pf(page);
	}
}

void push_pf(u32 address)
{
	u32 aligned = align_address(address);
	if (address != aligned)
		warning("push_page: address is not aligned - fixed");
	if (pf_stack_index > pf_stack_depth)
		panic("page frames stack is broken");
	pf_stack[pf_stack_index++] = aligned;
}

u32 pop_pf(void)
{
	if (!pf_stack_index)
		panic("Not enough memory!");
	return pf_stack[--pf_stack_index];
}

u32 * build_new_pagedir(void)
{
	u32 *new_pagedir = (u32 *)ph_alloc_a(0x1000);
	zeromeml(new_pagedir, 1024);
	new_pagedir[0x3FF] = (u32)new_pagedir | 0x3;
	return new_pagedir;
}

u32 * lomem_identity_map(void)
{
	u32 * pagetable = (u32 *)ph_alloc_a(0x1000);
	u32 i, address;
	for (i = 1, address = 0x1000; i < 1024; i++, address += 0x1000) {
		pagetable[i] = address | 0x3 | PTE_USER;
	}
	return pagetable;
}

void init_paging(void)
{
	register_interrupt_handler(14, page_fault);

	tmp_page = ph_alloc_a(4096);

	kernel_pagedir = build_new_pagedir();
	kernel_pagedir[0] = (u32)lomem_identity_map() | 0x3;

	init_pf_stack();

	enable_paging();

	u32 i;
	for (i = 1; i < 512; i++) {
		create_ptable(i, PDE_PRESENT | PDE_READWRITE);
	}
}

void page_fault(registers_t *regs)
{
	u32 cr2 = read_cr2();

	int present = regs->err_code & 1 ? 1 : 0;
	int write = regs->err_code & 2 ? 1 : 0;
	int user = regs->err_code & 4 ? 1 : 0;

	u32 pde = get_pde(cr2);
	
	if ((pde & PDE_PRESENT) == 0) {
		u32 pdindex = cr2 >> 22;
		u32 flags = (pde & 0xFFF) | PDE_PRESENT;
		create_ptable(pdindex, flags);
	} else {
		u32 pte = get_pte(cr2);

		if (!present && pte & VM_ALLOCATED) {
			// allocate a physical page frame
			u32 flags = pte & 0xFFF;
			flags &= ~VM_ALLOCATED;
			flags |= PTE_PRESENT;
			pte = pop_pf() | flags;
			set_pte(cr2 >> 22, (cr2 >> 12) & 0x3FF, pte);
			tlb_flush();
		} else if (write && pte & VM_COW) {
			// let's copy it
			u32 flags = (pte & 0xFFF & ~VM_COW) | PTE_READWRITE;
			u32 new_phys_address = pop_pf();
			map_page(tmp_page, get_phys_addr(cr2), 3);
			map_page(cr2, new_phys_address, flags);
			memcpy((u8*)cr2, (u8*)tmp_page, 0x1000);
			unmap_page(tmp_page, 0);
		} else {
			// unresolved
			tty_attr(LIGHT_RED);
			tty_puts("\n*** Page fault exception! ***\n");
			if (present)
				tty_puts("[present; ");
			else
				tty_puts("[not present; ");
			if (user)
				tty_puts("user; ");
			else
				tty_puts("supervisor; ");
			if (write)
				tty_puts("write]\n");
			else
				tty_puts("read]\n");
			tty_putc('<');
			tty_puthex(cr2);
			tty_putc('>');
			if (cr2 == 0)
				tty_puts(" - NULL POINTER");
			panic("Page fault.");
		}
	}
}

void enable_paging(void)
{
	write_cr3((u32)kernel_pagedir);
	write_cr0(read_cr0() | 0x80000000);
}

u32 get_pde(u32 addr)
{
	u32 index = addr >> 22;
	u32 *pd = (u32 *) 0xFFFFF000;

	return pd[index];
}

u32 get_pte(u32 addr)
{
	u32 pdindex = (addr >> 22);
	u32 ptindex = (addr >> 12) & 0x3FF;
	u32 *pt = (u32 *) 0xFFC00000 + 0x400 * pdindex;
	return pt[ptindex];
}

void set_pde(u32 index, u32 pde)
{
	u32 *pd = (u32 *) 0xFFFFF000;
	pd[index] = pde;
}

void set_pte(u32 pdindex, u32 ptindex, u32 pte)
{
	//u32 *pd = (u32 *) 0xFFFFF000;
	u32 *pt = (u32 *) 0xFFC00000 + 0x400 * pdindex;
	pt[ptindex] = pte;
}

u32 get_phys_addr(u32 addr)
{
	u32 pdindex = addr >> 22;
	u32 ptindex = (addr >> 12) & 0x3FF;

	u32 *pd = (u32 *) 0xFFFFF000;
	if ((pd[pdindex] & 1) == 0)
		return 0;

	u32 *pt = (u32 *) 0xFFC00000 + 0x400 * pdindex;
	if ((pt[ptindex] & 1) == 0)
		return 0;

	return (pt[ptindex] & 0xFFFFF000) | (addr & 0xFFF);
}

void map_page(u32 from, u32 to, u32 flags)
{
	u32 virtual_addr = align_address(from);
	u32 physical_addr = align_address(to);
	if (physical_addr != to)
		warning("map_page: physical address is not aligned - fixed");
	if (virtual_addr != from)
		warning("map_page: virtual address is not aligned - fixed");

	u32 pdindex = virtual_addr >> 22;
	u32 ptindex = (virtual_addr >> 12) & 0x3FF;

	u32 pde = get_pde(virtual_addr);
	if (pde == 0) {
		//u32 table = ph_alloc_a(0x1000);
		//memset((u8*)table, 0, 0x1000);
		//set_pde(pdindex, table | flags | 1);
		create_ptable(pdindex, flags);
	}

	if (to < 0x80000000) {
		// global page
		flags |= PTE_GLOBAL;
	}

	set_pte(pdindex, ptindex, physical_addr | flags | 1);

	tlb_flush();
}

void unmap_page(u32 page, u32 free_frame)
{
	if (free_frame)
		push_pf(get_phys_addr(page));
	map_page(page, 0, 0);
}

void create_ptable(u32 pdindex, u32 flags)
{
	flags |= 1 | PDE_USER;
	u32 new_table = pop_pf();
	map_page(0 /*tmp_page*/, new_table, 3);
	zeromeml((u32*)0 /*(u32*)tmp_page*/, 1024);
	unmap_page(0 /*tmp_page*/, 0);
	set_pde(pdindex, new_table | flags);
}

/*
   Create a new page directory
   Returns physical address of the pd
*/
u32 new_pagedir(void)
{
	u32 *pagedir = build_new_pagedir();
	u32 *curr_pd = (u32*) 0xFFFFF000;
	int i;
	for (i = 0; i < 0x200; i++) {
		pagedir[i] = curr_pd[i] | PDE_USER; // copy the pointers to the low-2GB pagetables
	}
	/*for (i = 0x200; i < 0x3FF; i++) {
		pagedir[i] = pop_pf() | PDE_PRESENT | PDE_READWRITE | PDE_USER;
	}*/
	return get_phys_addr((u32)pagedir);
}
