#include "GLFW/glfw3.h"
#include "ai.h"
#include "draw.h"
#include "network.h"

static void newBotRoute(int index);

int newSeed(){
	seed=(seed+(seed>>1)+1)%1000000007; //prime mod
	return seed;
}

void moveBots(){
	for (int i=0; i<spriteCount; i++){
		if (sprites[i].id == ID_BOT){
			if(sprites[i].walk){
				step(i);
			}
			else if (glfwGetTime()-botTimer > BOT_WAIT_TIME){
				newBotRoute(i);
			}
		}
	}
	if (glfwGetTime()-botTimer > BOT_WAIT_TIME){
		botTimer=glfwGetTime();
	}
}

static void newBotRoute(int index){
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
