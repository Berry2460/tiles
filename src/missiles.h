#ifndef MISSILES_H
#define MISSILES_H

#define MAX_PROJECTILES 255

#include "draw.h"

typedef struct Projectile{
	float x;
	float y;
	float destX;
	float destY;
	float stepX;
	float stepY;
	float speed;
}Projectile;

extern Projectile projectiles[MAX_PROJECTILES];
extern unsigned char projectileCount;

static void removeProjectile(int index);
void moveProjectiles();
void addProjectile(int x, int y, int destX, int destY, float speed);

#endif
