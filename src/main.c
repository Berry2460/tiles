#include "draw.h"
#include "missiles.h"
#include "player.h"

//draw globals
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
int seed=0;

//game loop
int main(){
	scale=1.0f;
	camX=MAP_X/2.0f;
	camY=MAP_Y/2.0f;
	startWindow("tiles");
	initMap();
	initLight();
	unsigned char player=createPlayer(camX, camY);
	addSprite(ID_BOT, round(camX)-3, round(camY)+1, 1.9f); //test bot 1
	addSprite(ID_BOT, round(camX)-3, round(camY)+2, 1.5f); //test bot 2
	//render
	while (windowLoop()){
		//glClear(GL_COLOR_BUFFER_BIT);
		drawMap();
		movePlayer(player);
		moveBots();
		moveProjectiles();
		playerControl(player);
	}
	return 0;
}
