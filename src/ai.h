#ifndef AI_H
#define AI_H

#define BOT_WANDER_AREA 3
#define BOT_WAIT_TIME 1.5f

#include "step.h"

extern int seed;
extern char bots[MAX_SPRITES];
extern char botCount;

static void newBotRoute();

void newSeed();
void moveBots();
#endif
