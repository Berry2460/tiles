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
int spawnX;
int spawnY;
int isHost;
int server;
int clientSocket[64];
int clientIndices[64];
int clientIndex;
int currClientCount;
char *joinAddr;
int playerIndex;
int proceedNetwork;
int clientCount;

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
		screenHeight=480;
		screenWidth=640;
		vsync=1;
		fullscreen=0;
		isHost=1;
		joinAddr="127.0.0.1";
		clientCount=1;
		f=fopen("config.txt", "w");
		fprintf(f, "ScreenHeight: %d\nScreenWidth: %d\nVsync: %d\nFullscreen: %d\nHostStatus: %d\nJoinAddress: %s\nclientCount: %d\n", screenHeight, screenWidth, vsync, fullscreen, isHost, joinAddr, clientCount);
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
		fscanf(f, "%s", out);
		fscanf(f, "%d", &clientCount);
		fclose(f);
	}
}

//game loop
int main(){
	joinAddr=malloc(16*sizeof(char));
	checkConfig();
	
	seed=257; //level generation RNG
	scale=1.0f;
	camX=MAP_X/2.0f +5;
	camY=MAP_Y/2.0f +5;

	spawnX=camX;
	spawnY=camY;

	startWindow("Dungeon Crawler");
	Texture *t=initTexture("t0.bmp", 96);
	generateLevel(t, 0, 0);
	botTimer=glfwGetTime();

	if (isHost || clientCount == 1){
		playerIndex=createPlayer(3, true, camX, camY);
	}
	else{
		clientIndices[0]=createDummyPlayer(3, true, camX, camY);
	}

	startNetworkThread();
	//render
	while (windowLoop()){
		glClear(GL_COLOR_BUFFER_BIT);
		drawMap();
		proceedNetwork=1;
		if (clientCount-1 == currClientCount){
			movePlayer(playerIndex);
			moveBots();
			moveProjectiles();
			playerControl(playerIndex);
		}
	}
	free(textures);
	free(t);
	return 0;
}
