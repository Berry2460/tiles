#include "step.h"
#include "window.h"
#include "draw.h"
#include "ai.h"
#include "level.h"

//clockwise direction table, 0 = bottom face
static unsigned char walkTable[3][3]={{4,5,6},
									  {3,0,7},
									  {2,1,0}};

void nextStep(int index){
	if (index < 0){
		return;
	}
	Sprite *s=&sprites[index];	
	//reset
	map[s->y][s->x].spriteIndex=MAX_SPRITES;
	map[s->y][s->x].occupied=false;
	//check show
	s->x=s->toStepX;
	s->offx=0.0f;
	s->y=s->toStepY;
	s->offy=0.0f;
	map[s->y][s->x].spriteIndex=index;
	map[s->y][s->x].occupied=true;
	//get next step
	s->nextX=round((s->stepDestX-s->x)/(abs(s->x-s->stepDestX)+0.0001f)); //get direction
	s->nextY=round((s->stepDestY-s->y)/(abs(s->y-s->stepDestY)+0.0001f));
	//basic pathing correction
	if (map[s->y+s->nextY][s->x+s->nextX].wall == true){
		if (map[s->y+s->nextY][s->x].wall == false){
			s->nextX=0;
		}
		else if (!map[s->y][s->x+s->nextX].wall == false){
			s->nextY=0;
		}
	}
	//collision
	if ((s->x == s->stepDestX && s->y == s->stepDestY) || map[s->y+s->nextY][s->x+s->nextX].occupied){
		//collisions halt travel
		s->walk=false;
		switch(s->id){
			case ID_BOT:
				//check if hit player
				break;
			case ID_PLAYER:
				//check if hit bot
				break;
			case ID_PROJECTILE:
				//check if hit bot or player
				break;
		}
	}else{
		//set next step
		s->toStepX=s->x+s->nextX;
		s->toStepY=s->y+s->nextY;
		//correct stepping direction for clipping tile order
		if (s->nextX+s->nextY > 0 || (s->nextX == 1 && s->nextY == -1)){
			map[s->y][s->x].spriteIndex=MAX_SPRITES;
			map[s->toStepY][s->toStepX].spriteIndex=index;
		}
		map[s->toStepY][s->toStepX].occupied=true;
	}
}

void newDest(int index, int x, int y){
	if (index < 0){
		return;
	}
	Sprite *s=&sprites[index];
	if (s->walk){ //add to cycle
		s->stepDestX=x;
		s->stepDestY=y;
	}else{ //new cycle
		s->walk=true;
		s->stepDestX=x;
		s->stepDestY=y;
		s->toStepX=s->x;
		s->toStepY=s->y;
		nextStep(index);
	}
}

void step(int index){
	if (index < 0){
		return;
	}
	Sprite *s=&sprites[index];
	//offset
	if (s->walk){
		if (fabs(s->offx) >= 1 || fabs(s->offy) >= 1){
			s->frame=walkTable[s->nextY+1][s->nextX+1]*s->frames*s->directional;
			nextStep(index);
		}else{
			float offx=(s->speed/(float)fps)*(s->x-s->toStepX);
			float offy=(s->speed/(float)fps)*(s->y-s->toStepY);
			s->offx=s->offx-offx;
			s->offy=s->offy+offy;
			if (fabs(s->offx) > 1.0f/s->frames*((s->frame%s->frames)+1) || fabs(s->offy) > 1.0f/s->frames*((s->frame%s->frames)+1)){
				//direction
				s->frame=(s->frame+1)%s->frames+(walkTable[s->nextY+1][s->nextX+1]*s->frames*s->directional);
			}
		}
	}
}
