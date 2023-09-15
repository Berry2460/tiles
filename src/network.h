#ifndef NETWORK_H
#define NETWORK_H

#include <stdbool.h>

#define PORT 2000

extern int isHost;
extern int server;
extern int clients[3];
extern int clientCount;
extern char *joinAddr;

int initNetwork();
void updateNetwork();

typedef struct Packet{
	//int index; //player index
	int destX; //player destination
	int destY;
	int shootX; //shoot projectile (-1 for none)
	int shootY;
}Packet;

#endif
