#include "player.h"
#include "draw.h"
#include "step.h"
#include "missiles.h"
#include "window.h"
#include "level.h"
#include "ai.h"
#include "network.h"

static int readyPlayerShot;

static unsigned char panim[3*8][2]={ {0, 1}, {1, 1}, {2, 1},
									{0, 2}, {1, 2}, {2, 2},
									{0, 3}, {1, 3}, {2, 3},
									{0, 4}, {1, 4}, {2, 4},
									{0, 5}, {1, 5}, {2, 5},
									{0, 6}, {1, 6}, {2, 6},
									{0, 7}, {1, 7}, {2, 7},
									{0, 8}, {1, 8}, {2, 8} };

static void syncPlayerProjectile(int x, int y){
	readyPlayerShot=0;
	if (!newPlayerProjectile){
		readyPlayerShot=1;
		newPlayerProjectile=1;
		newPlayerProjectileX=x;
		newPlayerProjectileY=y;
		
	}
}

void shootPlayerProjectile(int index, int x, int y, int dummy){
	if (!sprites[index].walk){
		if (((readyPlayerShot || dummy) && clientCount-1 == currClientCount) || clientCount == 1){
			addProjectile(2, 0, sprites[index].x, sprites[index].y, x, y, 5.0f, true);
			//animation
			sprites[index].time=glfwGetTime();
			//sprites[index].frame=3;
		}
	}else{
		newDest(index, sprites[index].toStepX, sprites[index].toStepY);
	}
	keys[LMB]=false;
}

void playerControl(int index){
	//hold position fire
	if (keys[SHIFT] && keys[LMB]){
		syncPlayerProjectile(mouseTileX, mouseTileY);
		shootPlayerProjectile(index, mouseTileX, mouseTileY, 0);
	}
	//player movement with mouse and target fire
	else if (keys[LMB]){
		if (map[mouseTileY][mouseTileX].spriteIndex != MAX_SPRITES && sprites[map[mouseTileY][mouseTileX].spriteIndex].id == ID_BOT){
			syncPlayerProjectile(mouseTileX, mouseTileY);
			shootPlayerProjectile(index, mouseTileX, mouseTileY, 0);
		}
		else if (map[mouseTileY+1][mouseTileX+1].spriteIndex != MAX_SPRITES && sprites[map[mouseTileY+1][mouseTileX+1].spriteIndex].id == ID_BOT){
			mouseTileY+=1;
			mouseTileX+=1;
			syncPlayerProjectile(mouseTileX, mouseTileY);
			shootPlayerProjectile(index, mouseTileX, mouseTileY, 0);
		}else{
			newDest(index, mouseTileX, mouseTileY);
		}
	}
	/*
	else if (keys[RMB]){
		map[mouseTileY][mouseTileX].textureX=0;
		map[mouseTileY][mouseTileX].wall=false;
		map[mouseTileY][mouseTileX].occupied=false;
	}
	else if (keys[KEY_D]){
		Light light[1];
		light[0].x=mouseTileX;
		light[0].y=mouseTileY;
		light[0].size=13;
		light[0].brightness=0.6f;
		computeLightMap(light, 1, false);
		keys[KEY_D]=false;
	}*/
}

void movePlayer(int index){
	if (index > 0){
		Sprite *s=&sprites[index];
		addLight(s->x, s->y, PLAYER_LIGHT_SIZE, PLAYER_LIGHT_BRIGHTNESS, true);
		step(index);
		//follow cam
		camX=s->x+s->offx;
		camY=s->y-s->offy;
		addLight(s->x, s->y, PLAYER_LIGHT_SIZE, PLAYER_LIGHT_BRIGHTNESS, false);
	}
	//move dummy players
	for (int i=0; i<clientCount; i++){
		if (clientIndices[i] >= 0){
			step(clientIndices[i]);
		}
	}
}

int createPlayer(unsigned char frames, bool directional, int x, int y){
	int player=addSprite(ID_PLAYER, directional, frames, panim, x, y, PLAYER_SPEED);
	//only create light if valid sprite
	if (player < MAX_SPRITES){
		addLight(x, y, PLAYER_LIGHT_SIZE, PLAYER_LIGHT_BRIGHTNESS, false);
	}
	return player;
}

int createDummyPlayer(unsigned char frames, bool directional, int x, int y){
	int player=addSprite(ID_PLAYER, directional, frames, panim, x, y, PLAYER_SPEED);
	return player;
}
