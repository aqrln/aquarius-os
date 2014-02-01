#ifndef _STDDEF_H
#define _STDDEF_H

#undef  NULL
#define NULL ((void *)0)

#define asm __asm__ __volatile__

typedef unsigned long size_t;
typedef long ptrdiff_t;

#define offsetof(type, field) ((size_t) &((type *)0)->field)

typedef signed   char  s8;
typedef signed   short s16;
typedef signed   int   s32;
typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;

#endif
