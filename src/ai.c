#include "GLFW/glfw3.h"
#include "ai.h"
#include "draw.h"
#include "network.h"

static void newBotRoute(int index);

static long long seedCount=0;
static char botReady=0;

int newSeed(){
	seed=(seed+(seed>>1)+1)%1000000007; //prime mod
	seedCount++;
	return seed;
}

long long getSeedCount(){
	return seedCount;
}

void setBotReady(char value){
	botReady=value;
}

int moveBots(){
	int moved=0;
	int updating=0;
	for (int i=0; i<spriteCount; i++){
		if (sprites[i].id == ID_BOT){
			if(sprites[i].walk){
				step(i);
				moved++;
			}
			else if ((glfwGetTime()-botTimer > BOT_WAIT_TIME && botReady) || updating){
				updating=1;
				newBotRoute(i);
			}
		}
	}
	if ((glfwGetTime()-botTimer > BOT_WAIT_TIME && botReady) || updating){
		botTimer=glfwGetTime();
	}
	if (botReady){
		botReady=0;
	}
	return moved;
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
