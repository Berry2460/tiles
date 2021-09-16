#include "draw.h"
#include "missiles.h"

//draw globals
float scale;
float camX;
float camY;
int mouseTileX;
int mouseTileY;
Tile map[MAP_Y][MAP_X];
Sprite sprites[MAX_SPRITES];
char spriteCount;

//window globals
int fps;
int mouseX;
int mouseY;
bool keys[KEYS];
bool keysPress[KEYS];

//ai globals
char bots[MAX_SPRITES];
char botCount;

//missile globals
Projectile projectiles[MAX_PROJECTILES];
unsigned char projectileCount;

//RNG
int seed=0;

void playerControl(int index){
	if (keys[A] && keys[LMB]){
		addProjectile(sprites[index].x, sprites[index].y, mouseTileX, mouseTileY, 5.0f); //test projectile
		//keys[LMB]=false;
	}
	//player movement with mouse
	else if (keys[LMB]){
		newDest(index, mouseTileX, mouseTileY);
		keys[LMB]=false;
	}
}

void movePlayer(int index){
	Sprite *s=&sprites[index];
	addLight(s->x, s->y, 2*WIN_Y/TILE_Y,true);
	step(index);
	//follow cam
	camX=s->x+s->offx;
	camY=s->y-s->offy;
	addLight(s->x, s->y, 2*WIN_Y/TILE_Y,false);
}

//game loop
int main(){
	scale=1.0f;
	camX=MAP_X/2.0f;
	camY=MAP_Y/2.0f;
	startWindow("tiles");
	initMap();
	initLight();
	addLight(camX, camY, 2*WIN_Y/TILE_Y,false); //demo light
	char player=addSprite(ID_PLAYER, round(camX), round(camY), 2.2f);
	addSprite(ID_BOT, round(camX)-3, round(camY)+1, 1.9f); //test bot 1
	addSprite(ID_BOT, round(camX)-3, round(camY)+2, 1.5f); //test bot 2
	//render
	while (windowLoop()){
		//glClear(GL_COLOR_BUFFER_BIT);
		movePlayer(player);
		moveBots();
		moveProjectiles();
		drawMap();
		playerControl(player);
	}
	return 0;
}
