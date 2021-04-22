#ifndef STEP_H
#define STEP_H

#include "draw.h"

static float speed=2.2f;

static void nextStep(Sprite *s);

void newDest(Sprite *s, int x, int y);
void step(Sprite *s);

#endif
