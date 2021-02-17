#include <string>
#include "TCPServer.h" 

void TCPServer::client_loop(descript_socket *desc)
{
	int n;
	char msg[MAXPACKETSIZE];
	while(1)
	{
		n = recv(desc->socket, msg, MAXPACKETSIZE, 0);
		if(n != -1) 
		{
			if(n==0)
			{
				on_disconnect(desc->id);
				close(desc->socket);
				client_sock.erase(desc->id);
				break;
			}
			else {
				on_recv(desc->id, string(msg));
			}
		}
		usleep(600);
    }

	if(desc != NULL)
		free(desc);
}

int TCPServer::setup(int port, vector<int> opts)
{
	int opt = 1;

	unique_id = 0;
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
	
 	if(listen(sockfd,5) < 0){
		cerr << "Errore listen" << endl;
		return -1;
	}
	
	return 0;
}

void TCPServer::loop()
{
	socklen_t sosize    = sizeof(clientAddress);
	while(1)
	{
		int new_client_so 	= accept(sockfd,(struct sockaddr*)&clientAddress,&sosize);

		descript_socket *so = new descript_socket;
		so->socket          = new_client_so;
		so->id              = unique_id ++ ;
		so->ip              = inet_ntoa(clientAddress.sin_addr);
		client_sock[so->id] = so;
		
		on_accept(so->id);
		std::thread cthread(&TCPServer::client_loop, this, so);
		cthread.detach();
	}
}

void TCPServer::send_msg(int id, std::string msg)
{
	send(client_sock[id]->socket, msg.c_str(), msg.length(), 0);
}

void TCPServer::broadcast_msg(std::string msg)
{
	for(auto it = client_sock.begin(); it != client_sock.end(); it++)
	{
		send_msg(it->second->id, msg);
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

void TCPServer::shutdown() 
{
	on_shutdown();
	close(sockfd);
}

