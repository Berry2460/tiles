#ifndef NETWORK_H
#define NETWORK_H

#include <stdbool.h>

#define PORT 2000

extern int isHost;
extern int server;
extern int clientSocket[3];
extern int clientIndex[3];
extern int playerIndex;
extern int clientCount;
extern char *joinAddr;

int initNetwork();
void updateNetwork();

typedef struct Packet{
	char newJoin; //flag for new joiners
	char pnum; //player number
	double time; //pass clocks
	double botTime;
	int destX; //player destination
	int destY;
	int shootX; //shoot projectile (-1 for none)
	int shootY;
}Packet;

#endif
