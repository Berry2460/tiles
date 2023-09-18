#include <winsock2.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include "GLFW/glfw3.h"

#include "network.h"
#include "step.h"
#include "player.h"
#include "draw.h"
#include "ai.h"
#include "missiles.h"

static WSADATA ws;
static struct sockaddr_in addr;
static pthread_t netThread;
static int alive=1;

static void doPacketRoutine(int dest, int index);
static void *updateNetwork(void *var);

static void doPacketRoutine(int dest, int index){
	if (proceedNetwork){
		proceedNetwork=0;
		Packet out;
		Packet in;

		out.botReady=(glfwGetTime()-botTimer > BOT_WAIT_TIME);
		out.seedCount=getSeedCount();

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
			//desync detection
			if (getSeedCount() < in.seedCount){
				printf("Desync detected! %I64d != %I64d. fixing...\n", getSeedCount(), in.seedCount);
				glfwSetTime(glfwGetTime()+BOT_WAIT_TIME);
				setBotReady(1);
				while (moveBots() > 0){
					movePlayer(-1);
					moveBots();
					moveProjectiles();
				}
			}
			//sync bots
			setBotReady(in.botReady);
			//sync actions
			if (in.destX != -1 && in.destY != -1){
				newDest(index, in.destX, in.destY);
			}
			if (in.shootX != -1 && in.shootY != -1){
				shootPlayerProjectile(index, in.shootX, in.shootY);
			}
		}
		else{
			alive=0;
		}
		free(inBuffer);
		Sleep(1.0f/TICK);
	}
}

static void *updateNetwork(void *var){
	while (alive){
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
	return NULL;
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
		//newSeed();
		addr.sin_addr.s_addr = inet_addr(joinAddr);
		if (connect(server, (struct sockaddr*)&addr, sizeof(struct sockaddr)) < 0){
			return -1;
		}
		clientCount++;
	}
	return 0;
}

void startNetworkThread(){
	pthread_create(&netThread, NULL, updateNetwork, NULL);
}
