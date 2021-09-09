#include "ai.h"

void moveBots(){
	for (int i=0; i<botCount; i++){
		step(bots[i]);
	}
}
