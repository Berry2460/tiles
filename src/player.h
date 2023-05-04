#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_LIGHT_BRIGHTNESS 1.3f
#define PLAYER_LIGHT_SIZE 14
#define PLAYER_SPEED 2.2f

#include <stdbool.h>

void playerControl(int index);
void movePlayer(int index);
unsigned char createPlayer(unsigned char frames, unsigned char animation[frames][2], int x, int y);
#endif
