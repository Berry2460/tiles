#include "player.h"

void playerControl(int index){
	//hold position fire
	if (keys[A] && keys[LMB] && !sprites[index].walk){
		addProjectile(sprites[index].x, sprites[index].y, mouseTileX, mouseTileY, 5.0f);
		keys[LMB]=false;
	}
	//player movement with mouse and target fire
	else if (keys[LMB]){
		if (sprites[map[mouseTileY][mouseTileX].spriteIndex].id == ID_BOT){
			if (!sprites[index].walk){
				addProjectile(sprites[index].x, sprites[index].y, mouseTileX, mouseTileY, 5.0f);
			}else{
				//stop walking when trying to fire
				newDest(index, sprites[index].toStepX, sprites[index].toStepY);
			}
		}else{
			newDest(index, mouseTileX, mouseTileY);
		}
		keys[LMB]=false;
	}
}

void movePlayer(int index){
	Sprite *s=&sprites[index];
	addLight(s->x, s->y, 2*WIN_Y/TILE_Y,true);
	step(index);
	//follow cam
	camX=s->x+s->offx;
	camY=s->y-s->offy;
	addLight(s->x, s->y, 2*WIN_Y/TILE_Y,false);
}
