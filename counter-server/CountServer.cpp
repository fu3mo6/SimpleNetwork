#include <iostream>
#include <csignal>
#include <ctime>
#include "CountServer.h"

bool isPrime(int n)
{
	if(n <= 1)
		return false;
	int i = 2;
	for(; i*i<=n; i++)
	{
		if(n%i==0)
			return false;
	}
	return true;
}

void CountServer::on_accept(int id)
{
	cout << "Id [ " << id << " ] Connection accepted" << endl;
}

void CountServer::on_recv(int id, std::string msg)
{
    int count = stoi(msg);
    handle_count(id, count);
    cout << "Recv [ " << id << " ] [ " << msg << " ] [ " << state << " ]" << endl;
}

void CountServer::on_disconnect(int id)
{
    cout << "Id [ " << id << " ] Disconnected" << endl;
    if(state == id)
    {
	    {
            std::lock_guard<std::mutex> guard(mt);
            state = -1;
            count_timeout = -1;
        }
        broadcast_msg("active");
    }
}

void CountServer::handle_count(int id, int count)
{
	std::string msg = "";
	std::string bcast = "";

    {
	    std::lock_guard<std::mutex> guard(mt);
        if (state < 0)
        {
            msg = "ack";
            if(isPrime(count))
            {
                state = id;
                count_timeout = count + 5;
            }
        }
        else if (state == id)
        {
            msg = "ack";
            if(count == count_timeout)
            {
                state = -1;
                count_timeout = -1;
                bcast = "active";
            }
        }
        else
        {
            msg = "inactive";
        }
    }

	send_msg(id, msg);
    if(bcast != "")
        broadcast_msg(bcast);
}
