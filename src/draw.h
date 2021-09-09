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
#include "ai.h"

#define MAP_X 512
#define MAP_Y 512
#define MAX_SPRITES 127 //signed char limit
#define TILE_X 64.0f
#define TILE_Y 32.0f

//sprite IDs
#define PLAYER_ID 0
#define BOT_ID 1

typedef struct Sprite{
	unsigned char id; //sprite identifier
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
	//char typeIndex;
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
extern bool clickProcessed;
extern Tile map[MAP_Y][MAP_X];
extern Sprite sprites[MAX_SPRITES];
//AI
extern char bots[MAX_SPRITES];
extern char botCount; //draw culls bots on screen to iterate

static char spriteCount;
static float screenSize;
static float tileSize;

void initMap();
char addSprite(unsigned char id, int x, int y);
void computeLightMap(Light *lights, int total, bool neg);
void addLight(int x, int y, int size, bool neg);
void initLight();
void drawMap();
#endif
