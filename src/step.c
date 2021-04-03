#include "step.h"

void nextStep(Sprite *s){ //private
	s->x=toStepX;
	s->offx=0.0f;
	s->y=toStepY;
	s->offy=0.0f;
	nextX=round((stepDestX-s->x)/(abs(s->x-stepDestX)+0.00001f));
	nextY=round((stepDestY-s->y)/(abs(s->y-stepDestY)+0.00001f));
	toStepX+=nextX;
	toStepY+=nextY;
	cX=0.0f;
	cY=0.0f;
	printf("%d %d\n",nextX,nextY);
	if (steps >= maxSteps || (s->x == stepDestX && s->y == stepDestY)){
		s->walk=false;
	}
}

void newDest(Sprite *s, int x, int y){
	s->walk=true;
	stepDestX=x;
	stepDestY=y;
	toStepX=s->x;
	toStepY=s->y;
	maxSteps=abs(s->x-stepDestX)+abs(s->y-stepDestY);
	steps=0;
	nextStep(s);
}

void step(Sprite *s){
	if (s->walk){
		if (cX >= 1 || cY >= 1){
			steps++;
			nextStep(s);
		}else{
			float offx=(speed/(float)fps)*(s->x-toStepX);
			float offy=(speed/(float)fps)*(s->y-toStepY);
			s->offx=s->offx-offx;
			s->offy=s->offy+offy;
			cX+=fabs(offx);
			cY+=fabs(offy);
		}
	}
}
