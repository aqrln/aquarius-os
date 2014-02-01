#include <kernel.h>
#include "descriptor_tables.h"

#include <heap.h>
#include <mem.h>
#include <io.h>

extern void gdt_flush(u32);
static void init_gdt(void);
static void gdt_set_gate(s32, u32, u32, u8, u8);

extern void idt_flush(u32);
static void init_idt(void);
//static void idt_set_gate(u8, u32, u16, u8);

extern void tss_flush();
static void write_tss(s32, u16, u32);

gdt_entry_t gdt_entries[5];
gdt_ptr_t   gdt_ptr;
idt_entry_t idt_entries[256];
idt_ptr_t   idt_ptr;
tss_entry_t tss_entry;

void init_descriptor_tables()
{
	init_gdt();
	init_idt();
}

static void init_gdt()
{
	gdt_ptr.limit = (sizeof(gdt_entry_t) * 6) - 1;
	gdt_ptr.base  = (u32)&gdt_entries;

	gdt_set_gate(0, 0, 0, 0, 0);                // Null segment
	gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // Code segment
	gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // Data segment
	gdt_set_gate(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // User mode code segment
	gdt_set_gate(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // User mode data segment
	write_tss(5, 0x10, kernel_stack_top);

	gdt_flush((u32)&gdt_ptr);
	tss_flush();
}

static void gdt_set_gate(s32 num, u32 base, u32 limit, u8 access, u8 gran)
{
	gdt_entries[num].base_low    = (base & 0xFFFF);
	gdt_entries[num].base_middle = (base >> 16) & 0xFF;
	gdt_entries[num].base_high   = (base >> 24) & 0xFF;

	gdt_entries[num].limit_low   = (limit & 0xFFFF);
	gdt_entries[num].granularity = (limit >> 16) & 0x0F;

	gdt_entries[num].granularity |= gran & 0xF0;
	gdt_entries[num].access      = access;
}

// Initialise our task state segment structure.
static void write_tss(s32 num, u16 ss0, u32 esp0)
{
	// Firstly, let's compute the base and limit of our entry into the GDT.
	u32 base = (u32) &tss_entry;
	u32 limit = base + sizeof(tss_entry);

	// Now, add our TSS descriptor's address to the GDT.
	gdt_set_gate(num, base, limit, 0xE9, 0x00);

	// Ensure the descriptor is initially zero.
	memset((u8*)&tss_entry, 0, sizeof(tss_entry));

	tss_entry.ss0 = ss0; // Set the kernel stack segment.
	tss_entry.esp0 = esp0; // Set the kernel stack pointer.

	// Here we set the cs, ss, ds, es, fs and gs entries in the TSS. These specify what
	// segments should be loaded when the processor switches to kernel mode.  Therefore
	// they are just our normal kernel code/data segments - 0x08 and 0x10 respectively,
	// but with the last two bits set, making 0x0b and 0x13. The setting of these bits
	// sets the RPL (requested privilege level) to 3, meaning that this TSS can be used
	// to switch to kernel mode from ring 3.
	tss_entry.cs = 0x0b;
	tss_entry.ss = tss_entry.ds = tss_entry.es = tss_entry.fs = tss_entry.gs = 0x13;
} 

void set_kernel_stack(u32 stack)
{
	tss_entry.esp0 = stack;
} 

static void init_idt()
{
	idt_ptr.limit = sizeof(idt_entry_t) * 256 -1;
	idt_ptr.base  = (u32)&idt_entries;

	memset((u8*)&idt_entries, 0, sizeof(idt_entry_t)*256);

	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x0);
	outb(0xA1, 0x0);

	idt_set_gate( 0, (u32)isr0 , 0x08, 0x8E);
	idt_set_gate( 1, (u32)isr1 , 0x08, 0x8E);
	idt_set_gate( 2, (u32)isr2 , 0x08, 0x8E);
	idt_set_gate( 3, (u32)isr3 , 0x08, 0x8E);
	idt_set_gate( 4, (u32)isr4 , 0x08, 0x8E);
	idt_set_gate( 5, (u32)isr5 , 0x08, 0x8E);
	idt_set_gate( 6, (u32)isr6 , 0x08, 0x8E);
	idt_set_gate( 7, (u32)isr7 , 0x08, 0x8E);
	idt_set_gate( 8, (u32)isr8 , 0x08, 0x8E);
	idt_set_gate( 9, (u32)isr9 , 0x08, 0x8E);
	idt_set_gate(10, (u32)isr10, 0x08, 0x8E);
	idt_set_gate(11, (u32)isr11, 0x08, 0x8E);
	idt_set_gate(12, (u32)isr12, 0x08, 0x8E);
	idt_set_gate(13, (u32)isr13, 0x08, 0x8E);
	idt_set_gate(14, (u32)isr14, 0x08, 0x8E);
	idt_set_gate(15, (u32)isr15, 0x08, 0x8E);
	idt_set_gate(16, (u32)isr16, 0x08, 0x8E);
	idt_set_gate(17, (u32)isr17, 0x08, 0x8E);
	idt_set_gate(18, (u32)isr18, 0x08, 0x8E);
	idt_set_gate(19, (u32)isr19, 0x08, 0x8E);
	idt_set_gate(20, (u32)isr20, 0x08, 0x8E);
	idt_set_gate(21, (u32)isr21, 0x08, 0x8E);
	idt_set_gate(22, (u32)isr22, 0x08, 0x8E);
	idt_set_gate(23, (u32)isr23, 0x08, 0x8E);
	idt_set_gate(24, (u32)isr24, 0x08, 0x8E);
	idt_set_gate(25, (u32)isr25, 0x08, 0x8E);
	idt_set_gate(26, (u32)isr26, 0x08, 0x8E);
	idt_set_gate(27, (u32)isr27, 0x08, 0x8E);
	idt_set_gate(28, (u32)isr28, 0x08, 0x8E);
	idt_set_gate(29, (u32)isr29, 0x08, 0x8E);
	idt_set_gate(30, (u32)isr30, 0x08, 0x8E);
	idt_set_gate(31, (u32)isr31, 0x08, 0x8E);

	idt_set_gate(32, (u32)irq0 , 0x08, 0x8E);
	idt_set_gate(33, (u32)irq1 , 0x08, 0x8E);
	idt_set_gate(34, (u32)irq2 , 0x08, 0x8E);
	idt_set_gate(35, (u32)irq3 , 0x08, 0x8E);
	idt_set_gate(36, (u32)irq4 , 0x08, 0x8E);
	idt_set_gate(37, (u32)irq5 , 0x08, 0x8E);
	idt_set_gate(38, (u32)irq6 , 0x08, 0x8E);
	idt_set_gate(39, (u32)irq7 , 0x08, 0x8E);
	idt_set_gate(40, (u32)irq8 , 0x08, 0x8E);
	idt_set_gate(41, (u32)irq9 , 0x08, 0x8E);
	idt_set_gate(42, (u32)irq10, 0x08, 0x8E);
	idt_set_gate(43, (u32)irq11, 0x08, 0x8E);
	idt_set_gate(44, (u32)irq12, 0x08, 0x8E);
	idt_set_gate(45, (u32)irq13, 0x08, 0x8E);
	idt_set_gate(46, (u32)irq14, 0x08, 0x8E);
	idt_set_gate(47, (u32)irq15, 0x08, 0x8E);

	idt_set_gate(128, (u32)isr128, 0x08, 0x8E);

	idt_flush((u32)&idt_ptr);
}

void idt_set_gate(u8 num, u32 base, u16 sel, u8 flags)
{
	idt_entries[num].base_lo = base & 0xFFFF;
	idt_entries[num].base_hi = (base >> 16) & 0xFFFF;

	idt_entries[num].sel     = sel;
	idt_entries[num].always0 = 0;
	idt_entries[num].flags   = flags  | 0x60 ;
}
