#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "TCPClient.h"

#define MAX_RECV_SIZE 4096

TCPClient::TCPClient()
{
	sock = -1;
	port = 0;
	address = "";
	connected = false;
}

void TCPClient::loop()
{
	while(connected) {
		std::string rec = this->receive_msg();
		if (rec != "")
		{
			on_recv(rec);
		}
		usleep(1000);
	}
}

#ifdef WINDOWS
bool TCPClient::_setup(std::string address, int port)
{
    WSAData wsaData;
    WSAStartup( MAKEWORD(2,2), &wsaData);

    SOCKADDR_IN addr;
    int addlen = sizeof(addr);
 
    sock = socket(AF_INET, SOCK_STREAM, NULL);
 
    //設定 addr 資料
    addr.sin_addr.s_addr = inet_addr(address.c_str());
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
 
	if (connect(sock, (SOCKADDR*)&addr, sizeof(addr)) < 0)
	{
		perror("connect failed. Error");
		return false;
	}

	return true;
}

#else
bool TCPClient::_setup(std::string address, int port)
{
  	if(sock == -1)
	{
		sock = socket(AF_INET , SOCK_STREAM , 0);
		if (sock == -1){
			cerr << "Failed to open socket" << endl;
			return false;
		}
	}
  	if((signed)inet_addr(address.c_str()) == -1)
  	{
		struct hostent *he;
		struct in_addr **addr_list;
		if ( (he = gethostbyname( address.c_str() ) ) == NULL)
		{
			herror("gethostbyname");
			cerr << "Failed to resolve hostname" << endl;
			return false;
		}
	   	addr_list = (struct in_addr **) he->h_addr_list;
		for(int i = 0; addr_list[i] != NULL; i++)
		{
			server.sin_addr = *addr_list[i];
			break;
		}
  	}
  	else
  	{
		server.sin_addr.s_addr = inet_addr( address.c_str() );
  	}
  	server.sin_family = AF_INET;
  	server.sin_port = htons( port );
  	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
  	{
		perror("connect failed. Error");
		return false;
  	}
	return true;
}
#endif

bool TCPClient::setup(std::string address , int port)
{
	if (_setup(address, port))
	{
		connected = true;
		on_connect();
		return true;
	}
	else 
  		return false;
}

bool TCPClient::send_msg(std::string data)
{
	if(sock != -1) 
	{
		if( send(sock , data.c_str() , strlen( data.c_str() ) , 0) < 0)
		{
			cout << "Send failed : " << data << endl;
			return false;
		}
	}
	else
		return false;
	return true;
}

std::string TCPClient::receive_msg()
{
  	char buffer[MAX_RECV_SIZE];
	memset(&buffer[0], 0, sizeof(buffer));

  	std::string reply;
	int r;

	r = recv(sock , buffer , MAX_RECV_SIZE, 0);
	if(r < 0)
  	{
	    cout << "receive failed!" << endl;
		return "";
  	}
	if(r == 0){
		cout << "Connection closed" << endl;
		connected = 0;
		return "";
	}

	buffer[MAX_RECV_SIZE-1]='\0';
  	reply = buffer;
  	return reply;
}
/*
string TCPClient::read()
{
  	char buffer[1] = {};
  	string reply;
  	while (buffer[0] != '\n') {
    	if( recv(sock , buffer , sizeof(buffer) , 0) < 0)
    	{
      		cout << "receive failed!" << endl;
			return nullptr;
    	}
		reply += buffer[0];
	}
	return reply;
}*/

void TCPClient::do_shutdown()
{
	connected = false;
	on_disconnect();
#ifndef WINDOWS
    close( sock );
#else
	shutdown(sock, SD_SEND);
    closesocket(sock);
    WSACleanup();
#endif
}
