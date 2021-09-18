#include "draw.h"

void initMap(){
	//init
	int spriteCount=0;
	for (int i=0; i < MAP_Y; i++){
		for (int j=0; j < MAP_X; j++){
			map[i][j].brightness=0.0f;
			map[i][j].spriteIndex=MAX_SPRITES;
			map[i][j].occupied=false;
		}
	}
}

unsigned char addSprite(unsigned char id, int x, int y, float speed){
	if (spriteCount < MAX_SPRITES){
		char out=spriteCount;
		sprites[spriteCount].id=id;
		sprites[spriteCount].x=x;
		sprites[spriteCount].y=y;
		sprites[spriteCount].offx=0.0f;
		sprites[spriteCount].offy=0.0f;
		sprites[spriteCount].walk=false;
		sprites[spriteCount].time=0.0f;
		sprites[spriteCount].speed=speed;
		sprites[spriteCount].show=true;
		map[y][x].spriteIndex=spriteCount;
		map[y][x].occupied=true;
		spriteCount++;
		return out;
	}
	else{
		return MAX_SPRITES;
	}
}

void computeLightMap(Light *lights, int total, bool neg){
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
		lights[i].size=31;
		lights[i].brightness=1.1f;
	}
	computeLightMap(lights,1,false);
}

void drawMap(){
	//bot culling
	botCount=0;
	//culling
	float screenSize=WIN_X+WIN_Y;
	float tileSize=TILE_X+TILE_Y;
	tileX=(TILE_X/WIN_X)*scale;
	tileY=(TILE_Y/WIN_Y)*scale;
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
	if (startX < 1){
		startX=1;
	}
	if (startY < 1){
		startY=1;
	}
	#ifdef NO_CULLING
	xMax=MAP_X;
	yMax=MAP_Y;
	startX=1;
	startY=1;
	#endif
	//drawing
	float dx=(mouseX*2.0f)/WIN_X-1.0f;
	float dy=((mouseY*2.0f)/WIN_Y-1.0f)*-1.0f;
	glBegin(GL_QUADS);
	for (int x=startX; x < xMax; x++){
		for (int y=startY; y < yMax; y++){
			//tile X and Y
			Coordinates coord=transform(x, y);
			float tx=coord.x;
			float ty=coord.y;
			//pathing debugging
			int pathing=1;
			#ifdef DEBUG
			if (map[y][x].occupied){
				pathing=0;
			}
			#endif
			glColor3f(map[y][x].brightness, map[y][x].brightness * pathing, map[y][x].brightness);
			if (fabs(dx-tx)+fabs(dy-ty) < tileX && keys[LMB]){
				mouseTileX=x;
				mouseTileY=y;
			}
			glVertex2f(tx-tileX,ty);
			glVertex2f(tx,ty+tileY);
			glVertex2f(tx+tileX,ty);
			glVertex2f(tx,ty-tileY);

			//draw sprite
			if (map[y-1][x-1].spriteIndex != MAX_SPRITES){
				//-1 offset for overdraw
				int i=map[y-1][x-1].spriteIndex;
				//add bot to visible
				if (sprites[i].id == ID_BOT){
					bots[botCount]=map[y-1][x-1].spriteIndex;
					botCount++;
				}
				//draw sprite per tile
				if (x-1 == sprites[i].x && y-1 == sprites[i].y){
					glColor3f(sprites[i].id*map[sprites[i].y][sprites[i].x].brightness, map[sprites[i].y][sprites[i].x].brightness-sprites[i].id, 0);
					//recalculate X and Y for sprites with offsets
					float sx=sprites[i].x+sprites[i].offx;
					float sy=sprites[i].y-sprites[i].offy;
					#ifdef NO_SMOOTHING
					sx=sprites[i].x;
					sy=sprites[i].y;
					#endif
					//retransform X and Y to tiles
					coord=transform(sx, sy);
					tx=coord.x;
					ty=coord.y;
					//verts
					glVertex2f(tx+((TILE_X*0.3f*scale)/WIN_X), ty);
					glVertex2f(tx-((TILE_X*0.3f*scale)/WIN_X), ty);
					glVertex2f(tx-((TILE_X*0.3f*scale)/WIN_X), ty+((TILE_Y*3.0f*scale)/WIN_Y));
					glVertex2f(tx+((TILE_X*0.3f*scale)/WIN_X), ty+((TILE_Y*3.0f*scale)/WIN_Y));
				}
			}
			//draw projectile
			else{
				for(int i=0; i<projectileCount; i++){
					if (round(projectiles[i].x) == x-1 && round(projectiles[i].y) == y-1){
						float px=projectiles[i].x;
						float py=projectiles[i].y;
						#ifdef NO_SMOOTHING
						px=(int)px;
						py=(int)py;
						#endif
						//transform
						coord=transform(px, py);
						tx=coord.x;
						ty=coord.y;
						glColor3f(map[y][x].brightness, map[y][x].brightness, 0);
						glVertex2f(tx+((TILE_Y*0.5f*scale)/WIN_X), ty+((TILE_Y*scale)/WIN_Y));
						glVertex2f(tx-((TILE_Y*0.5f*scale)/WIN_X), ty+((TILE_Y*scale)/WIN_Y));
						glVertex2f(tx-((TILE_Y*0.5f*scale)/WIN_X), ty+((TILE_Y*2.0f*scale)/WIN_Y));
						glVertex2f(tx+((TILE_Y*0.5f*scale)/WIN_X), ty+((TILE_Y*2.0f*scale)/WIN_Y));
					}
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

static Coordinates transform(float x, float y){
	Coordinates out;
	out.x=(x-y)*tileX - ((camX-camY)*tileX);
	out.y=((y+x)*tileY)*-1 + ((camY+camX)*tileY);
	return out;
}
