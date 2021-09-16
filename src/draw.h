#ifndef DRAW_H
#define DRAW_H

//#define DEBUG
//#define NO_CULLING
//#define NO_SMOOTHING

#define MAP_X 512
#define MAP_Y 512
#define MAX_SPRITES 127 //signed char limit
#define TILE_X 64.0f
#define TILE_Y 32.0f

//sprite IDs
#define ID_PLAYER 0
#define ID_BOT 1
#define ID_PROJECTILE 2

#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <stdbool.h>
#include "window.h"
#include "ai.h"
#include "missiles.h"

typedef struct Sprite{
	unsigned char id; //sprite identifier
	unsigned char index; //other stat assignment
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
	float time; //timer
	float speed; //movement
}Sprite;

typedef struct Tile{
	char spriteIndex;
	float brightness;
	bool occupied;
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
extern Tile map[MAP_Y][MAP_X];
extern Sprite sprites[MAX_SPRITES];
extern char spriteCount;

static float screenSize;
static float tileSize;

void initMap();
char addSprite(unsigned char id, int x, int y, float speed);
void computeLightMap(Light *lights, int total, bool neg);
void addLight(int x, int y, int size, bool neg);
void initLight();
void drawMap();
#endif
