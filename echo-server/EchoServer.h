#include <iostream>
#include <csignal>
#include <ctime>
#include "TCPServer.h"

class EchoServer : public TCPServer
{
public:
	void on_accept(int id) override
	{
		cout << "Id [ " << id << " ] Connection accepted" << endl;
	}

	void on_recv(int id, std::string msg) override
	{
		cout << "Recv [ " << id << " ] [ " << msg << " ]" << endl;
		send_msg(id, msg);
	}

	void on_disconnect(int id) override
	{
		cout << "Id [ " << id << " ] Disconnected" << endl;
	}
};