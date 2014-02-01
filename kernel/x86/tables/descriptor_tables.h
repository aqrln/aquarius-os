#ifndef _DESCRIPTOR_TABLES_H_
#define _DESCRIPTOR_TABLES_H_

struct gdt_entry_struct
{
	u16 limit_low;           // The lower 16 bits of the limit.
	u16 base_low;            // The lower 16 bits of the base.
	u8  base_middle;         // The next 8 bits of the base.
	u8  access;              // Access flags, determine what ring this segment can be used in.
	u8  granularity;
	u8  base_high;           // The last 8 bits of the base.
} __attribute__((packed));
typedef struct gdt_entry_struct gdt_entry_t;

struct gdt_ptr_struct
{
	u16 limit;               // The upper 16 bits of all selector limits.
	u32 base;                // The address of the first gdt_entry_t struct.
}
 __attribute__((packed));
typedef struct gdt_ptr_struct gdt_ptr_t;

struct idt_entry_struct
{
	u16 base_lo;             // The lower 16 bits of the address to jump to when this interrupt fires.
	u16 sel;                 // Kernel segment selector.
	u8  always0;             // This must always be zero.
	u8  flags;               // More flags. See documentation.
	u16 base_hi;             // The upper 16 bits of the address to jump to.
} __attribute__((packed));
typedef struct idt_entry_struct idt_entry_t;

struct idt_ptr_struct
{
	u16 limit;
	u32 base;                // The address of the first element in our idt_entry_t array.
} __attribute__((packed));
typedef struct idt_ptr_struct idt_ptr_t;

// A struct describing a Task State Segment.
struct tss_entry_struct
{
	u32 backlink; // The previous TSS - if we used hardware task switching this would form a linked list.
	u32 esp0; // The stack pointer to load when we change to kernel mode.
	u32 ss0; // The stack segment to load when we change to kernel mode.
	u32 esp1; // Unused...
	u32 ss1;
	u32 esp2;
	u32 ss2;
	u32 cr3;
	u32 eip;
	u32 eflags;
	u32 eax;
	u32 ecx;
	u32 edx;
	u32 ebx;
	u32 esp;
	u32 ebp;
	u32 esi;
	u32 edi;
	u32 es; // The value to load into ES when we change to kernel mode.
	u32 cs; // The value to load into CS when we change to kernel mode.
	u32 ss; // The value to load into SS when we change to kernel mode.
	u32 ds; // The value to load into DS when we change to kernel mode.
	u32 fs; // The value to load into FS when we change to kernel mode.
	u32 gs; // The value to load into GS when we change to kernel mode.
	u32 ldt; // Unused...
	u16 trap;
	u16 iomap_base;
} __attribute__((packed));
typedef struct tss_entry_struct tss_entry_t; 

void init_descriptor_tables(void);
void idt_set_gate(u8, u32, u16, u8);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0 ();
extern void irq1 ();
extern void irq2 ();
extern void irq3 ();
extern void irq4 ();
extern void irq5 ();
extern void irq6 ();
extern void irq7 ();
extern void irq8 ();
extern void irq9 ();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

extern void isr128();

#endif
