#include "missiles.h"

static void removeProjectile(int index){
	projectileCount--;
	for(int i=index; i<projectileCount; i++){
		projectiles[i]=projectiles[i+1];
	}
}

void moveProjectiles(){
	for (int i=0; i<projectileCount; i++){
		if ((int)projectiles[i].x != (int)projectiles[i].destX){
			projectiles[i].x+=projectiles[i].stepX/fps;
		}
		if ((int)projectiles[i].y != (int)projectiles[i].destY){
			projectiles[i].y+=projectiles[i].stepY/fps;
		}
		if (((int)projectiles[i].x == (int)projectiles[i].destX && (int)projectiles[i].y == (int)projectiles[i].destY)){
			removeProjectile(i);
		}
	}
}

void addProjectile(int x, int y, int destX, int destY, float speed){
	if (projectileCount < MAX_PROJECTILES){
		projectiles[projectileCount].x=x;
		projectiles[projectileCount].y=y;
		projectiles[projectileCount].destX=destX;
		projectiles[projectileCount].destY=destY;
		float xlen=projectiles[projectileCount].destX-projectiles[projectileCount].x;
		float ylen=projectiles[projectileCount].destY-projectiles[projectileCount].y;
		float hyp=sqrt((xlen*xlen)+(ylen*ylen));
		projectiles[projectileCount].stepX=xlen/hyp*speed;
		projectiles[projectileCount].stepY=ylen/hyp*speed;
		//integer rounding correction
		if (projectiles[projectileCount].stepX < 0.0f){
			projectiles[projectileCount].destX-=1;
		}
		if (projectiles[projectileCount].stepY < 0.0f){
			projectiles[projectileCount].destY-=1;
		}
		projectileCount++;
	}
}
