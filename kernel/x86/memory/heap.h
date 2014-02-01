#ifndef HEAP_H
#define HEAP_H

u32 ph_alloc(u32);
u32 ph_alloc_a(u32);

#define KHEAP_ADDRESS 0x40000000 // 1 gb
#define KHEAP_SIZE    0x800000  // 8 mb

void init_heap(void);
void *kmalloc(u32);
void kfree(void *);

#endif
