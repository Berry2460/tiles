#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <stdbool.h>

#include "draw.h"
#include "step.h"
#include "ai.h"
#include "player.h"
#include "missiles.h"
#include "window.h"

#include "GLFW/glfw3.h"

//draw globals
GLuint *textures;
float scale;
float camX;
float camY;
int mouseTileX;
int mouseTileY;
Tile map[MAP_Y][MAP_X];
Sprite sprites[MAX_SPRITES];
unsigned char spriteCount;

//window globals
int fps;
int frames;
int mouseX;
int mouseY;
bool keys[KEYS];
bool keysPress[KEYS];

//ai globals
unsigned char bots[MAX_SPRITES];
unsigned char botCount;

//missile globals
Projectile projectiles[MAX_PROJECTILES];
unsigned char projectileCount;

//RNG
int seed;

//game loop
int main(){
	scale=1.0f;
	camX=MAP_X/2.0f;
	camY=MAP_Y/2.0f;
	unsigned char panim[2][2]={ {0, 1}, {0, 2} };
	unsigned char banim[2][2]={ {1, 1}, {1, 2} };
	startWindow("tiles");
	Texture *t=initTexture("t0.bmp", 96);
	/*initTexture("t1.bmp");
	initTexture("t2.bmp");
	initTexture("t3.bmp");
	initTexture("t4.bmp");
	initTexture("t5.bmp");
	initTexture("t6.bmp");
	initTexture("t7.bmp");
	initTexture("t8.bmp");
	initTexture("t9.bmp");*/
	initMap(t, 0, 0);
	initLight();
	unsigned char player=createPlayer(2, panim, camX, camY);
	addSprite(ID_BOT, 2, banim, round(camX)-3, round(camY)+1, 1.9f); //test bot 1
	addSprite(ID_BOT, 2, banim, round(camX)-3, round(camY)+2, 1.5f); //test bot 2
	addSprite(ID_BOT, 2, banim, round(camX)+3, round(camY)+3, 1.8f); //test bot 3
	addSprite(ID_BOT, 2, banim, round(camX)+5, round(camY)-2, 2.0f); //test bot 4
	addSprite(ID_BOT, 2, banim, round(camX)-4, round(camY)-3, 1.6f); //test bot 5
	//render
	while (windowLoop()){
		glClear(GL_COLOR_BUFFER_BIT);
		drawMap();
		movePlayer(player);
		moveBots();
		moveProjectiles();
		playerControl(player);
	}
	free(textures);
	free(t);
	return 0;
}
