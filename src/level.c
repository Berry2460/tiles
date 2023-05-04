#include "level.h"
#include "ai.h"

static void initMap(Texture *texture, unsigned char x, unsigned char y, bool occupied){
	//init
	spriteCount=0;
	for (int i=0; i < MAP_Y; i++){
		for (int j=0; j < MAP_X; j++){
			map[i][j].texture=texture;
			map[i][j].textureX=x;
			map[i][j].textureY=y;
			map[i][j].brightness=0.0f;
			map[i][j].spriteIndex=MAX_SPRITES;
			map[i][j].occupied=occupied;
			map[i][j].wall=occupied;
		}
	}
}

static void carveMap(unsigned char x, unsigned char y, int startx, int starty, int width, int height, bool occupied){
	for (int i=startx; i < startx+width; i++){
		for (int j=starty; j < starty+height; j++){
			map[i][j].textureX=x;
			map[i][j].textureY=y;
			map[i][j].brightness=0.0f;
			map[i][j].spriteIndex=MAX_SPRITES;
			map[i][j].occupied=occupied;
			map[i][j].wall=occupied;
		}
	}
}

void generateLevel(Texture *texture, int x, int y){
	initMap(texture, 1, y, true);
	int offx=ROOM_SIZE_MAX;
	int offy=ROOM_SIZE_MAX;
	bool done=false;
	while (!done){
		int sizex=newSeed()%(ROOM_SIZE_MAX-ROOM_SIZE_MIN)+ROOM_SIZE_MIN;
		int sizey=newSeed()%(ROOM_SIZE_MAX-ROOM_SIZE_MIN)+ROOM_SIZE_MIN;
		if (sizex+offx < MAP_X && sizey+offy < MAP_Y){
			carveMap(x, y, offx, offy, sizex, sizey, false);
			offx+=ROOM_SIZE_MAX;
		}
		else if (sizex+offx >= MAP_X){
			offy+=ROOM_SIZE_MAX;
			offx=ROOM_SIZE_MAX;
		}
		else if (sizey+offy >= MAP_Y){
			done=true;
		}
	}
}

void computeLightMap(Light *lights, int total, bool neg){
	for (int i=0; i < total; i++){
		int size=lights[i].size;
		int lx=lights[i].x-(size>>1);
		int ly=lights[i].y-(size>>1);
		for (int x=0; x < size; x++){
			if (x+lx >= MAP_X){break;}
			else if (x+lx < 0){continue;}
			for (int y=0; y < size; y++){
				if (y+ly >= MAP_Y){break;}
				else if (y+ly < 0){continue;}
				float increase=((float)(size>>1) - (abs(lights[i].x - (lx+x)) + abs(lights[i].y - (ly+y))))/size*lights[i].brightness;
				if (increase < 0.0f){continue;}
				if(neg){increase*=-1.0f;}
				map[ly+y][lx+x].brightness+=increase;
				if (map[ly+y][lx+x].brightness < 0.0f){map[ly+y][lx+x].brightness=0.0f;}
			}
		}
	}
}

void addLight(int x, int y, int size, float brightness, bool neg){ //add light
	Light light[1];
	light[0].x=x;
	light[0].y=y;
	light[0].size=size;
	light[0].brightness=brightness;
	computeLightMap(light, 1, neg);
}

void initLight(){
	Light lights[1];
	for (int i=0; i < 1; i++){
		lights[i].x=MAP_X/2;
		lights[i].y=MAP_Y/2;
		lights[i].size=15;
		lights[i].brightness=0.9f;
	}
	computeLightMap(lights,1,false);
}