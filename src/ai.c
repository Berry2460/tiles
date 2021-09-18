#include "ai.h"

void newSeed(){
	seed=(seed+(seed>>1)+1)%1000000000;
}

void moveBots(){
	for (int i=0; i<botCount; i++){
		if (sprites[bots[i]].show){
			if(sprites[bots[i]].walk){
				step(bots[i]);
			}
			else if (glfwGetTime()-sprites[bots[i]].time > BOT_WAIT_TIME){
				newBotRoute(bots[i]);
			}
		}
		else if (map[sprites[bots[i]].y][sprites[bots[i]].x].spriteIndex == bots[i]){
			//remove bot (does not clean up sprite array space)
			map[sprites[bots[i]].y][sprites[bots[i]].x].occupied=false;
			map[sprites[bots[i]].y][sprites[bots[i]].x].spriteIndex=MAX_SPRITES;
			if (sprites[bots[i]].walk){
				map[sprites[bots[i]].toStepY][sprites[bots[i]].toStepX].occupied=false;
			}
		}
	}
}

static void newBotRoute(int index){
	sprites[index].time=glfwGetTime();
	int x=sprites[index].x;
	int y=sprites[index].y;
	newSeed();
	int neg=(seed%2 * -2)+1;
	newSeed();
	int addX=((seed*x)%BOT_WANDER_AREA)*neg;
	newSeed();
	int addY=((seed*y)%BOT_WANDER_AREA)*neg;
	x+=addX;
	y+=addY;
	newDest(index, x, y);
}
