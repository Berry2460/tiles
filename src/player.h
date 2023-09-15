#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_LIGHT_BRIGHTNESS 1.3f
#define PLAYER_LIGHT_SIZE 14
#define PLAYER_SPEED 2.2f

#include <stdbool.h>

extern int newPlayerProjectile;
extern int newPlayerProjectileX;
extern int newPlayerProjectileY;

void playerControl(int index);
void movePlayer(int index);
int createPlayer(unsigned char frames, bool directional, unsigned char animation[][2], int x, int y);
int createDummyPlayer(unsigned char frames, bool directional, unsigned char animation[][2], int x, int y);
void shootPlayerProjectile(int index, int x, int y);
int didPlayerShoot();
#endif
