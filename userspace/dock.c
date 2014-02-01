#include "aq.h"

unsigned short *vmem;

char* itoa(/*char* result,*/ int value/*, int base*/) {
	static char result[100];
	int base = 10;
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

short vbuf[80*25];

void put(unsigned char c, unsigned short attr, int x, int y)
{
	//vmem[y * 80 + x] = c | attr;
	vbuf[y * 80 + x] = c | attr;
}

void str(char *s, unsigned short attr, int x, int y)
{
	while (*s) {
		put(*s++, attr, x++, y);
	}
}

int timer = 0;

void update(void)
{
	int x, y;
	for (x = 50; x < 80; x++) {
		for (y = 0; y < 8; y++) {
			put(' ', MAGENTA_BG, x, y);
		}
	}
	str("Aquarius", WHITE | MAGENTA_BG, 60, 1);
	str("Uptime (sec):", WHITE | MAGENTA_BG, 54, 4);
	SystemCall(7, 0, &timer);
	char *t = ftoa(timer / 100.0f);
	str(t, WHITE | MAGENTA_BG, 68, 4);
}

void draw(void)
{
	if (timer % 5 == 0) {
		int x, y;
		for (x = 50; x < 80; x++) {
			for (y = 0; y < 8; y++) {
				vmem[x + 80*y] = vbuf[x + 80*y];
			}
		}
	}
}

void wait(void)
{
	int x = 100000;
	while (x--) ;
}

int main()
{
	vmem = (unsigned short *) 0xb8000;
	for (;;) {
		update();
		draw();
		//wait();
	}
	return 0;
}
