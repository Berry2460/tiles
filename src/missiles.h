#ifndef MISSILES_H
#define MISSILES_H

#include "draw.h"

extern char projectiles[MAX_SPRITES];
static char projectileCount=0;

static void removeProjectile(int index);
void moveProjectiles();
void addProjectile(int x, int y, int destX, int destY, float speed);

#endif
