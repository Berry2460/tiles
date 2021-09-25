#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_LIGHT_BRIGHTNESS 1.5f
#define PLAYER_LIGHT_SIZE 21
#define PLAYER_SPEED 2.2f

#include "draw.h"

static void shootPlayerProjectile(int index);

void playerControl(int index);
void movePlayer(int index);
unsigned char createPlayer(unsigned char *animation, int frames, int x, int y);
#endif
