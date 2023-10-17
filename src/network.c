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

static void updateNetwork();
static void *initNetwork(void *var);
static void sendConnectPacket(int dest);
static void getConnectPacket();

static void updateNetwork(){
	if (proceedNetwork){
		proceedNetwork=0;

		int totalResponses=0;
		int botReadyCount=0;

		//load outgoing packet
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

		//send packets
		if (isHost){
			//server sends to all clients
			for (int i=0; i<clientCount-1; i++){
				send(clientSocket[i], outBuffer, sizeof(Packet), 0);
			}
		}
		else{
			//client sends data to server
			send(server, outBuffer, sizeof(Packet), 0);
		}

		//process data
		for (int i=0; i<clientCount-1; i++){
			//recieve packets
			int result=-1;
			
			if (isHost){
				result=recv(clientSocket[i], inBuffer, sizeof(Packet), 0);
			}
			else{
				result=recv(server, inBuffer, sizeof(Packet), 0);
			}
			if (result >= 0){
				totalResponses++;
				in=*((Packet *)inBuffer);

				//sync dummy players at start
				if (clientIndices[in.fromPlayer] == -1){
					clientIndices[in.fromPlayer]=createDummyPlayer(3, true, spawnX+in.fromPlayer, camY);
				}
				//update client count
				if (in.fromPlayer > currClientCount){
					currClientCount=in.fromPlayer;
				}

				//relay client packets to other clients
				if (isHost){
					for (int i=0; i<clientCount-1; i++){
						if (i != in.fromPlayer-1){
							send(clientSocket[i], inBuffer, sizeof(Packet), 0);
						}
					}
					in.fromPlayer-=1;
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
				//collect bot states
				botReadyCount+=in.botReady;
				//sync actions
				if (in.destX != -1 && in.destY != -1){
					newDest(clientIndices[in.fromPlayer], in.destX, in.destY);
				}
				if (in.shootX != -1 && in.shootY != -1){
					shootPlayerProjectile(clientIndices[in.fromPlayer], in.shootX, in.shootY, 1);
				}
			}
		}
		free(inBuffer);
		if (!totalResponses){
			printf("LOST CONNECTION!\n");
			alive=false;
		}
		else{
			setBotReady((botReadyCount == clientCount-1));
		}
	}
}

static void sendConnectPacket(int dest){
	Packet tosend;
	clientIndices[currClientCount]=createDummyPlayer(3, true, spawnX+currClientCount+1, spawnY);
	tosend.fromPlayer=currClientCount+1; //use fromPlayer param to sync client indices
	char *buffer=(char *)&tosend;
	send(dest, buffer, sizeof(Packet), 0);
}

static void getConnectPacket(){
	Packet in;
	char *buffer=malloc(sizeof(Packet));
	int result=recv(server, buffer, sizeof(Packet), 0);
	if (result >= 0){
		in=*((Packet *)buffer);
		clientIndex=in.fromPlayer; //sync client indices
		camX+=clientIndex;
		currClientCount=clientIndex;
		clientIndices[clientIndex]=-2;
		playerIndex=createPlayer(3, true, spawnX+clientIndex, spawnY);
	}
	printf("client index: %d\n",clientIndex);
}

static void *initNetwork(void *var){
	//init client array
	currClientCount=0;
	for (int i=0; i<clientCount; i++){
		clientIndices[i]=-1;
	}

	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0 || clientCount == 1){
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
		while (currClientCount < clientCount-1){
			clientSocket[currClientCount]=accept(server, NULL, NULL);
			sendConnectPacket(clientSocket[currClientCount]);
			currClientCount++;
		}
	}
	else{ //join host
		addr.sin_addr.s_addr = inet_addr(joinAddr);
		if (connect(server, (struct sockaddr*)&addr, sizeof(struct sockaddr)) < 0){
			return NULL;
		}
		getConnectPacket();
	}
	while (alive){
		updateNetwork();
		Sleep(1000.0f/TICK);
	}
	return NULL;
}

void startNetworkThread(){
	pthread_create(&netThread, NULL, initNetwork, NULL);
}
