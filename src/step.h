#ifndef STEP_H
#define STEP_H

#define DIRECTIONAL_WALKING 0

#include "draw.h"

//clockwise direction table, 0 = bottom face
static unsigned char walkTable[3][3]={{4,5,6},
									  {3,0,7},
									  {2,1,0}};

void nextStep(int index);
void newDest(int index, int x, int y);
void step(int index);
#endif
