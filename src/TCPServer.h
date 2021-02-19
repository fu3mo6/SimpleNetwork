#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 

#ifndef WINDOWS
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "WS2_32.lib")
#endif

#include <thread>
#include <algorithm>
#include <cctype>
#include <mutex>
#include <unordered_map>

using namespace std;

#define MAXPACKETSIZE 40960
#define MAX_CLIENT 1000
//#define CODA_MSG 4

struct descript_socket{
#ifndef WINDOWS
	int socket     = -1;
#else
	SOCKET socket;
#endif
	string ip      = "";
	int id         = -1;
};

class TCPServer
{
public:
	int Setup(int port);
	void Loop();
	void ClientLoop(descript_socket *desc);

	virtual void on_accept(int id) {}
	virtual void on_recv(int id, std::string msg) {}
	virtual void on_disconnect(int id) {}
	virtual void on_shutdown() {}

	virtual void SendMsg(int id, std::string msg);
	virtual void BroadcastMsg(std::string msg);
	void Shutdown();

private:
#ifndef WINDOWS
	int sockfd;
	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;
#else
	SOCKET sockfd;
	sockaddr_in serverAddress;
	sockaddr_in clientAddress;
#endif

	unordered_map<int, descript_socket*> client_sock;
	int unique_id;
};

#endif
