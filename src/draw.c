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

//for culling
float screenSize=sqrt((WIN_X*WIN_X)+(WIN_Y*WIN_Y));
float tileSize=sqrt((TILE_X*TILE_X)+(TILE_Y*TILE_Y));

void drawMap(int map[MAP_Y][MAP_X], float lightMap[MAP_Y][MAP_X], Sprite sprites[MAX_SPRITES]){
	clickProcessed=false;
	float tileX=(TILE_X/WIN_X)*scale;
	float tileY=(TILE_Y/WIN_Y)*scale;
	//culling
	float offset=3.0f/scale+1; //adjustments
	int xMax=screenSize/(tileSize*scale)+camX+offset;
	int yMax=screenSize/(tileSize*scale)+camY+offset;
	int endX=xMax-camX-1/scale;
	int endY=yMax-camY-1/scale;
	if (xMax > MAP_X){
		xMax=MAP_X;
	}
	if (yMax > MAP_Y){
		yMax=MAP_Y;
	}
	int startX=camX-endX;
	int startY=camY-endY;
	if (startX < 0){
		startX=0;
	}
	if (startY < 0){
		startY=0;
	}
	#ifdef BAD
	xMax=MAP_X;
	yMax=MAP_Y;
	startX=0;
	startY=0;
	#endif
	//drawing
	float dx=(mouseX*2.0f)/WIN_X-1.0f;
	float dy=((mouseY*2.0f)/WIN_Y-1.0f)*-1.0f;
	glBegin(GL_QUADS);
	for (int x=startX; x < xMax; x++){
		for (int y=startY; y < yMax; y++){
			//tile X and Y
			float tx=(x-y)*tileX - ((camX-camY)*tileX);
			float ty=((y+x)*tileY)*-1 + ((camY+camX)*tileY);
			//mouse collision and light
			glColor3f(map[y][x]*lightMap[y][x], map[y][x]*lightMap[y][x], map[y][x]*lightMap[y][x]);
			if (fabs(dx-tx)+fabs(dy-ty) < tileX && keys[LMB]){
				mouseTileX=x;
				mouseTileY=y;
				clickProcessed=true;
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

			//sprites
			for(int i=0; i<MAX_SPRITES; i++){
				if(sprites[i].x == NULL){ //this gives warnings but its all fine :P
					break;
				}
				else if (x-1 == sprites[i].x && y-1 == sprites[i].y){
					glColor3f(0*lightMap[sprites[i].y][sprites[i].x],1.1*lightMap[sprites[i].y][sprites[i].x],0*lightMap[sprites[i].y][sprites[i].x]);
					//recalculate X and Y for sprites with offsets
					float sx=sprites[i].x+sprites[i].offx;
					float sy=sprites[i].y-sprites[i].offy;
					#ifdef NO_SMOOTHING
					sx=sprites[i].x;
					sy=sprites[i].y;
					#endif
					//retransform X and Y to tiles
					tx=(sx-sy)*tileX - ((camX-camY)*tileX);
					ty=((sy+sx)*tileY)*-1 + ((camY+camX)*tileY);
					//verts
					glVertex2f(tx+((20.0f*scale)/WIN_X), ty);
					glVertex2f(tx-((20.0f*scale)/WIN_X), ty);
					glVertex2f(tx-((20.0f*scale)/WIN_X), ty+((90.0f*scale)/WIN_Y));
					glVertex2f(tx+((20.0f*scale)/WIN_X), ty+((90.0f*scale)/WIN_Y));
				}
			}
			#ifdef DEBUG
			glColor3f(0,1,0);
			glVertex2f(tx,ty);
			glVertex2f(tx-(6.0/WIN_X),ty);
			glVertex2f(tx-(6.0/WIN_X),ty+(6.0/WIN_Y));
			glVertex2f(tx,ty+(6.0/WIN_Y));
			#endif
		}
	}
	glEnd();
}

/*GLuint initTexture(char* name){ //need image reader library :(
	FILE* f=NULL;
	f=fopen(name, "r");
	if (f == NULL){
		exit(1);
	}
	GLuint t=0; //LOAD IMAGE HERE!
	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_2D, t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	unsigned char data[] = {255,0,0,255};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	return t;
}
GLuint texture;*/
