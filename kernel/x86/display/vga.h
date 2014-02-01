#ifndef VGA_H
#define VGA_H

#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ			0x3C1
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		0x3C9
#define	VGA_MISC_READ		0x3CC
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF
/*			COLOR emulation		MONO emulation */
#define VGA_CRTC_INDEX		0x3D4		/* 0x3B4 */
#define VGA_CRTC_DATA		0x3D5		/* 0x3B5 */
#define	VGA_INSTAT_READ		0x3DA

#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_REGS		(1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + \
							VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

#define vpeekb(O)       *(unsigned char *)(16uL * 0xA000 + (O))
#define vpokeb(O,V)     *(unsigned char *)(16uL * 0xA000 + (O)) = (V)
#define vpokew(O,V)     *(unsigned short *)(16uL * 0xA000 + (O)) = (V)
#define vpokel(O,V)     *(unsigned long *)(16uL * 0xA000 + (O)) = (V)
#define vmemwr(DO,S,N)  memcpy((char *)(0xA000 * 16 + (DO)), S, N)

#define set_vga_plane(p) \
    outb(VGA_GC_INDEX, 4); \
    outb(VGA_GC_DATA, (p) ); \
    outb(VGA_SEQ_INDEX, 2); \
    outb(VGA_SEQ_DATA, 1 << (p) );

void vga_set_text_mode (void);
void vga_set_graphics_mode (void);
void vga_putpixel (u32 x, u32 y, u32 c);

#endif
