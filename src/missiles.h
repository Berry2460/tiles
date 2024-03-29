#ifndef MISSILES_H
#define MISSILES_H

#define MAX_PROJECTILES 255
#define PROJECTILE_RANGE 15

#include <stdbool.h>

typedef struct Projectile{
	int startX;
	int startY;
	float x;
	float y;
	float destX;
	float destY;
	float stepX;
	float stepY;
	unsigned char textureX;
	unsigned char textureY;
	bool glow;
}Projectile;

extern Projectile projectiles[MAX_PROJECTILES];
extern unsigned char projectileCount;

void moveProjectiles();
void addProjectile(int tx, int ty, int x, int y, int destX, int destY, float speed, bool glow);
#endif
