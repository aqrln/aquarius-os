#include "os.h"
#include "game.h"

float x = -200;
float y = -200;
float vx = 5;
float vy = 15;

int state = AWAITING;

void input() {
    char c = keypressed();
    if (state == AWAITING) {
        switch (c) {
            case ' ':
                state = FLIGHT;
                break;

            case 'a':
                vx -= 0.5f;
                break;

            case 'd':
                vx += 0.5f;
                break;

            case 'w':
                vy += 0.5f;
                break;

            case 's':
                vy -= 0.5f;
                break;
                
            default:
                break;
        }
    }
    switch (c) {
        case 'r':
            x = -200;
            y = -200;
            vx = 5;
            vy = 15;
            state = AWAITING;
            break;

        default:
            break;
    }
}

void update() {
    if (state == FLIGHT) {
        if (x > 200.0f || x < -200.0f) {
            state = AWAITING;
            vx = 5.0f;
            vy = 15.0f;
            x = -200.0f;
            y = -200.0f;
        } else if (y < -200.0f) {
            y = -200.0f;
            vx = 0.0f;
            vy = 0.0f;
            state = AWAITING;
        } else {
            x += vx;
            vy -= 0.5;
            y += vy;
        }
    }
}

void draw() {
    clear();
    drawtext(0, 1, "Vx: ");
    drawtext(0, 2, "Vy: ");
    drawnum(4, 1, vx);
    drawnum(4, 2, vy);
    put(x, y, 'X');
    repaint();
}

int main() {
    init_lib(400.0f, 400.0f);
    put(0, 0, 0);
    for (;;) {
        input();
        update();
        draw();
        wait();
    }
    return 0;
}

