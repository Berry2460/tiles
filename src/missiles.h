#ifndef MISSILES_H
#define MISSILES_H

#define MAX_PROJECTILES 255
#define PROJECTILE_RANGE 15

#include <stdbool.h>
#include "draw.h"

typedef struct Projectile{
	int startX;
	int startY;
	float x;
	float y;
	float destX;
	float destY;
	float stepX;
	float stepY;
	bool glow;
}Projectile;

extern Projectile projectiles[MAX_PROJECTILES];
extern unsigned char projectileCount;

static void removeProjectile(int index);
void moveProjectiles();
void addProjectile(int x, int y, int destX, int destY, float speed, bool glow);

#endif
