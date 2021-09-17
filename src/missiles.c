#include "missiles.h"

static void removeProjectile(int index){
	projectileCount--;
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
			if (testX != projectiles[i].startX && testY != projectiles[i].startY){
				if (map[testY][testX].spriteIndex != -1){
					if (sprites[map[testY][testX].spriteIndex].id == ID_BOT){
						sprites[map[testY][testX].spriteIndex].show=false;
						removeProjectile(i);
					}
					else{
						removeProjectile(i);
					}
				}
			}
			projectiles[i].x+=projectiles[i].stepX/fps;
			projectiles[i].y+=projectiles[i].stepY/fps;
		}else{
			removeProjectile(i);
		}
	}
}

void addProjectile(int x, int y, int destX, int destY, float speed){
	if (projectileCount < MAX_PROJECTILES){
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
	}
}
