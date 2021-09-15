#ifndef AI_H
#define AI_H

#include "draw.h"
#include "step.h"

#define BOT_WANDER_AREA 3
#define BOT_WAIT_TIME 1.5f

extern int seed;

static void newBotRoute();

void newSeed();
void moveBots();
#endif
