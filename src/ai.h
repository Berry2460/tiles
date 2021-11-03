#ifndef AI_H
#define AI_H

#define BOT_WANDER_AREA 3
#define BOT_WAIT_TIME 1.5f

#include "step.h"

extern int seed;
extern unsigned char bots[MAX_SPRITES];
extern unsigned char botCount;

void newSeed();
void moveBots();
#endif
