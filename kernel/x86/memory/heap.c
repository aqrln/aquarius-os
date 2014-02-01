#include <kernel.h>
#include "heap.h"

#include <paging.h>
#include <panic.h>

/* primary heap */

u32 phys_heap_end;

u32 ph_alloc(u32 size)
{
	u32 location = phys_heap_end;
	phys_heap_end += size;
	return location;
}

u32 ph_alloc_a(u32 size)
{
	u32 location = align_address(phys_heap_end);
	phys_heap_end = location + size;
	return location;
}

/* the real heap */

u8 *kheap;

typedef struct header {
	u32 magic;
	u32 size;
	struct header *prev, *next;
	u32 free;
} header_t;

#define MAGIC 0x12345678

void init_heap(void)
{
	kheap = (u8 *) KHEAP_ADDRESS;
	u32 page;
	for (page = KHEAP_ADDRESS; page < KHEAP_ADDRESS + KHEAP_SIZE; page += 0x1000) {
		map_page(page, 0, PTE_READWRITE | VM_ALLOCATED);
	}
	header_t *hdr = (header_t *) kheap;
	hdr->magic = MAGIC;
	hdr->size = KHEAP_SIZE - sizeof (header_t);
	hdr->next = hdr->prev = NULL;
	hdr->free = 1;
}

void *kmalloc(u32 size)
{
	header_t *smallest_hole = NULL;
	header_t *hole = NULL;

	// find the smallest suitable hole
	for (hole = (header_t *) kheap; hole != NULL; hole = hole->next) {
		assert(hole->magic == MAGIC);
		if (!hole->free || hole->size < size)
			continue;
		if (smallest_hole == NULL) {
			smallest_hole = hole;
			continue;
		}
		if (hole->size < smallest_hole->size) {
			smallest_hole = hole;
		}
	}

	if (smallest_hole == NULL)
		return NULL;

	// divide the hole if its size is bigger than we need and there's enough
	// room for a new hole
	if (smallest_hole->size - size > sizeof (header_t)) {
		header_t *new_hole = (header_t *) ((u32)smallest_hole + sizeof (header_t) + size);
		new_hole->magic = MAGIC;
		new_hole->size = smallest_hole->size - sizeof (header_t) - sizeof (header_t) - size;
		new_hole->prev = smallest_hole;
		new_hole->next = smallest_hole->next;
		new_hole->free = 1;
		smallest_hole->size = size;
		smallest_hole->next = new_hole;
		smallest_hole->free = 0;
	}

	return (void *) ((u32) smallest_hole + sizeof (header_t));
}

void kfree(void *addr)
{
	header_t *block = (header_t *) ((u32)addr - sizeof (header_t));
	assert(block->magic == MAGIC);

	block->free = 1;

	if (block->prev) {
		header_t *left = block->prev;
		assert(left->magic == MAGIC);
		if (left->free) {
			left->next = block->next;
			left->size += block->size + sizeof (header_t);
			block = left;
		}
	}

	if (block->next) {
		header_t *right = block->next;
		assert(right->magic == MAGIC);
		if (right->free) {
			block->next = right->next;
			block->size += right->size + sizeof (header_t);
		}
	}
}
