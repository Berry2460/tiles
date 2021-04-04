#ifndef WINDOW_H
#define WINDOW_H

#include <stdio.h>
#include "GLFW/glfw3.h"
#include "draw.h"

#define WIN_X 800
#define WIN_Y 600
#define V_SYNC 0

#define KEYS 6

#define W 0
#define S 1
#define A 2
#define D 3
#define SPACE 4
#define LMB 5

static char* title;
static int frames;
static float start;

int fps;
int mouseX;
int mouseY;
bool keys[KEYS];

GLFWwindow* window;
int startWindow(char* winTitle);
bool windowLoop();
#endif
