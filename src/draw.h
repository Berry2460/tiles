#ifndef DRAW_H
#define DRAW_H

//#define DEBUG
//#define NO_CULLING
//#define NO_SMOOTHING

#include <windows.h>
#include <math.h>
#include <stdbool.h>
#include "window.h"

#define MAP_X 512
#define MAP_Y 512
#define MAX_LIGHTS 512
#define MAX_SPRITES 255 //byte limit
#define TILE_X 64.0f
#define TILE_Y 32.0f

typedef struct Sprite{
	int x;
	int y;
	int nextX;
	int nextY;
	int toStepX;
	int toStepY;
	int stepDestX;
	int stepDestY;
	float offx;
	float offy;
	bool walk;
}Sprite;

typedef struct Tile{ //integrate soon...
	unsigned char spriteIndex; //to save memory
	float brightness;
}Tile;

typedef struct Light{
	int x;
	int y;
	int size;
	float brightness;
}Light;

extern float scale;
extern float camX;
extern float camY;
extern int mouseTileX;
extern int mouseTileY;
extern bool clickProcessed;
extern Sprite sprites[MAX_SPRITES];

static float screenSize;
static float tileSize;

void fillScreen(int map[MAP_Y][MAP_X]);
void clearLight(float lightMap[MAP_Y][MAP_X]);
void computeLightMap(float lightMap[MAP_Y][MAP_X], Light *lights, int total, bool neg);
void addLight(float lightMap[MAP_Y][MAP_X], int x, int y, int size, bool neg);
void initLight(float lightMap[MAP_Y][MAP_X]);
void drawMap(int map[MAP_Y][MAP_X], float lightMap[MAP_Y][MAP_X]);
#endif
