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

static void doPacketRoutine(int dest, int toPlayer);
static void updateNetwork();
static void *initNetwork(void *var);
static void sendConnectPacket(int dest);

static void doPacketRoutine(int dest, int toPlayer){

	//TODO: need to have server load X packets and send X packets

	if (proceedNetwork){
		proceedNetwork=0;
		Packet out;
		Packet in;

		out.botReady=(glfwGetTime()-botTimer > BOT_WAIT_TIME);
		out.seedCount=getSeedCount();
		out.fromPlayer=clientIndex;

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

			//sync dummy players at start
			if (clientIndices[in.fromPlayer] == -1){
				clientIndices[in.fromPlayer]=createDummyPlayer(3, true, camX+in.fromPlayer, camY);
			}
			//desync detection
			if (!isHost && getSeedCount() < in.seedCount){
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
				newDest(clientIndices[in.fromPlayer], in.destX, in.destY);
			}
			if (in.shootX != -1 && in.shootY != -1){
				shootPlayerProjectile(clientIndices[in.fromPlayer], in.shootX, in.shootY, 1);
			}
		}
		else{
			alive=0;
		}
		free(inBuffer);
		Sleep(1000.0f/TICK);
	}
}

static void updateNetwork(){
	while (alive){
		for (int i=1; i<playerCount; i++){
			if (i != clientIndex){
				if (isHost){
					doPacketRoutine(clientSocket[i], clientIndices[i]);
				}
				else{
					doPacketRoutine(server, clientIndices[i]);
				}
			}
		}
	}
}

static void sendConnectPacket(int dest){
	Packet tosend;
	clientIndices[clientCount-1]=createDummyPlayer(3, true, camX+clientCount, camY);
	tosend.fromPlayer=clientCount; //use fromPlayer param to sync client indices
	char *buffer=(char *)&tosend;
	send(dest, buffer, sizeof(Packet), 0);
}

static void getConnectPacket(int dest){
	Packet in;
	char *buffer=malloc(sizeof(Packet));
	int result=recv(dest, buffer, sizeof(Packet), 0);
	if (result >= 0){
		in=*((Packet *)buffer);
		clientIndex=in.fromPlayer; //sync client indices
		playerIndex=createPlayer(3, true, camX+clientIndex, camY);
	}
}

static void *initNetwork(void *var){
	//init client array
	for (int i=0; i<playerCount; i++){
		clientIndices[i]=-1;
	}

	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0 || playerCount == 1){
		return NULL;
	}
	server=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (server < 0){
		return NULL;
	}
	addr.sin_family=AF_INET;
	addr.sin_port=htons(PORT);
	memset(&(addr.sin_zero), 0, 8);

	if (isHost){ //start hosting
		clientIndex=0;
		addr.sin_addr.s_addr = inet_addr("0.0.0.0");
		char opt=0;
		if (setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(char)) < 0){
			return NULL;
		}
		if (bind(server, (struct sockaddr*)&addr, sizeof(struct sockaddr)) < 0){
			return NULL;
		}
		if (listen(server, 5) < 0){
			return NULL;
		}
		while (clientCount < playerCount-1){
			clientSocket[clientCount]=accept(server, NULL, NULL);
			clientCount++;
			sendConnectPacket(clientCount);
		}
	}
	else{ //join host
		addr.sin_addr.s_addr = inet_addr(joinAddr);
		if (connect(server, (struct sockaddr*)&addr, sizeof(struct sockaddr)) < 0){
			return NULL;
		}
		else{
			getConnectPacket(server);
		}
	}
	updateNetwork();
	return NULL;
}

void startNetworkThread(){
	pthread_create(&netThread, NULL, initNetwork, NULL);
}
