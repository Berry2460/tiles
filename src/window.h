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

#include <stdio.h>
#include "GLFW/glfw3.h"
#include "draw.h"

static char* title;
static int frames;
static float start;
static GLFWwindow* window;

extern int fps;
extern int mouseX;
extern int mouseY;
extern bool keys[KEYS];

static void mouse(GLFWwindow* window, double x, double y);
static void scroll(GLFWwindow* window, double xoffset, double yoffset);
static void mouseControl(GLFWwindow* window, int key, int action, int mods);
static void buttons(GLFWwindow* window, int key, int scancode, int action, int mods);

int startWindow(char* winTitle);
bool windowLoop();
#endif
