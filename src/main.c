#include "draw.h"
#include "window.h"
#include "step.h"

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

/*
void moveCam(){
	float speed=(float)((WIN_Y+WIN_X)>>8)/(fps); //camera moving for keyboard
	int oldX=camX;
	int oldY=camY;
	if (keys[W]){
		camY-=speed;
		camX-=speed;
	}
	if (keys[S]){
		camY+=speed;
		camX+=speed;
	}
	if (keys[A]){
		camX-=speed;
		camY+=speed;
		
	}
	if (keys[D]){
		camX+=speed;
		camY-=speed;
	}
	if (keys[SPACE]){
		keys[SPACE]=false;
		clearLight(lightMap);
	}
	if (camY < 0){camY=0;}
	else if (camY > MAP_Y){camY=MAP_Y;}
	if (camX < 0){camX=0;}
	else if (camX > MAP_X){camX=MAP_X;}
}*/

void move(int index){
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

int main(){
	scale=1.0f;
	camX=MAP_X/2.0f;
	camY=MAP_Y/2.0f;
	startWindow("tiles");
	initMap();
	initLight();
	addLight(camX, camY, 2*WIN_Y/TILE_Y,false);
	char player=addSprite(round(camX), round(camY));
	addSprite(round(camX)-3, round(camY)+1); //test
	//render
	while (windowLoop()){
		//glClear(GL_COLOR_BUFFER_BIT);
		//moveCam();
		drawMap();
		move(player);
	}
}
