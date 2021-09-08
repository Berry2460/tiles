#ifndef STEP_H
#define STEP_H

#include "draw.h"

static float speed=2.2f;

static void nextStep(int index);

void newDest(int index, int x, int y);
void step(int index);
#endif
