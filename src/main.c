#include <stdio.h>
#include <stdlib.h>
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
#include "network.h"

#include "GLFW/glfw3.h"

//network globals
int isHost;
int server;
int clientSocket[3];
int clientIndex[3];
int clientCount;
char *joinAddr;
int playerIndex;

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

//player globals
int newPlayerProjectile;
int newPlayerProjectileX;
int newPlayerProjectileY;

//window globals
long long fps;
int frames;
int mouseX;
int mouseY;
int screenHeight;
int screenWidth;
int fullscreen;
int vsync;
bool keys[KEYS];
bool keysPress[KEYS];

//missile globals
Projectile projectiles[MAX_PROJECTILES];
unsigned char projectileCount;

//AI globals
double botTimer;
int seed;

void checkConfig(){
	FILE *f=fopen("config.txt", "r");
	if (f == NULL){
		screenHeight=720;
		screenWidth=1280;
		vsync=1;
		fullscreen=0;
		isHost=0;
		joinAddr="127.0.0.1";
		f=fopen("config.txt", "w");
		fprintf(f, "ScreenHeight: %d\nScreenWidth: %d\nVsync: %d\nFullscreen: %d\nHostStatus: %d\nJoinAddress: %s", screenHeight, screenWidth, vsync, fullscreen, isHost, joinAddr);
		fclose(f);
	}
	else{
		char out[128];
		fscanf(f, "%s", out);
		fscanf(f, "%d", &screenHeight);
		fscanf(f, "%s", out);
		fscanf(f, "%d", &screenWidth);
		fscanf(f, "%s", out);
		fscanf(f, "%d", &vsync);
		fscanf(f, "%s", out);
		fscanf(f, "%d", &fullscreen);
		fscanf(f, "%s", out);
		fscanf(f, "%d", &isHost);
		fscanf(f, "%s", out);
		fscanf(f, "%s", joinAddr);
		fclose(f);

		printf("ScreenHeight: %d\nScreenWidth: %d\nVsync: %d\nFullscreen: %d\nHostStatus: %d\nJoinAddress: %s\n", screenHeight, screenWidth, vsync, fullscreen, isHost, joinAddr);
	}
}


//game loop
int main(){
	joinAddr=malloc(16*sizeof(char));
	checkConfig();

	//init network
	int doNetwork=initNetwork();
	if (doNetwork == 0){
		printf("Network initialized\n");
	}
	else{
		printf("Failed to initialize network!\n");
	}
	
	seed=257; //level generation RNG
	scale=1.0f;
	camX=MAP_X/2.0f +5;
	camY=MAP_Y/2.0f +5;

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
	botTimer=glfwGetTime();

	//netcode temporary workaround
	if (!isHost){
		clientIndex[0]=createDummyPlayer(3, true, panim, camX, camY);
		camX++;
	}
	else{
		clientIndex[0]=createDummyPlayer(3, true, panim, camX+1, camY);
	}
	playerIndex=createPlayer(3, true, panim, camX, camY);

	//render
	while (windowLoop()){
		glClear(GL_COLOR_BUFFER_BIT);
		drawMap();
		if (doNetwork == 0){
			updateNetwork();
		}
		movePlayer(playerIndex);
		moveBots();
		moveProjectiles();
		playerControl(playerIndex);
	}
	free(textures);
	free(t);
	return 0;
}
