#include <kernel.h>
#include "vga.h"
#include <io.h>

unsigned char g_80x25_text[] =
{
/* MISC */
    0x67,
/* SEQ */
    0x03, 0x00, 0x03, 0x00, 0x02,
/* CRTC */
    0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F,
    0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x50,
    0x9C, 0x0E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3,
    0xFF,
/* GC */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
    0xFF,
/* AC */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
    0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    0x0C, 0x00, 0x0F, 0x08, 0x00
};

unsigned char g_640x480x16[] =
{
/* MISC */
    0xE3,
/* SEQ */
    0x03, 0x01, 0x08, 0x00, 0x06,
/* CRTC */
    0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0x0B, 0x3E,
    0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xEA, 0x0C, 0xDF, 0x28, 0x00, 0xE7, 0x04, 0xE3,
    0xFF,
/* GC */
    0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x05, 0x0F,
    0xFF,
/* AC */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
    0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
    0x01, 0x00, 0x0F, 0x00, 0x00
};

static void vga_write_regs(unsigned char *regs)
{
    unsigned i;

/* write MISCELLANEOUS reg */
    outb(VGA_MISC_WRITE, *regs);
    regs++;
/* write SEQUENCER regs */
    for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
    {
        outb(VGA_SEQ_INDEX, i);
        outb(VGA_SEQ_DATA, *regs);
        regs++;
    }
/* unlock CRTC registers */
    outb(VGA_CRTC_INDEX, 0x03);
    outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
    outb(VGA_CRTC_INDEX, 0x11);
    outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);
/* make sure they remain unlocked */
    regs[0x03] |= 0x80;
    regs[0x11] &= ~0x80;
/* write CRTC regs */
    for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
    {
        outb(VGA_CRTC_INDEX, i);
        outb(VGA_CRTC_DATA, *regs);
        regs++;
    }
/* write GRAPHICS CONTROLLER regs */
    for(i = 0; i < VGA_NUM_GC_REGS; i++)
    {
        outb(VGA_GC_INDEX, i);
        outb(VGA_GC_DATA, *regs);
        regs++;
    }
/* write ATTRIBUTE CONTROLLER regs */
    for(i = 0; i < VGA_NUM_AC_REGS; i++)
    {
        (void)inb(VGA_INSTAT_READ);
        outb(VGA_AC_INDEX, i);
        outb(VGA_AC_WRITE, *regs);
        regs++;
    }
/* lock 16-color palette and unblank display */
    (void)inb(VGA_INSTAT_READ);
    outb(VGA_AC_INDEX, 0x20);
}

/*inline*/ void __vga_putpixel (u32 x, u32 y, u32 c)
{
    u32 off = (640 * y + x) / 8;
    u8 mask = 0x80 >> (x & 7);
    u8 p, pmask = 1;
    for(p = 0; p < 4; p++)
    {
        set_vga_plane(p);
        if(pmask & c)
            vpokeb(off, vpeekb(off) | mask);
        else
            vpokeb(off, vpeekb(off) & ~mask);
        pmask <<= 1;
    }
}

/*inline*/ void __vga_putpixel_specific_plane_32b (u8 pmask, u32 x, u32 y, u32 c)
{
    u32 off = (640 * y + x) / 8;
    if(pmask & c)
        vpokel(off, 0xFFFFFFFF);
    else
        vpokel(off, 0);
}

/*inline*/ void __vga_putpixel_32b (u32 x, u32 y, u32 c)
{
    u32 off = (640 * y + x) / 8;
    u8 p, pmask = 1;
    for(p = 0; p < 4; p++)
    {
        set_vga_plane(p);
        if(pmask & c)
            vpokel (off, 0xFFFFFFFF);
        else
            vpokel (off, 0);
        pmask <<= 1;
    }
}

void vga_putpixel (u32 x, u32 y, u32 c)
{
    __vga_putpixel (x, y, c);
}

void vga_fill_screen (u32 c)
{
    u32 x, y;
    u8 p, pmask = 1;
    for (p = 0; p < 4; p++) {
        set_vga_plane (p);
        for (y = 0; y < 480; y++) {
            for (x = 0; x < 640; x += 16) {
                __vga_putpixel_specific_plane_32b (pmask, x, y, c);
            }
        }
        pmask <<= 1;
    }
}

void vga_set_text_mode (void)
{
#define pokeb(S,O,V) *(unsigned char *)(16uL * (S) + (O)) = (V)
#define pokew(S,O,V) *(unsigned short *)(16uL * (S) + (O)) = (V)
    vga_write_regs (g_80x25_text);
    pokew(0x40, 0x4A, 80);        /* columns on screen */
    pokew(0x40, 0x4C, 80 * 25 * 2); /* framebuffer size */
    pokew(0x40, 0x50, 0);                /* cursor pos'n */
    pokeb(0x40, 0x60, 16 - 1);        /* cursor shape */
    pokeb(0x40, 0x61, 16 - 2);
    pokeb(0x40, 0x84, 25 - 1);        /* rows on screen - 1 */
    pokeb(0x40, 0x85, 16);                /* char heig16 */
/* set white-on-black attributes for all text */
    u32 i;
    for(i = 0; i < 80 * 25; i++)
        pokeb(0xB800, i * 2 + 1, 7);
#undef pokeb
#undef pokew
}

void vga_set_graphics_mode (void)
{
    vga_write_regs (g_640x480x16);
}
