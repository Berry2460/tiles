#ifndef DRAW_H
#define DRAW_H

#include <windows.h>
#include <math.h>
#include <stdbool.h>
#include "window.h"

#define MAP_X 1024
#define MAP_Y 1024
#define MAX_LIGHTS 1024
#define TILE_X 64.0f
#define TILE_Y 32.0f

float scale;
float camX;
float camY;

typedef struct{
	int x;
	int y;
	int size;
	float brightness;
}Light;
void fillScreen(int map[MAP_Y][MAP_X]);
void clearLight(float lightMap[MAP_Y][MAP_X]);
void computeLightMap(float lightMap[MAP_Y][MAP_X], Light *lights, int total, bool neg);
void addLight(float lightMap[MAP_Y][MAP_X], int x, int y, int size, bool neg);
void initLight(float lightMap[MAP_Y][MAP_X]);
void drawMap(int map[MAP_Y][MAP_X], float lightMap[MAP_Y][MAP_X], int mouseX, int mouseY, int fps);
#endif
