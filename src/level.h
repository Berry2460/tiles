#ifndef LEVEL_H
#define LEVEL_H

#include "draw.h"

#define ROOM_SIZE_MAX 13
#define ROOM_SIZE_MIN 6

void computeLightMap(Light *lights, int total, bool neg);
void addLight(int x, int y, int size, float brightness, bool neg);
void initLight();
void generateLevel(Texture *texture, int x, int y);

#endif