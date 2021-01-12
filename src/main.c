#include "draw.h"
#include "window.h"

int map[MAP_Y][MAP_X];
float lightMap[MAP_Y][MAP_X];

void move(){
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
	if ((keys[W] || keys[S] || keys[A] || keys[D]) && (oldX != (int)camX || oldY != (int)camY)){
		addLight(lightMap, oldX, oldY, 2*WIN_Y/TILE_Y,true);
		addLight(lightMap, camX, camY, 2*WIN_Y/TILE_Y,false);
	}
	if (keys[SPACE]){
		keys[SPACE]=false;
		clearLight(lightMap);
		initLight(lightMap);
		addLight(lightMap, camX, camY, 2*WIN_Y/TILE_Y,false);
	}
	if (camY < 0){camY=0;}
	else if (camY > MAP_Y){camY=MAP_Y;}
	if (camX < 0){camX=0;}
	else if (camX > MAP_X){camX=MAP_X;}
}

int main(){
	scale=1.0f;
	camX=512.0f;
	camY=512.0f;
	startWindow("tiles");
	fillScreen(map);
	initLight(lightMap);
	addLight(lightMap, camX, camY, 2*WIN_Y/TILE_Y,false);
	//render
	while (windowLoop()){
		move();
		drawMap(map, lightMap);
	}
}
