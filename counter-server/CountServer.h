#include <iostream>
#include <csignal>
#include <ctime>
#include "TCPServer.h"

class CountServer : public TCPServer
{
public:
    CountServer()
    {
        state = -1;
        count_timeout = -1;
    }

	void on_accept(int id) override;
	void on_recv(int id, std::string msg) override;
	void on_disconnect(int id) override;
    void on_shutdown() override;

    int get_state() { return state; }
    int get_timeout() { return count_timeout; }

private:
    bool handle_count(int id, int count);
	int state, count_timeout;
	std::mutex mt;
};