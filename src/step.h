#ifndef STEP_H
#define STEP_H

#include <stdbool.h>
#include <stdio.h>
#include "draw.h"

static float speed=2.2f;

static int stepDestX;
static int stepDestY;
static int toStepX;
static int toStepY;
static int steps;
static int maxSteps;
static int nextY;
static int nextX;
static float cX;
static float cY;

void newDest(Sprite *s, int x, int y);
void step(Sprite *s);
#endif
