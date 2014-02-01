#ifndef OS_H
#define	OS_H

#define LINES 25
#define COLS  80

int keypressed();
void repaint();
void put(float x, float y, char c);
void init_lib(float width, float height);
void wait();
void clear();
void drawtext(int x, int y, char *s);
void drawnum(int x, int y, float n);

#endif	/* OS_H */

