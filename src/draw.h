#ifndef DRAW_H
#define DRAW_H

//#define DEBUG
//#define NO_CULLING
//#define NO_SMOOTHING

#define MAP_X 512
#define MAP_Y 512
#define MAX_SPRITES 255
#define TILE_X 96.0f
#define TILE_Y 48.0f
#define BILINEAR 1
#define MAX_TEXTURES 255

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

/*
typedef struct tagBITMAPFILEHEADER
{
    WORD bfType;  //specifies the file type
    DWORD bfSize;  //specifies the size in bytes of the bitmap file
    WORD bfReserved1;  //reserved; must be 0
    WORD bfReserved2;  //reserved; must be 0
    DWORD bfOffBits;  //specifies the offset in bytes from the bitmapfileheader to the bitmap bits
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;  //specifies the number of bytes required by the struct
    LONG biWidth;  //specifies width in pixels
    LONG biHeight;  //specifies height in pixels
    WORD biPlanes;  //specifies the number of color planes, must be 1
    WORD biBitCount;  //specifies the number of bits per pixel
    DWORD biCompression;  //specifies the type of compression
    DWORD biSizeImage;  //size of image in bytes
    LONG biXPelsPerMeter;  //number of pixels per meter in x axis
    LONG biYPelsPerMeter;  //number of pixels per meter in y axis
    DWORD biClrUsed;  //number of colors used by the bitmap
    DWORD biClrImportant;  //number of colors that are important
}BITMAPINFOHEADER;
*/

typedef struct Coordinates{
	float x;
	float y;
}Coordinates;

typedef struct Sprite{
	unsigned char textureIndex;
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
	unsigned char textureIndex;
	unsigned char spriteIndex;
	float brightness;
	bool occupied;
}Tile;

typedef struct Light{
	int x;
	int y;
	unsigned char size; //light diameter
	float brightness;
}Light;

static float tileX;
static float tileY;
static unsigned char texCount;

extern GLuint *textures; //texture data
extern float scale;
extern float camX;
extern float camY;
extern int mouseTileX;
extern int mouseTileY;
extern Tile map[MAP_Y][MAP_X];
extern Sprite sprites[MAX_SPRITES];
extern unsigned char spriteCount;

static Coordinates transform(float x, float y);
static unsigned char *loadBitmap(char *filename, BITMAPINFOHEADER *bitmapInfoHeader);

void initMap(int tex);
unsigned char addSprite(unsigned char id, int tex, int x, int y, float speed);
void removeSprite(int index);
void computeLightMap(Light *lights, int total, bool neg);
void addLight(int x, int y, int size, float brightness, bool neg);
void initLight();
void drawMap();
unsigned char initTexture(char* name);
#endif
