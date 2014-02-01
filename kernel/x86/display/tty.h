#ifndef _TTY_H_
#define _TTY_H_

#define BLACK            0x0000
#define BLUE             0x0100
#define GREEN            0x0200
#define CYAN             0x0300
#define RED              0x0400
#define MAGENTA          0x0500
#define BROWN            0x0600
#define LIGHT_GREY       0x0700
#define DARK_GREY        0x0800
#define LIGHT_BLUE       0x0900
#define LIGHT_GREEN      0x0A00
#define LIGHT_CYAN       0x0B00
#define LIGHT_RED        0x0C00
#define LIGHT_MAGENTA    0x0D00
#define LIGHT_BROWN      0x0E00
#define WHITE            0x0F00

#define BLACK_BG         0x0000
#define BLUE_BG          0x1000
#define GREEN_BG         0x2000
#define CYAN_BG          0x3000
#define RED_BG           0x4000
#define MAGENTA_BG       0x5000
#define BROWN_BG         0x6000
#define LIGHT_GREY_BG    0x7000

#define BLINK            0x8000

#define tty_nl() tty_putc('\n')

void tty_putc(char);
void tty_puts(char *);
void tty_attr(u16);
void tty_move(u8, u8);
void tty_clear(void);
void tty_scroll(void);
void tty_init(void);
void tty_puthex(u32);
void tty_putdec(u32);

#endif
