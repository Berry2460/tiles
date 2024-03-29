#ifndef NETWORK_H
#define NETWORK_H

#include <stdbool.h>

#define PORT 2000
#define TICK 10 //hertz

#define NORMAL_FLAG 0
#define DISCONNECT_FLAG 255

extern int isHost;
extern int server;
extern int clientSocket[64];
extern int clientIndices[64];
extern int clientIndex;
extern int playerIndex;
extern char *joinAddr;
extern int proceedNetwork;
extern int clientCount;
extern int currClientCount;
extern int spawnX;
extern int spawnY;

void startNetworkThread();

typedef struct Packet{
	char botReady; //sync bots
	unsigned char fromPlayer; //dedicate packets to players
	unsigned char flag;
	int destX; //player destination
	int destY;
	int shootX; //shoot projectile (-1 for none)
	int shootY;
	long long seedCount; //sync seed
}Packet;

#endif
