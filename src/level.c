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

static void carveMap(unsigned char x, unsigned char y, int startx, int starty, int width, int height){
	int endx=startx+width;
	int endy=starty+height;
	if (startx > endx){
		int temp=startx;
		startx=endx;
		endx=temp;
	}
	if (starty > endy){
		int temp=starty;
		starty=endy;
		endy=temp;
	}
	for (int i=startx; i < endx; i++){
		for (int j=starty; j < endy; j++){
			if (i < MAP_X-2 && j < MAP_Y-2 && i > 1 && j > 1){
				if (map[i][j].wall == true){
					map[i][j].textureX=x;
					map[i][j].textureY=y;
					map[i][j].brightness=0.0f;
					map[i][j].spriteIndex=MAX_SPRITES;
					map[i][j].occupied=false;
					map[i][j].wall=false;
				}
			}
		}
	}
}

void generateLevel(Texture *texture, int x, int y){
	initMap(texture, 1, y, true);
	int offx=ROOM_SIZE_MIN;
	int offy=ROOM_SIZE_MIN;
	int largesty=0;
	bool done=false;
	while (!done){
		int sizex=newSeed()%(ROOM_SIZE_MAX-ROOM_SIZE_MIN)+ROOM_SIZE_MIN;
		int sizey=newSeed()%(ROOM_SIZE_MAX-ROOM_SIZE_MIN)+ROOM_SIZE_MIN;
		if (sizey > largesty){
			largesty=sizey;
		}
		if (sizex+offx < MAP_X && sizey+offy < MAP_Y){
			carveMap(x, y, offx, offy, sizex, sizey);
			int hallways=(newSeed()&1)+2;
			while (hallways){
				int dirX=0;
				int dirY=0;
				hallways--;
				if (newSeed()&1){
					dirX=((newSeed()&1)<<1)-1;
				}
				else{
					dirY=((newSeed()&1)<<1)-1;
				}
				carveMap(x, y, offx+(sizex>>1), offy+(sizey>>1), ROOM_SIZE_MAX*dirX+2, ROOM_SIZE_MAX*dirY+2);
			}
			offx+=sizex+(newSeed()%ROOM_SIZE_MIN)+2;
		}
		else if (sizex+offx >= MAP_X){
			offy+=largesty+(newSeed()%ROOM_SIZE_MIN)+1;
			offx=ROOM_SIZE_MAX+(newSeed()%ROOM_SIZE_MIN);
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