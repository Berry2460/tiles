#include "network.h"
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>

static int hostListen(char* addr, int addrlen){
	char buffer[1024]={0};
	clientCount++;
	return 0;
}

int host(){
	isHost=true;
	clientCount=0;
	return hostListen(NULL, 0);
}

int join(){
	isHost=false;
	return 0;
}

int update(){
	return 0;
}
