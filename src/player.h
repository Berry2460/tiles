#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_LIGHT_BRIGHTNESS 1.3f
#define PLAYER_LIGHT_SIZE 25
#define PLAYER_SPEED 2.2f

#include "draw.h"

static void shootPlayerProjectile(int index);

void playerControl(int index);
void movePlayer(int index);
unsigned char createPlayer(int tex, int x, int y);
#endif
