#ifndef WINDOW_H
#define WINDOW_H

#include "GLFW/glfw3.h"
#include "stdbool.h"
#include "stdio.h"
#include "draw.h"

#define WIN_X 800
#define WIN_Y 600

#define KEYS 6

#define W 0
#define S 1
#define A 2
#define D 3
#define SPACE 4
#define LMB 5

char* title;
int mouseX;
int mouseY;
int fps;
bool keys[KEYS];
int frames;
float start;

GLFWwindow* window;
int startWindow(char* title);
bool windowLoop();
#endif
