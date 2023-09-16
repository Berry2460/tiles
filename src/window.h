#ifndef WINDOW_H
#define WINDOW_H

#define KEYS 8

#define KEY_W 0
#define KEY_S 1
#define KEY_A 2
#define KEY_D 3
#define SPACE 4
#define SHIFT 5
#define LMB 6
#define RMB 7

#include <stdbool.h>

extern int frames;
extern long long fps;
extern int mouseX;
extern int mouseY;
extern int screenHeight;
extern int screenWidth;
extern int fullscreen;
extern int vsync;
extern bool keys[KEYS];

int startWindow(char* winTitle);
bool windowLoop();
#endif
