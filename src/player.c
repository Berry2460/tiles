#include "player.h"

static void shootPlayerProjectile(int index){
	if (!sprites[index].walk){
		addProjectile(2, sprites[index].x, sprites[index].y, mouseTileX, mouseTileY, 5.0f, true);
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
}

void movePlayer(int index){
	Sprite *s=&sprites[index];
	addLight(s->x, s->y, PLAYER_LIGHT_SIZE, PLAYER_LIGHT_BRIGHTNESS, true);
	step(index);
	//follow cam
	camX=s->x+s->offx;
	camY=s->y-s->offy;
	addLight(s->x, s->y, PLAYER_LIGHT_SIZE, PLAYER_LIGHT_BRIGHTNESS, false);
}

unsigned char createPlayer(int tex, int x, int y){
	unsigned char player=addSprite(ID_PLAYER, tex, x, y, PLAYER_SPEED);
	//only create light if valid sprite
	if (player < MAX_SPRITES){
		addLight(x, y, PLAYER_LIGHT_SIZE, PLAYER_LIGHT_BRIGHTNESS, false);
	}
	return player;
}
