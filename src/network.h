#ifndef NETWORK_H
#define NETWORK_H

#include <stdbool.h>

#define PORT 2000

extern bool isHost;
extern int server;
extern int clients[3];
extern int clientCount;

int host();
int join();
int update();

#endif
