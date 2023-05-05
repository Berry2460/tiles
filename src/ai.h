#ifndef AI_H
#define AI_H

#define BOT_WANDER_AREA 3
#define BOT_WAIT_TIME 1.5f

#include "step.h"
#include "draw.h"

extern int seed;
extern int bots[MAX_SPRITES];
extern int botCount;

int newSeed();
void moveBots();
#endif
