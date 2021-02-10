#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
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
	int socket     = -1;
	string ip      = "";
	int id         = -1;
};

class TCPServer
{
public:
	int setup(int port, vector<int> opts = vector<int>());
	void loop();
	void client_loop(descript_socket *desc);

	virtual void on_accept(int id) {}
	virtual void on_recv(int id, std::string msg) {}
	virtual void on_disconnect(int id) {}

	void send_msg(int id, std::string msg);
	void broadcast_msg(std::string msg);
	void shutdown();

private:
	int sockfd;
	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;

	unordered_map<int, descript_socket*> client_sock;
	int unique_id;
};

#endif
