#include "network.h"
#include <winsock2.h>
#include <stdlib.h>
#include <string.h>

static WSADATA ws;
static struct sockaddr_in addr;

int initNetwork(){
	

	if (WSAStartup(MAKEWORD(2, 2), &ws) < 0){
		return -1;
	}
	if (isHost){
		server=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (server < 0){
			return -1;
		}
		addr.sin_family=AF_INET;
		addr.sin_port=htons(PORT);
		addr.sin_addr.s_addr = inet_addr("0.0.0.0");
		memset(&(addr.sin_zero), 0, 8);
		if (bind(server, (struct sockaddr*)&addr, sizeof(struct sockaddr)) < 0){
			return -1;
		}
	}
	else{
	}
	return 0;
}

int updateNetwork(){
	return 0;
}
