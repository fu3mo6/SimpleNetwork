#include <iostream>
#include <csignal>
#include <ctime>
#include "TCPServer.h"

class CountServer : public TCPServer
{
public:
	void on_accept(int id) override;
	void on_recv(int id, std::string msg) override;
	void on_disconnect(int id) override;

private:
    void handle_count(int id, int count);
	int state, count_timeout;
	std::mutex mt;
};