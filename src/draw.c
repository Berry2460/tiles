#include "draw.h"

static float tileX;
static float tileY;
static unsigned char texCount;

static Coordinates transform(float x, float y);
static unsigned char *loadBitmap(char *filename, BITMAPINFOHEADER *bitmapInfoHeader);

void initMap(Texture *texture, unsigned char x, unsigned char y){
	//init
	spriteCount=0;
	for (int i=0; i < MAP_Y; i++){
		for (int j=0; j < MAP_X; j++){
			map[i][j].texture=texture;
			map[i][j].textureX=x;
			map[i][j].textureY=y;
			map[i][j].brightness=0.0f;
			map[i][j].spriteIndex=MAX_SPRITES;
			map[i][j].occupied=false;
			map[i][j].wall=false;
		}
	}
}

unsigned char addSprite(unsigned char id, Texture *texture, unsigned char *animation, unsigned char frames, int x, int y, float speed){
	if (spriteCount < MAX_SPRITES){
		char out=spriteCount;
		sprites[spriteCount].textureX=0;
		sprites[spriteCount].textureY=0;
		sprites[spriteCount].texture=texture;
		sprites[spriteCount].animation=animation;
		sprites[spriteCount].frame=0;
		sprites[spriteCount].frames=frames;
		sprites[spriteCount].id=id;
		sprites[spriteCount].x=x;
		sprites[spriteCount].y=y;
		sprites[spriteCount].offx=0.0f;
		sprites[spriteCount].offy=0.0f;
		sprites[spriteCount].walk=false;
		sprites[spriteCount].time=0.0f;
		sprites[spriteCount].speed=speed;
		map[y][x].spriteIndex=spriteCount;
		map[y][x].occupied=true;
		spriteCount++;
		return out;
	}
	else{
		return MAX_SPRITES;
	}
}

void removeSprite(int index){
	spriteCount--;
	map[sprites[index].y][sprites[index].x].spriteIndex=MAX_SPRITES;
	map[sprites[index].y][sprites[index].x].occupied=false;
	if (sprites[index].walk){
		map[sprites[index].toStepY][sprites[index].toStepX].occupied=false;
		map[sprites[index].toStepY][sprites[index].toStepX].spriteIndex=MAX_SPRITES;
	}
	sprites[index]=sprites[index+1];
	for (int i=index+1; i<spriteCount; i++){
		if (sprites[i].walk && (sprites[i].nextX+sprites[i].nextY > 0 || (sprites[i].nextX == 1 && sprites[i].nextY == -1))){
			map[sprites[i].toStepY][sprites[i].toStepX].spriteIndex--;
		}else{
			map[sprites[i].y][sprites[i].x].spriteIndex--;
		}
		sprites[i]=sprites[i+1];
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
	if (xMax >= MAP_X){
		xMax=MAP_X-1;
	}
	if (yMax >= MAP_Y){
		yMax=MAP_Y-1;
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
	//tile X and Y transform coords
	float tx;
	float ty;
	Coordinates coord;
	//start drawing
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	//verts
	glBegin(GL_QUADS);
	for (int x=startX; x < xMax; x++){
		for (int y=startY; y < yMax; y++){
			//pathing debugging
			int pathing=1;
			#ifdef DEBUG
			if (map[y][x].occupied){
				pathing=0;
			}
			#endif
			//draw floor
			coord=transform(x, y);
			tx=coord.x;
			ty=coord.y;
			//mouse
			if (fabs(dx-tx)+fabs(dy-ty) < tileX){
				mouseTileX=x;
				mouseTileY=y;
			}
			//texture mapping
			float minTextureX=(float)(map[y][x].textureX)*map[y][x].texture->size / map[y][x].texture->width;
			float maxTextureX=(float)(map[y][x].textureX+1)*map[y][x].texture->size / map[y][x].texture->width;
			float minTextureY=1-(float)(map[y][x].textureY)*map[y][x].texture->size / map[y][x].texture->height;
			float maxTextureY=1-(float)(map[y][x].textureY+1)*map[y][x].texture->size / map[y][x].texture->height;
			//floor
			glColor3f(map[y+1][x].brightness, map[y+1][x].brightness * pathing, map[y+1][x].brightness); //left
			glTexCoord2f(minTextureX,minTextureY);
			glVertex2f(tx-tileX,ty);
			glColor3f(map[y][x].brightness, map[y][x].brightness * pathing, map[y][x].brightness); //top
			glTexCoord2f(minTextureX,maxTextureY);
			glVertex2f(tx,ty+tileY);
			glColor3f(map[y][x+1].brightness, map[y][x+1].brightness * pathing, map[y][x+1].brightness); //right
			glTexCoord2f(maxTextureX,maxTextureY);
			glVertex2f(tx+tileX,ty);
			glColor3f(map[y+1][x+1].brightness, map[y+1][x+1].brightness * pathing, map[y+1][x+1].brightness); //bot
			glTexCoord2f(maxTextureX,minTextureY);
			glVertex2f(tx,ty-tileY);
			//draw wall
			if (map[y][x].wall){
				float transparent=1.0f;
				if (((y-round(camY)) <= 1 && (x-round(camX)) <= 1) && ((y-round(camY)) >= 0 && (x-round(camX)) >= 0)){
					transparent=0.75f;
				}
				//top
				glColor4f(map[y+1][x].brightness*0.6f, map[y+1][x].brightness*0.6f, map[y+1][x].brightness*0.6f, transparent);
				glTexCoord2f(minTextureY,minTextureX);
				glVertex2f(tx-tileX,ty+(tileY*3));
				glColor4f(map[y][x].brightness*0.6f, map[y][x].brightness*0.6f, map[y][x].brightness*0.6f, transparent);
				glTexCoord2f(minTextureY,maxTextureX);
				glVertex2f(tx,ty+tileY+(tileY*3));
				glColor4f(map[y][x+1].brightness*0.6f, map[y][x+1].brightness*0.6f, map[y][x+1].brightness*0.6f, transparent);
				glTexCoord2f(maxTextureY,maxTextureX);
				glVertex2f(tx+tileX,ty+(tileY*3));
				glColor4f(map[y+1][x+1].brightness*0.6f, map[y+1][x+1].brightness*0.6f, map[y+1][x+1].brightness*0.6f, transparent);
				glTexCoord2f(maxTextureY,minTextureX);
				glVertex2f(tx,ty-tileY+(tileY*3));
				//left face
				if (!map[y+1][x].wall || transparent > 0.99f){
					glColor4f(map[y+1][x].brightness, map[y+1][x].brightness, map[y+1][x].brightness, transparent);
					glTexCoord2f(minTextureY,minTextureX);
					glVertex2f(tx-tileX,ty);
					glColor4f(map[y][x-1].brightness, map[y][x-1].brightness, map[y][x-1].brightness, transparent);
					glTexCoord2f(minTextureY,maxTextureX);
					glVertex2f(tx-tileX,ty+(tileY*3));
					glColor4f(map[y][x+1].brightness, map[y][x+1].brightness, map[y][x+1].brightness, transparent);
					glTexCoord2f(maxTextureY,maxTextureX);
					glVertex2f(tx,ty-tileY+(tileY*3));
					glColor4f(map[y+1][x+1].brightness, map[y+1][x+1].brightness, map[y+1][x+1].brightness, transparent);
					glTexCoord2f(maxTextureY,minTextureX);
					glVertex2f(tx,ty-tileY);
				}
				//right face
				if (!map[y][x+1].wall || transparent > 0.99f){
					glColor4f(map[y+1][x+1].brightness*0.8f, map[y+1][x+1].brightness*0.8f, map[y+1][x+1].brightness*0.8f, transparent);
					glTexCoord2f(minTextureY,minTextureX);
					glVertex2f(tx,ty-tileY);
					glColor4f(map[y+1][x].brightness*0.8f, map[y+1][x].brightness*0.8f, map[y+1][x].brightness*0.8f, transparent);
					glTexCoord2f(minTextureY,maxTextureX);
					glVertex2f(tx,ty-tileY+(tileY*3));
					glColor4f(map[y-1][x].brightness*0.8f, map[y-1][x].brightness*0.8f, map[y-1][x].brightness*0.8f, transparent);
					glTexCoord2f(maxTextureY,maxTextureX);
					glVertex2f(tx+tileX,ty+(tileY*3));
					glColor4f(map[y][x+1].brightness*0.8f, map[y][x+1].brightness*0.8f, map[y][x+1].brightness*0.8f, transparent);
					glTexCoord2f(maxTextureY,minTextureX);
					glVertex2f(tx+tileX,ty);
				}
			}
			
			//draw sprite
			if (map[y][x].spriteIndex != MAX_SPRITES){
				int i=map[y][x].spriteIndex;
				//add bot to visible
				if (sprites[i].id == ID_BOT){
					bots[botCount]=map[y][x].spriteIndex;
					botCount++;
				}
				//draw sprite per tile
				glColor3f(map[sprites[i].y][sprites[i].x].brightness, map[sprites[i].y][sprites[i].x].brightness, map[sprites[i].y][sprites[i].x].brightness);
				//recalculate X and Y for sprites with offsets
				float sx=sprites[i].x+sprites[i].offx;
				float sy=sprites[i].y-sprites[i].offy;
				//transform X and Y to tiles
				coord=transform(sx, sy);
				tx=coord.x;
				ty=coord.y;
				//texture mapping
				minTextureX=(float)sprites[map[y][x].spriteIndex].textureX*map[y][x].texture->size / map[y][x].texture->width;
				maxTextureX=(float)(sprites[map[y][x].spriteIndex].textureX+1)*map[y][x].texture->size / map[y][x].texture->width;
				minTextureY=(float)sprites[map[y][x].spriteIndex].textureY*map[y][x].texture->size / map[y][x].texture->height;
				maxTextureY=(float)(sprites[map[y][x].spriteIndex].textureY+1)*map[y][x].texture->size / map[y][x].texture->height;
				//verts
				glTexCoord2f(0.0f,0.0f);
				glVertex2f(tx-((TILE_X*0.5f*scale)/WIN_X), ty);
				glTexCoord2f(0.0f,1.0f);
				glVertex2f(tx-((TILE_X*0.5f*scale)/WIN_X), ty+((TILE_Y*3.0f*scale)/WIN_Y));
				glTexCoord2f(1.0f,1.0f);
				glVertex2f(tx+((TILE_X*0.5f*scale)/WIN_X), ty+((TILE_Y*3.0f*scale)/WIN_Y));
				glTexCoord2f(1.0f,0.0f);
				glVertex2f(tx+((TILE_X*0.5f*scale)/WIN_X), ty);
			}
			//draw projectiles
			if (map[y][x].spriteIndex == MAX_SPRITES){
				for(int i=0; i<projectileCount; i++){
					if (round(projectiles[i].x) == x && round(projectiles[i].y) == y){
						float px=projectiles[i].x;
						float py=projectiles[i].y;
						//transform
						coord=transform(px, py);
						tx=coord.x;
						ty=coord.y;
						glColor3f(map[y][x].brightness, map[y][x].brightness, map[y][x].brightness);
						// TEXTURE STUFFS
						glBindTexture(GL_TEXTURE_2D, textures[projectiles[i].textureIndex]);
						//verts
						//glBegin(GL_QUADS);
						glTexCoord2f(0.0f,0.0f);
						glVertex2f(tx-((TILE_Y*0.5f*scale)/WIN_X), ty+((TILE_Y*scale)/WIN_Y));
						glTexCoord2f(0.0f,1.0f);
						glVertex2f(tx-((TILE_Y*0.5f*scale)/WIN_X), ty+((TILE_Y*2.0f*scale)/WIN_Y));
						glTexCoord2f(1.0f,1.0f);
						glVertex2f(tx+((TILE_Y*0.5f*scale)/WIN_X), ty+((TILE_Y*2.0f*scale)/WIN_Y));
						glTexCoord2f(1.0f,0.0f);
						glVertex2f(tx+((TILE_Y*0.5f*scale)/WIN_X), ty+((TILE_Y*scale)/WIN_Y));
						//glEnd();
					}
				}
			}
			#ifdef DEBUG
			glEnd();
			glBindTexture(GL_TEXTURE_2D, 0);
			glBegin(GL_QUADS);
			glColor3f(0,1,0);
			glVertex2f(tx,ty);
			glVertex2f(tx-(6.0/WIN_X),ty);
			glVertex2f(tx-(6.0/WIN_X),ty+(6.0/WIN_Y));
			glVertex2f(tx,ty+(6.0/WIN_Y));
			glEnd();
			glBegin(GL_QUADS);
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

static unsigned char *loadBitmap(char *filename, BITMAPINFOHEADER *bitmapInfoHeader){
	FILE *filePtr;  //our file pointer
    BITMAPFILEHEADER bitmapFileHeader;  //our bitmap file header
    unsigned char *bitmapImage;  //store image data
    int imageIdx=0;  //image index counter
    unsigned char tempRGB;  //our swap variable
    //open file in read binary mode
    filePtr = fopen(filename,"rb");
    if (filePtr == NULL){
        return NULL;
    }
    //read the bitmap file header
    fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER),1,filePtr);
    //verify that this is a .BMP file by checking bitmap id
    if (bitmapFileHeader.bfType !=0x4D42){
        fclose(filePtr);
        return NULL;
    }
    //read the bitmap info header
    fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER),1,filePtr); 
    //move file pointer to the beginning of bitmap data
    fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
    //allocate enough memory for the bitmap image data
    bitmapImage=malloc(bitmapInfoHeader->biSizeImage);
    //verify memory allocation
    if (!bitmapImage){
        free(bitmapImage);
        fclose(filePtr);
        return NULL;
    }
    //read in the bitmap image data
    fread(bitmapImage,bitmapInfoHeader->biSizeImage,1,filePtr);
    //make sure bitmap image data was read
    if (bitmapImage == NULL){
        fclose(filePtr);
        return NULL;
    }
    int off=0;
    unsigned char *finalBitmapImage=malloc(bitmapInfoHeader->biWidth*bitmapInfoHeader->biHeight*4);
    //swap the R and B values to get RGB (bitmap is BGR)
    for (imageIdx=0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3){
        //transparency 0xFF00FF
        if (bitmapImage[imageIdx] == 255 && bitmapImage[imageIdx+1] == 0 && bitmapImage[imageIdx+2] == 255){
        	finalBitmapImage[imageIdx+off]=0;
        	finalBitmapImage[imageIdx+1+off]=0;
	        finalBitmapImage[imageIdx+2+off]=0;
	        finalBitmapImage[imageIdx+3+off]=0;
        }
        else{
        	tempRGB=bitmapImage[imageIdx];
        	finalBitmapImage[imageIdx+off]=bitmapImage[imageIdx+2];
        	finalBitmapImage[imageIdx+1+off]=bitmapImage[imageIdx+1];
        	finalBitmapImage[imageIdx+2+off]=tempRGB;
        	finalBitmapImage[imageIdx+3+off]=255;
    	}
    	//printf("%d %d %d %d\n",finalBitmapImage[imageIdx+off],finalBitmapImage[imageIdx+1+off],finalBitmapImage[imageIdx+2+off],finalBitmapImage[imageIdx+3+off]);
    	off++;
    }
    //close file and return bitmap image data
    fclose(filePtr);
    free(bitmapImage);
    return finalBitmapImage;
}

Texture *initTexture(char *name, int size){
	Texture *out=malloc(sizeof(Texture));
	BITMAPINFOHEADER infoHeader;
	unsigned char *pixelData;
	pixelData=loadBitmap(name, &infoHeader);
	if (pixelData != NULL || texCount >= MAX_TEXTURES){
		int imgX=infoHeader.biWidth;
		int imgY=infoHeader.biHeight;
		glBindTexture(GL_TEXTURE_2D, textures[texCount]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if(BILINEAR == 1){
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		else{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgX, imgY, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
		out->textureIndex=texCount;
		out->height=imgY;
		out->width=imgX;
		out->size=size;
		texCount++;
		free(pixelData);
		return out;
	}else{
		printf("FAILED LOADING %s\n",name);
		free(pixelData);
		free(out);
		return NULL;
	}
}
