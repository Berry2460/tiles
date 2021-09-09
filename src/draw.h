#ifndef DRAW_H
#define DRAW_H

//#define DEBUG
//#define NO_CULLING
//#define NO_SMOOTHING

#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <stdbool.h>
#include "window.h"

#define MAP_X 512
#define MAP_Y 512
#define MAX_SPRITES 128 //signed char limit
#define TILE_X 64.0f
#define TILE_Y 32.0f

typedef struct Sprite{
	int x; //current coords
	int y;
	char nextX; //plus or minus 1 to step
	char nextY;
	int toStepX; //next step
	int toStepY;
	int stepDestX; //destination
	int stepDestY;
	float offx; //tile offset
	float offy;
	bool walk; //is moving
}Sprite;

typedef struct Tile{
	char spriteIndex;
	float brightness;
}Tile;

typedef struct Light{
	int x;
	int y;
	unsigned char size; //light diameter
	float brightness;
}Light;

extern float scale;
extern float camX;
extern float camY;
extern int mouseTileX;
extern int mouseTileY;
extern bool clickProcessed;
extern Tile map[MAP_Y][MAP_X];
extern Sprite sprites[MAX_SPRITES];

static int spriteCount;
static float screenSize;
static float tileSize;

void initMap();
int addSprite(int x, int y);
void computeLightMap(Light *lights, int total, bool neg);
void addLight(int x, int y, int size, bool neg);
void initLight();
void drawMap();
#endif
