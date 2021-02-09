#include <iostream>
#include <csignal>
#include <ctime>
#include "TCPServer.h"

TCPServer tcp;
int state = -1;
int count_limit = -1;
std::mutex mt;

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

void close_app(int s) {
	tcp.closed();
	exit(0);
}

std::string handle_count(int id, int count)
{
	std::lock_guard<std::mutex> guard(mt);
	std::string msg = "";
	std::string broadcast = "";

	if (state < 0)
	{
		msg = "ack";
		tcp.Send(msg, id);
		if(isPrime(count))
		{
			state = id;
			count_limit = count * 2;
		}
	}
	else if (state == id)
	{
		msg = "ack";
		tcp.Send(msg, id);
		if(count == count_limit)
		{
			state = -1;
			count_limit = -1;
			broadcast = "active";
		}
	}
	else
	{
		msg = "inactive";
		tcp.Send(msg, id);
	}
	return broadcast;
}

void * received(void * m)
{
    pthread_detach(pthread_self());
	cout << "start new thread: received" << endl;
	std::string msg;
	vector<descript_socket*> desc;
	while(1)
	{
		desc = tcp.getMessage();
		for(unsigned int i = 0; i < desc.size(); i++) {
			if( desc[i]->message != "")
			{
				int count = stoi(desc[i]->message);

				cout << desc[i]->id 
					 << " count " << count 
					 << " state " << state
					 << " limit " << count_limit
					 << endl;

				msg = handle_count(desc[i]->id, count);
				if ( msg != ""){
					cout << "Broadcast "<< msg<<endl;
					tcp.Broadcast(msg);
				}
				tcp.clean(i);
			}
		}
		usleep(1000);
	}
	cout << "End thread: received" << endl;
	return 0;
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cerr << "Usage: ./server port" << endl;
		return 0;
	}
	std::signal(SIGINT, close_app);

	pthread_t msg;
	vector<int> opts = { SO_REUSEPORT, SO_REUSEADDR };

	if( tcp.setup(atoi(argv[1]),opts) == 0) {
		if( pthread_create(&msg, NULL, received, (void *)0) == 0)
		{
			while(1) {
				tcp.accepted();
				cerr << "Accepted" << endl;
			}
		}
	}
	else
		cerr << "Errore apertura socket" << endl;
	return 0;
}
