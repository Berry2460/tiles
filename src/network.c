#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "GLFW/glfw3.h"

#include "network.h"
#include "step.h"
#include "player.h"
#include "draw.h"
#include "ai.h"

static WSADATA ws;
static struct sockaddr_in addr;

static void doPacketRoutine(int dest, int index){
	Packet out;
	Packet in;

	out.time=glfwGetTime();
	out.botTime=botTimer;

	if (sprites[playerIndex].walk){
		out.destX=sprites[playerIndex].stepDestX;
		out.destY=sprites[playerIndex].stepDestY;
	}
	else{
		out.destX=-1;
		out.destY=-1;
	}
	
	if (newPlayerProjectile){
		out.shootX=newPlayerProjectileX;
		out.shootY=newPlayerProjectileY;
		newPlayerProjectile=0;
	}
	else{
		out.shootX=-1;
		out.shootY=-1;
	}
	
	char *inBuffer=malloc(sizeof(Packet));
	char *outBuffer=(char *)&out;
	send(dest, outBuffer, sizeof(Packet), 0);
	int result=recv(dest, inBuffer, sizeof(Packet), 0);
	if (result >= 0){
		in=*((Packet *)inBuffer);
		//sync time
		botTimer=(botTimer+in.botTime)/2;
		glfwSetTime((in.time+out.time)/2);
		if (in.destX != -1 && in.destY != -1){
			newDest(index, in.destX, in.destY);
		}
		if (in.shootX != -1 && in.shootY != -1){
			shootPlayerProjectile(index, in.shootX, in.shootY);
		}
		step(index);
	}
	free(inBuffer);
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
		clientSocket[0]=accept(server, NULL, NULL);
		clientCount++;
	}
	else{ //join host
		newSeed();
		addr.sin_addr.s_addr = inet_addr(joinAddr);
		if (connect(server, (struct sockaddr*)&addr, sizeof(struct sockaddr)) < 0){
			return -1;
		}
		clientCount++;
	}
	return 0;
}

void updateNetwork(){
	if (isHost){
		for (int i=0; i<clientCount; i++){
			doPacketRoutine(clientSocket[i], clientIndex[i]);
		}
	}
	else{
		for (int i=0; i<clientCount; i++){
			doPacketRoutine(server, clientIndex[0]);
		}
	}
}
