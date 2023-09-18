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

int modSeed(int tomod){
	int mod=(tomod+(tomod>>1)+1)%1000000007;
	return mod;
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
		newSeed();
	}
	if (botReady){
		botReady=0;
	}
	return moved;
}

static void newBotRoute(int index){
	int x=sprites[index].x;
	int y=sprites[index].y;
	int mod=modSeed(seed);
	int neg=(mod%2 * -2)+1;
	mod=modSeed(mod);
	int addX=((mod*x)%BOT_WANDER_AREA)*neg;
	mod=modSeed(mod);
	int addY=((mod*y)%BOT_WANDER_AREA)*neg;
	x+=addX;
	y+=addY;
	newDest(index, x, y);
}
