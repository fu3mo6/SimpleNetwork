#include <iostream>
#include <csignal>
#include <ctime>
#include <mutex>
#include <thread>

#include "TCPClient.h"

class CountClient : public TCPClient
{
public:
    CountClient(){
        counter = 0;
        server_state = true;
    }

	void on_connect() override;
	void on_recv(std::string msg) override;
	void on_disconnect() override;

private:
    int counter;
    bool server_state;
    void count_loop();
	std::mutex mt;
    std::thread count_thread;
};