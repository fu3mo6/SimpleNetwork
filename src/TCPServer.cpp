#include <string>
#include "TCPServer.h" 

void TCPServer::ClientLoop(std::shared_ptr<descript_socket> desc)
{
	int n;
	char msg[MAXPACKETSIZE];
	while(desc->connected)
	{
		n = recv(desc->socket, msg, MAXPACKETSIZE, 0);
		if(n != -1) 
		{
			if(n==0)
			{
				desc->connected = false;
				break;
			}
			else {
				on_recv(desc->id, string(msg));
			}
		}
		usleep(600);
    }

	on_disconnect(desc->id);
#ifdef WINDOWS
	closesocket(desc->socket);
#else
	close(desc->socket);
#endif

	client_sock.erase(desc->id);
}

int TCPServer::Setup(int port)
{
	unique_id = 0;

#ifndef WINDOWS
	int opt = 1;
    vector<int> opts = { SO_REUSEPORT, SO_REUSEADDR };
	sockfd = socket(AF_INET,SOCK_STREAM,0);
 	memset(&serverAddress,0,sizeof(serverAddress));

	for(unsigned int i = 0; i < opts.size(); i++) {
		if( (setsockopt(sockfd, SOL_SOCKET, opts.size(), (char *)&opt, sizeof(opt))) < 0 ) {
			cerr << "Errore setsockopt" << endl; 
      			return -1;
	      	}
	}

	serverAddress.sin_family      = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port        = htons(port);

	if((::bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress))) < 0){
		cerr << "Errore bind" << endl;
		return -1;
	}
	
#else
    WSADATA wsaData;
    SOCKADDR_IN addr;

    // Initialize Winsock
    WSAStartup(MAKEWORD(2,2), &wsaData);

    //設定位址資訊的資料
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
 
    //設定 Listen
    sockfd = socket(AF_INET, SOCK_STREAM, NULL);
    bind(sockfd, (SOCKADDR*)&addr, sizeof(addr));
#endif

 	if(listen(sockfd,20) < 0){
		cerr << "Errore listen" << endl;
		return -1;
	}
	connected = true;
	return 0;
}

void TCPServer::Loop()
{
	socklen_t sosize    = sizeof(clientAddress);
	while(connected)
	{
#ifndef WINDOWS		
		int new_client_so 	= accept(sockfd, (struct sockaddr*)&clientAddress, &sosize);
#else
		SOCKET new_client_so = accept(sockfd, (SOCKADDR*)&clientAddress, &sosize);
#endif

		auto so = std::make_shared<descript_socket>();
		so->socket = new_client_so;
		so->id = unique_id ++ ;
		so->ip = inet_ntoa(clientAddress.sin_addr);
		so->connected = true;
		client_sock[so->id] = so;
		
		on_accept(so->id);
		std::thread cthread(&TCPServer::ClientLoop, this, so);
		cthread.detach();
	}

	while(!client_sock.empty())
	{
		cout << "Wait for client socket cleanup" << endl;
		sleep(1);
	}

#ifndef WINDOWS	
	close(sockfd);
#else
    closesocket(sockfd);
    WSACleanup();
#endif
}

void TCPServer::SendMsg(int id, std::string msg)
{
	send(client_sock[id]->socket, msg.c_str(), msg.length(), 0);
}

void TCPServer::BroadcastMsg(std::string msg)
{
	for(auto it = client_sock.begin(); it != client_sock.end(); it++)
	{
		SendMsg(it->second->id, msg);
	}
}
/*
void TCPServer::detach(int id)
{
	close(newsockfd[id]->socket);
	newsockfd[id]->ip = "";
	newsockfd[id]->id = -1;
	newsockfd[id]->message = "";
} */

void TCPServer::Shutdown() 
{
	on_shutdown();
	for(auto it = client_sock.begin(); it != client_sock.end(); it++)
	{
		it->second->connected = false;
#ifdef WINDOWS
		shutdown(it->second->socket, SD_SEND);
#endif
	}
}

