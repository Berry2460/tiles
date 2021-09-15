#include "missiles.h"

static void removeProjectile(int pindex){
	projectileCount--;
	spriteCount--;
	map[sprites[projectiles[pindex]].y][sprites[projectiles[pindex]].x].spriteIndex=-1;
	map[sprites[projectiles[pindex]].y][sprites[projectiles[pindex]].x].occupied=false;
	for(int i=projectiles[pindex]; i<spriteCount; i++){
		sprites[i]=sprites[i+1];
	}
	for(int i=0; i<projectileCount; i++){
		if (projectiles[i] >= projectiles[pindex]){
			projectiles[i]=projectiles[i+1]-1;
			map[sprites[projectiles[i]].y][sprites[projectiles[i]].x].spriteIndex=projectiles[i];
		}
	}
}

void moveProjectiles(){

	for (int i=0; i<projectileCount; i++){
		if(sprites[projectiles[i]].walk){
			step(projectiles[i]);
		}
		else{
			removeProjectile(i);
		}
	}
}

void addProjectile(int x, int y, int destX, int destY, float speed){
	char oldIndex=map[y][x].spriteIndex;
	char oldOccupied=map[y][x].occupied;
	int index=addSprite(ID_PROJECTILE, x, y, speed);
	if (index != -1){
		newDest(index, destX, destY);
		nextStep(index);
		map[y][x].spriteIndex=oldIndex;
		map[y][x].occupied=oldOccupied;
		projectiles[projectileCount]=index;
		projectileCount++;
	}
}
