#include "step.h"
#include "draw.h"
#include "window.h"


int map[MAP_Y][MAP_X];
float lightMap[MAP_Y][MAP_X];

void moveCam(){
	float speed=(float)((WIN_Y+WIN_X)>>8)/(fps); //camera moving
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
}

void move(Sprite *s){
	addLight(lightMap, s->x, s->y, 2*WIN_Y/TILE_Y,true);
	if (keys[LMB] && clickProcessed){
		newDest(s, mouseTileX, mouseTileY);
		keys[LMB]=false;
	}else{
		step(s);
		//follow cam
		camX=s->x+s->offx;
		camY=s->y-s->offy;
	}
	addLight(lightMap, s->x, s->y, 2*WIN_Y/TILE_Y,false);
}

int main(){
	scale=1.0f;
	camX=512.0f;
	camY=512.0f;
	startWindow("tiles");
	fillScreen(map);
	initLight(lightMap);
	addLight(lightMap, camX, camY, 2*WIN_Y/TILE_Y,false);
	Sprite sprites[MAX_SPRITES];
	sprites[0].x=round(camX);
	sprites[0].y=round(camY);
	sprites[0].offx=0.0f;
	sprites[0].offy=0.0f;
	sprites[0].walk=false;
	//render
	while (windowLoop()){
		//glClear(GL_COLOR_BUFFER_BIT);
		moveCam();
		move(&sprites[0]);
		drawMap(map, lightMap, sprites);
	}
}
