#include <kernel.h>
#include "tty.h"

#include <io.h>
#include <vga.h>

static u8 x, y;
static u16 attribute;

static u16 *video_memory;

static void update_cursor(void)
{
	u16 pos = y * 80 + x;
	outb(0x3D4, 14);
	outb(0x3D5, pos >> 8);
	outb(0x3D4, 15);
	outb(0x3D5, (u8) pos);
}

void tty_scroll(void)
{
	u16 line, col, pos;
	for (line = 0; line < 24; line++) {
		for (col = 0; col < 80; col++) {
			pos = line * 80 + col;
			video_memory[pos] = video_memory[pos + 80];
		}
	}
	for (col = 0; col < 80; col++) {
		video_memory[24 * 80 + col] = ' ' | attribute;
	}
}

void tty_move(u8 new_x, u8 new_y)
{
	x = new_x;
	y = new_y;
	update_cursor();
}

void tty_clear(void)
{
	int i, j;
	for (i = 0; i < 25; i++) {
		for (j = 0; j < 80; j++) {
			video_memory[i * 80 + j] = ' ' | attribute;
		}
	}
	tty_move(0, 0);
}

void tty_putc(char c)
{
	switch (c) {
		case 8: //Backspace
			if (x == 0) {
				if (y == 0) break;
				y--;
				x = 79;
			} else {
				x--;
			}
			video_memory[y * 80 + x + 1] = ' ' | BLACK_BG;
			break;
		
		case '\t':
			x = (x + 8) & ~7;
			break;

		case '\r':
			x = 0;
			break;

		case '\n':
			x = 0;
			y++;
			break;

		default:
			video_memory[y * 80 + x] = c | attribute;
			x++;
			if (x >= 80) {
				x = 0;
				y++;
			}
			break;
	}
	if (y >= 25) {
		y = 24;
		tty_scroll();
	}
	update_cursor();
}

void tty_puts(char *s)
{
	while (*s) {
		tty_putc(*s++);
	}
}

void tty_attr(u16 attr)
{
	attribute = attr;
}

void tty_init(void)
{
	vga_set_text_mode();
	video_memory = (u16*) 0xB8000;
	attribute = BLACK_BG | WHITE;
	tty_clear();
}

void tty_puthex(u32 n)
{
    s32 tmp;

    tty_puts("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }
    
        if (tmp >= 0xA)
        {
            noZeroes = 0;
            tty_putc (tmp-0xA+'a' );
        }
        else
        {
            noZeroes = 0;
            tty_putc( tmp+'0' );
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        tty_putc (tmp-0xA+'a');
    }
    else
    {
        tty_putc (tmp+'0');
    }

}

void tty_putdec(u32 n)
{

    if (n == 0)
    {
        tty_putc('0');
        return;
    }

    s32 acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0)
    {
        c2[i--] = c[j++];
    }
    tty_puts(c2);

}

