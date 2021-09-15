#include "step.h"

static void nextStep(int index){
	Sprite *s=&sprites[index];	
	//reset
	map[s->y][s->x].spriteIndex=-1;
	map[s->y][s->x].occupied=false;
	s->x=s->toStepX;
	s->offx=0.0f;
	s->y=s->toStepY;
	s->offy=0.0f;
	map[s->y][s->x].spriteIndex=index;
	map[s->y][s->x].occupied=true;
	//get next step
	s->nextX=round((s->stepDestX-s->x)/(abs(s->x-s->stepDestX)+0.0001f)); //get direction
	s->nextY=round((s->stepDestY-s->y)/(abs(s->y-s->stepDestY)+0.0001f));
	//collision
	if ((s->x == s->stepDestX && s->y == s->stepDestY) || map[s->y+s->nextY][s->x+s->nextX].occupied){
		//collisions halt travel
		s->walk=false;

	}else{
		//advance RNG
		newSeed();
		//set next step
		s->toStepX=s->x+s->nextX;
		s->toStepY=s->y+s->nextY;
		map[s->toStepY][s->toStepX].occupied=true;
	}
}

void newDest(int index, int x, int y){
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
	Sprite *s=&sprites[index];
	//offset
	if (s->walk){
		if (fabs(s->offx) >= 1 || fabs(s->offy) >= 1){
			nextStep(index);
		}else{
			float offx=(speed/(float)fps)*(s->x-s->toStepX);
			float offy=(speed/(float)fps)*(s->y-s->toStepY);
			s->offx=s->offx-offx;
			s->offy=s->offy+offy;
		}
	}
}
