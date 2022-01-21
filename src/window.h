#ifndef WINDOW_H
#define WINDOW_H

#define WIN_X 1280
#define WIN_Y 720
#define V_SYNC 1

#define KEYS 8

#define W 0
#define S 1
#define A 2
#define D 3
#define SPACE 4
#define SHIFT 5
#define LMB 6
#define RMB 7

#include <stdbool.h>

extern int frames;
extern int fps;
extern int mouseX;
extern int mouseY;
extern bool keys[KEYS];

int startWindow(char* winTitle);
bool windowLoop();
#endif
