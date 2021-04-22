#include "step.h"

static void nextStep(Sprite *s){
	//reset
	s->x=s->toStepX;
	s->offx=0.0f;
	s->y=s->toStepY;
	s->offy=0.0f;
	s->nextX=round((s->stepDestX-s->x)/(abs(s->x-s->stepDestX)+0.0001f)); //get direction
	s->nextY=round((s->stepDestY-s->y)/(abs(s->y-s->stepDestY)+0.0001f));
	s->toStepX=s->toStepX+s->nextX;
	s->toStepY=s->toStepY+s->nextY;
	if (s->x == s->stepDestX && s->y == s->stepDestY){
		s->walk=false;
	}
}

void newDest(Sprite *s, int x, int y){
	if (s->walk){ //add to cycle
		s->stepDestX=x;
		s->stepDestY=y;
	}else{ //new cycle
		s->walk=true;
		s->stepDestX=x;
		s->stepDestY=y;
		s->toStepX=s->x;
		s->toStepY=s->y;
		nextStep(s);
	}
}

void step(Sprite *s){
	//offset
	if (s->walk){
		if (fabs(s->offx) >= 1 || fabs(s->offy) >= 1){
			nextStep(s);
		}else{
			float offx=(speed/(float)fps)*(s->x-s->toStepX);
			float offy=(speed/(float)fps)*(s->y-s->toStepY);
			s->offx=s->offx-offx;
			s->offy=s->offy+offy;
		}
	}
}
