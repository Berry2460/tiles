#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "network.h"
#include "step.h"
#include "player.h"
#include "draw.h"

static WSADATA ws;
static struct sockaddr_in addr;

static void doPacketRoutine(int dest, int index){
	Packet out;
	Packet in;
	out.destX=0;
	out.destY=0;
	out.shootX=-1;
	out.shootY=-1;
	/*
	char *outBuffer=malloc(sizeof(Packet));
	int result=send(dest, outBuffer, sizeof(Packet), 0);
	result=recv(dest, inBuffer, sizeof(Packet), 0);
	newDest(in.destX, in.destY);
	if (in.shootX != -1 && in.shootY != -1){
		shootPlayerProjectile(index, in.shootX, in.shootY);
	}
	step(index);*/
}

int initNetwork(){
	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0){
		return -1;
	}
	server=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server < 0){
		return -1;
	}
	addr.sin_family=AF_INET;
	addr.sin_port=htons(PORT);
	memset(&(addr.sin_zero), 0, 8);

	if (isHost){ //start hosting
		addr.sin_addr.s_addr = inet_addr("0.0.0.0");
		char opt=0;
		if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(char)) < 0){
			return -1;
		}
		if (bind(server, (struct sockaddr*)&addr, sizeof(struct sockaddr)) < 0){
			return -1;
		}
		if (listen(server, 5) < 0){
			return -1;
		}
		printf("Waiting for client...");
		clients[0]=accept(server, NULL, NULL);
		clientCount++;
	}
	else{ //join host
		addr.sin_addr.s_addr = inet_addr(joinAddr);
		if (connect(server, (struct sockaddr*)&addr, sizeof(struct sockaddr)) < 0){
			return -1;
		}
	}
	return 0;
}

void updateNetwork(){
	if (isHost){
		for (int i; i<clientCount; i++){
			doPacketRoutine(clients[i], i+1);
		}
	}
	else{
		doPacketRoutine(server, 1);
	}
}
