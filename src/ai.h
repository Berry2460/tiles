#ifndef AI_H
#define AI_H

#define BOT_WANDER_AREA 3
#define BOT_WAIT_TIME 1.5f

#include "step.h"
#include "draw.h"

extern int seed;
extern double botTimer;

int newSeed();
int modSeed(int tomod);
int moveBots();
void setBotReady(char value);
long long getSeedCount();
#endif
