#ifndef NETWORK_H
#define NETWORK_H

#include <stdbool.h>

#define PORT 2000
#define TICK 10 //hertz

extern int isHost;
extern int server;
extern int clientSocket[3];
extern int clientIndex[3];
extern int playerIndex;
extern int clientCount;
extern char *joinAddr;
extern int proceedNetwork;

int initNetwork();
void startNetworkThread();

typedef struct Packet{
	char botReady; //sync bots
	long long seedCount; //sync seed
	int destX; //player destination
	int destY;
	int shootX; //shoot projectile (-1 for none)
	int shootY;
}Packet;

#endif
