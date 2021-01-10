#include "draw.h"

void fillScreen(int map[MAP_Y][MAP_X]){
	for (int i=0; i < MAP_Y; i++){
		for (int j=0; j < MAP_X; j++){
			map[i][j]=1;
		}
	}
}

void clearLight(float lightMap[MAP_Y][MAP_X]){
	for (int i=0; i < MAP_Y; i++){
		for (int j=0; j < MAP_X; j++){
			lightMap[i][j]=0.0f;
		}
	}
}

void computeLightMap(float lightMap[MAP_Y][MAP_X], Light *lights, int total, bool neg){
	for (int i=0; i < total; i++){
		int size=lights[i].size+2; //edge correction
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
				lightMap[ly+y][lx+x]+=increase;
				if (lightMap[ly+y][lx+x] < 0.0f){lightMap[ly+y][lx+x]=0.0f;}
			}
		}
	}
}

void addLight(float lightMap[MAP_Y][MAP_X], int x, int y, int size, bool neg){ //add light
	Light light[1];
	light[0].x=x;
	light[0].y=y;
	light[0].size=size;
	light[0].brightness=0.9f;
	computeLightMap(lightMap, light, 1, neg);
}

void initLight(float lightMap[MAP_Y][MAP_X]){
	Light lights[1];
	for (int i=0; i < 1; i++){
		lights[i].x=512;
		lights[i].y=512;
		lights[i].size=31;
		lights[i].brightness=1.5f;
	}
	computeLightMap(lightMap, lights,1,false);
}

void drawMap(int map[MAP_Y][MAP_X], float lightMap[MAP_Y][MAP_X], int mouseX, int mouseY, int fps){
	float tileX=(TILE_X/WIN_X)*scale;
	float tileY=(TILE_Y/WIN_Y)*scale;
	//culling
	int xMax=(WIN_X)/(TILE_Y*scale)+camX;
	int yMax=(WIN_Y)/(TILE_Y*scale)+camY;
	int endX=xMax-camX;
	int endY=yMax-camY;
	if (xMax > MAP_X){
		xMax=MAP_X;
	}
	if (yMax > MAP_Y){
		yMax=MAP_Y;
	}
	bool draw=true;
	int startX=camX-endX;
	int startY=camY-endY;
	if (startX < 0){
		startX=0;
	}
	if (startY < 0){
		startY=0;
	}
	//drawing
	float dx=(mouseX*2.0f)/WIN_X-1.0f;
	float dy=((mouseY*2.0f)/WIN_Y-1.0f)*-1.0f;
	glBegin(GL_QUADS);
	for (int x=startX; x < xMax; x++){
		for (int y=startY; y < yMax; y++){
			//tile X and Y
			float tx=(x-y)*tileX - ((camX-camY)*tileX);
			float ty=((y+x)*tileY)*-1 + ((camY+camX)*tileY);
			//collision and light
			glColor3f(map[y][x]*lightMap[y][x], map[y][x]*lightMap[y][x], map[y][x]*lightMap[y][x]);
			if (fabs(dx-tx)+fabs(dy-ty) < tileX && keys[LMB] && draw){
				lightMap[y][x]+=12.0f/(fps*scale);
				draw=false;
				//LMB=false; //prevents rapid firing
			}
			//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
			//glBindTexture(GL_TEXTURE_2D, texture);
			//glEnable(GL_TEXTURE_2D);
			//glTexCoord2f(0.0,0.0);
			glVertex2f(tx-tileX,ty);
			//glTexCoord2f(0.0,2.0);
			glVertex2f(tx,ty+tileY);
			//glTexCoord2f(2.0,2.0);
			glVertex2f(tx+tileX,ty);
			//glTexCoord2f(2.0,0.0);
			glVertex2f(tx,ty-tileY);
			//debug
			/*
			glColor3f(0,1,0);
			glVertex2f(tx,ty);
			glVertex2f(tx-(6.0/WIN_X),ty);
			glVertex2f(tx-(6.0/WIN_X),ty+(6.0/WIN_Y));
			glVertex2f(tx,ty+(6.0/WIN_Y));*/
		}
	}
	glEnd();
}
