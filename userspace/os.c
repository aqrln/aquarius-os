#include "os.h"

#define KB 256

static short buffer[LINES][COLS];
static float width;
static float height;

static char kbuf[KB];
static int kb_i = 0;

void keypress_handler(int scancode, char ascii)
{
	if (kb_i != KB) {
		kbuf[kb_i++] = ascii;
	}
}

void keyrelease_handler(int scancode, char ascii)
{

}

int keypressed()
{
	if (kb_i == 0) {
		int kbd = 0;
		SystemCall(2, 0, &kbd);
		if (kbd) {
			keyboard();
		}
		return 0;
	} else {
		return kbuf[--kb_i];
	}
}

char* ltoa(char* result, int value, int base) {
    // check that the base if valid
    if (base < 2 || base > 36) {
        *result = '\0';
        return result;
    }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
    } while (value);

    // Apply negative sign
    if (tmp_value < 0)
        *ptr++ = '-';
    *ptr-- = '\0';

    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }

    return result;
}

typedef union {
    long L;
    float F;
} LF_t;

char *ftoa(float f) {
    long mantissa, int_part, frac_part;
    short exp2;
    LF_t x;
    char *p;
    static char outbuf[15];

    //*status = 0;
    if (f == 0.0) {
        outbuf[0] = '0';
        outbuf[1] = '.';
        outbuf[2] = '0';
        outbuf[3] = 0;
        return outbuf;
    }
    x.F = f;

    exp2 = (unsigned char) (x.L >> 23) - 127;
    mantissa = (x.L & 0xFFFFFF) | 0x800000;
    frac_part = 0;
    int_part = 0;

    if (exp2 >= 31) {
        return 0;
    } else if (exp2 < -23) {
        return 0;
    } else if (exp2 >= 23)
        int_part = mantissa << (exp2 - 23);
    else if (exp2 >= 0) {
        int_part = mantissa >> (23 - exp2);
        frac_part = (mantissa << (exp2 + 1)) & 0xFFFFFF;
    } else /* if (exp2 < 0) */
        frac_part = (mantissa & 0xFFFFFF) >> -(exp2 + 1);

    p = outbuf;

    if (x.L < 0)
        *p++ = '-';

    if (int_part == 0)
        *p++ = '0';
    else {
        ltoa(p, int_part, 10);
        while (*p)
            p++;
    }
    *p++ = '.';

    if (frac_part == 0)
        *p++ = '0';
    else {
        char m, max;

        max = sizeof (outbuf) - (p - outbuf) - 1;
        if (max > 7)
            max = 7;
        /* print BCD */
        for (m = 0; m < max; m++) {
            /* frac_part *= 10; */
            frac_part = (frac_part << 3) + (frac_part << 1);

            *p++ = (frac_part >> 24) + '0';
            frac_part &= 0xFFFFFF;
        }
        /* delete ending zeroes */
        for (--p; p[0] == '0' && p[-1] != '.'; --p)
            ;
        ++p;
    }
    *p = 0;

    return outbuf;
}

void put(float x, float y, char c)
{
    int sx = (int)(COLS / 2 + COLS*x/width);
    int sy = (int)(LINES / 2 - LINES*y/height);
    buffer[sy][sx] = c;
}

void clear()
{
    int i, j;
    for (i = 0; i < LINES; i++) {
        for (j = 0; j < COLS; j++) {
            buffer[i][j] = ' ';
        }
    }
}

void drawtext(int x, int y, char *s)
{
    while (*s) {
        buffer[y][x++] = *s++;
    }
}

void drawnum(int x, int y, float n)
{
    drawtext(x, y, ftoa(n));
}

void memcpy(void *d, void *s, int len)
{
	char *dest = (char*)d;
	char *src = (char*)s;
	while (len--) {
		*dest++ = *src++;
	}
}

void repaint()
{
    //write(1, buffer, LINES * COLS);
	memcpy(0xb8000, buffer, LINES * COLS * 2);
}

void init_lib(float w, float h)
{
    width = w;
    height = h;
    clear();
    /*fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK);
    struct termios oflags, nflags;
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;
    tcsetattr(fileno(stdin), TCSANOW, &nflags);*/
}

void wait()
{
    //usleep(100000);
}
