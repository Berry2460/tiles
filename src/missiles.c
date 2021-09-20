#include "missiles.h"

static void removeProjectile(int index){
	projectileCount--;
	if (projectiles[index].glow){
		addLight((int)round(projectiles[index].x), (int)round(projectiles[index].y), 7, 0.5f, true);
	}
	for(int i=index; i<projectileCount; i++){
		projectiles[i]=projectiles[i+1];
	}
}

void moveProjectiles(){
	for (int i=0; i<projectileCount; i++){
		int testX=round(projectiles[i].x);
		int testY=round(projectiles[i].y);
		if (testX != round(projectiles[i].destX) || testY != round(projectiles[i].destY)){
			//collision
			if (testX != projectiles[i].startX || testY != projectiles[i].startY){
				if (map[testY][testX].spriteIndex != MAX_SPRITES){
					if (sprites[map[testY][testX].spriteIndex].id == ID_BOT){
						removeSprite(map[testY][testX].spriteIndex);
						removeProjectile(i);
						return;
					}
					else{
						removeProjectile(i);
						return;
					}
				}
				else if (map[testY][testX].occupied){
					removeProjectile(i);
					return;
				}
			}
			projectiles[i].x+=projectiles[i].stepX/fps;
			projectiles[i].y+=projectiles[i].stepY/fps;
			if (projectiles[i].glow){
				addLight(testX, testY, 7, 0.5f, true);
				addLight((int)round(projectiles[i].x), (int)round(projectiles[i].y), 7, 0.5f, false);
			}
		}else{
			removeProjectile(i);
		}
	}
}

void addProjectile(int tex, int x, int y, int destX, int destY, float speed, bool glow){
	if (projectileCount < MAX_PROJECTILES && (x != destX || y != destY)){
		projectiles[projectileCount].textureIndex=tex;
		projectiles[projectileCount].glow=glow;
		projectiles[projectileCount].x=x;
		projectiles[projectileCount].y=y;
		projectiles[projectileCount].startX=x;
		projectiles[projectileCount].startY=y;
		float xlen=destX-x;
		float ylen=destY-y;
		float hyp=sqrt((xlen*xlen)+(ylen*ylen));
		projectiles[projectileCount].stepX=xlen/hyp*speed;
		projectiles[projectileCount].stepY=ylen/hyp*speed;
		projectiles[projectileCount].destX=xlen/hyp*PROJECTILE_RANGE+x;
		projectiles[projectileCount].destY=ylen/hyp*PROJECTILE_RANGE+y;
		projectileCount++;
		if (glow){
			addLight(x, y, 7, 0.5f, false);
		}
	}
}
