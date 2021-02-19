#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>

#ifndef WINDOWS
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "WS2_32.lib")
#endif

#include <vector>

using namespace std;
class TCPClient
{
public:
    TCPClient();
    bool Setup(std::string address, int port);
    void Loop();

    virtual void on_connect() {}
    virtual void on_recv(std::string msg) {}
    virtual void on_disconnect() {}

    virtual bool SendMsg(std::string msg);
    virtual std::string RecvMsg();
    void Shutdown();

    bool IsConnected() { return connected; }

private:
#ifndef WINDOWS
    int sock;
    struct sockaddr_in server;
#else
    SOCKET sock;
#endif
    std::string address;
    int port;
    bool connected;

    virtual bool _setup(std::string address, int port);
};

#endif
