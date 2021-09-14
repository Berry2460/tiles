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
	char bot1=addSprite(BOT_ID, round(camX)-3, round(camY)+1); //test 1
	char bot2=addSprite(BOT_ID, round(camX)-3, round(camY)+2); //test 2
	//render
	while (windowLoop()){
		//glClear(GL_COLOR_BUFFER_BIT);
		newDest(bot1, sprites[bot2].x-2, sprites[bot2].y+3); //test 1
		newDest(bot2, sprites[bot1].x+2, sprites[bot1].y-3); //test 2
		drawMap();
		movePlayer(player);
		moveBots();
	}
	return 0;
}
