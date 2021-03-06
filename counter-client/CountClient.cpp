#include <iostream>
#include <csignal>
#include <ctime>
#include <mutex>
#include <thread>
#include "CountClient.h"

CountClient::~CountClient()
{
	count_thread.join();
}

void CountClient::count_loop()
{
    std::string msg;
	while(IsConnected()) {
		if(server_state)
		{
            {
                std::lock_guard<std::mutex> guard(mt);
                msg = to_string(counter);
            }

            SendMsg(msg);
            cout << "send " << msg << endl;
			sleep(1);
		}
		else {
			usleep(1000);
		}
	}
}

void CountClient::on_connect()
{
	cout << "Connection established." << endl;
	count_thread = std::thread(&CountClient::count_loop, this);
}

void CountClient::on_recv(std::string msg)
{
	std::lock_guard<std::mutex> guard(mt);
	if(msg.find("ack") != std::string::npos)
		counter++;

	if(msg.find("active") != std::string::npos)
		server_state = true;

	if(msg.find("inactive") != std::string::npos)
		server_state = false;

	if(msg.find("shutdown") != std::string::npos)
		Shutdown();
}

void CountClient::on_disconnect()
{
    cout << "Disconnected." << endl;
}
