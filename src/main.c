#include "draw.h"

//draw globals
float scale;
float camX;
float camY;
int mouseTileX;
int mouseTileY;
bool clickProcessed;
Tile map[MAP_Y][MAP_X];
Sprite sprites[MAX_SPRITES];

//window globals
int fps;
int mouseX;
int mouseY;
bool keys[KEYS];

//ai globals
char bots[MAX_SPRITES];
char botCount;

//RNG
int seed=0;

//player movement with mouse
void movePlayer(int index){
	Sprite *s=&sprites[index];
	addLight(s->x, s->y, 2*WIN_Y/TILE_Y,true);
	if (keys[LMB] && clickProcessed){
		newDest(index, mouseTileX, mouseTileY);
		keys[LMB]=false;
	}else{
		step(index);
		//follow cam
		camX=s->x+s->offx;
		camY=s->y-s->offy;
	}
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
	addLight(camX, camY, 2*WIN_Y/TILE_Y,false);
	char player=addSprite(PLAYER_ID, round(camX), round(camY));
	addSprite(BOT_ID, round(camX)-3, round(camY)+1); //test bot 1
	addSprite(BOT_ID, round(camX)-3, round(camY)+2); //test bot 2
	//render
	while (windowLoop()){
		//glClear(GL_COLOR_BUFFER_BIT);
		drawMap();
		movePlayer(player);
		moveBots();
	}
	return 0;
}
