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
#include "level.h"

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
int spriteCount;

//window globals
int fps;
int frames;
int mouseX;
int mouseY;
bool keys[KEYS];
bool keysPress[KEYS];

//ai globals
int bots[MAX_SPRITES];
int botCount;

//missile globals
Projectile projectiles[MAX_PROJECTILES];
unsigned char projectileCount;

//RNG
int seed;

//game loop
int main(){
	seed=350; //level generation RNG
	scale=1.0f;
	camX=MAP_X/2.0f;
	camY=MAP_Y/2.0f;
	unsigned char panim[3*8][2]={ {0, 1}, {1, 1}, {2, 1},
								  {0, 2}, {1, 2}, {2, 2},
								  {0, 3}, {1, 3}, {2, 3},
								  {0, 4}, {1, 4}, {2, 4},
								  {0, 5}, {1, 5}, {2, 5},
								  {0, 6}, {1, 6}, {2, 6},
								  {0, 7}, {1, 7}, {2, 7},
								  {0, 8}, {1, 8}, {2, 8} };

	startWindow("tiles");
	Texture *t=initTexture("t0.bmp", 96);
	generateLevel(t, 0, 0);
	initLight();
	int player=createPlayer(3, true, panim, camX, camY);
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
