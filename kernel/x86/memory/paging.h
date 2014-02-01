#ifndef PAGING_H
#define PAGING_H

#include "isr.h"

#define PDE_PRESENT       (1 << 0)
#define PDE_READWRITE     (1 << 1)
#define PDE_USER          (1 << 2)
#define PDE_WRITE_THROUGH (1 << 3)
#define PDE_DISABLE_CACHE (1 << 4)
#define PDE_ACCESSED      (1 << 5)
#define PDE_BIG           (1 << 7)

#define PTE_PRESENT       (1 << 0)
#define PTE_READWRITE     (1 << 1)
#define PTE_USER          (1 << 2)
#define PTE_WRITE_THROUGH (1 << 3)
#define PTE_DISABLE_CACHE (1 << 4)
#define PTE_ACCESSED      (1 << 5)
#define PTE_DIRTY         (1 << 6)
#define PTE_GLOBAL        (1 << 8)

#define VM_ALLOCATED      (1 << 9)
#define VM_COW            (1 << 10)

#define map_p(page, flags) map_page((page), pop_pf(), (flags))

void init_paging(void);
void page_fault(registers_t *);
void enable_paging(void);
void set_pde(u32, u32);
void set_pte(u32, u32, u32);
u32 get_pde(u32);
u32 get_pte(u32);
u32 get_phys_addr(u32);
void map_page(u32, u32, u32);
void push_pf(u32);
u32 pop_pf(void);
void create_ptable(u32, u32);
void unmap_page(u32, u32);

u32 align_address(u32);
u32 read_cr2(void);
u32 read_cr3(void);
u32 read_cr0(void);
void write_cr0(u32);
void write_cr3(u32);
void tlb_flush(void);

u32 new_pagedir(void);

#endif
