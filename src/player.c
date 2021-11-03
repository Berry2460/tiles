#include "player.h"

static void shootPlayerProjectile(int index);

static void shootPlayerProjectile(int index){
	if (!sprites[index].walk){
		addProjectile(4, sprites[index].x, sprites[index].y, mouseTileX, mouseTileY, 5.0f, true);
		//animation
		sprites[index].time=glfwGetTime();
		sprites[index].textureIndex=3;
	}else{
		newDest(index, sprites[index].toStepX, sprites[index].toStepY);
	}
	keys[LMB]=false;
}

void playerControl(int index){
	//hold position fire
	if (keys[SHIFT] && keys[LMB]){
		shootPlayerProjectile(index);
	}
	//player movement with mouse and target fire
	else if (keys[LMB]){
		if (map[mouseTileY][mouseTileX].spriteIndex != MAX_SPRITES && sprites[map[mouseTileY][mouseTileX].spriteIndex].id == ID_BOT){
			shootPlayerProjectile(index);
		}else{
			newDest(index, mouseTileX, mouseTileY);
			keys[LMB]=false;
		}
	}
	else if (keys[RMB]){
		map[mouseTileY][mouseTileX].textureIndex=1;
		map[mouseTileY][mouseTileX].wall=true;
		map[mouseTileY][mouseTileX].occupied=true;
	}
}

void movePlayer(int index){
	Sprite *s=&sprites[index];
	//attack animation
	if (glfwGetTime()-s->time > 0.3f && !s->walk){
		s->textureIndex=0; //temp
	}
	addLight(s->x, s->y, PLAYER_LIGHT_SIZE, PLAYER_LIGHT_BRIGHTNESS, true);
	step(index);
	//follow cam
	camX=s->x+s->offx;
	camY=s->y-s->offy;
	addLight(s->x, s->y, PLAYER_LIGHT_SIZE, PLAYER_LIGHT_BRIGHTNESS, false);
}

unsigned char createPlayer(unsigned char *animation, int frames, int x, int y){
	unsigned char player=addSprite(ID_PLAYER, animation, frames, x, y, PLAYER_SPEED);
	//only create light if valid sprite
	if (player < MAX_SPRITES){
		addLight(x, y, PLAYER_LIGHT_SIZE, PLAYER_LIGHT_BRIGHTNESS, false);
	}
	return player;
}
