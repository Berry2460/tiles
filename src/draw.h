#ifndef DRAW_H
#define DRAW_H

//#define DEBUG
//#define NO_TEXTURES
//#define NO_CULLING

#define MIN_SCALE 0.5f
#define MAX_SCALE 2.0f
#define SCALE_AMOUNT 0.06f

#define MAP_X 384
#define MAP_Y 384
#define MAX_SPRITES 4096
#define TILE_X 128.0f
#define TILE_Y 64.0f
#define BILINEAR 1
#define MAX_TEXTURES 255

//sprite IDs
#define ID_PLAYER 0
#define ID_BOT 1
#define ID_PROJECTILE 2
#define ID_PROP 3

#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <stdbool.h>

#include "GLFW/glfw3.h"

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

typedef struct Texture{
	unsigned char textureIndex;
	int height;
	int width;
	int size;
}Texture;

typedef struct Sprite{
	bool directional; //8 direction animation
	unsigned char frame;
	unsigned char frames;
	unsigned char *animation;
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
	Texture *texture;
	int spriteIndex;
	unsigned char textureX;
	unsigned char textureY;
	float brightness;
	bool occupied; //walls must be occupied
	bool wall; //draw in wall mode
}Tile;

typedef struct Light{
	int x;
	int y;
	unsigned char size; //light diameter
	float brightness;
}Light;

extern GLuint *textures; //texture data
extern float scale;
extern float camX;
extern float camY;
extern int mouseTileX;
extern int mouseTileY;
extern Sprite sprites[MAX_SPRITES];
extern int spriteCount;
extern Tile map[MAP_Y][MAP_X];

int addSprite(unsigned char id, bool directional, unsigned char frames, unsigned char animation[][2], int x, int y, float speed);
void removeSprite(int index);
void drawMap();
Texture *initTexture(char* name, int size);
#endif
