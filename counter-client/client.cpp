#include <iostream>
#include <signal.h>
#include <mutex>
#include "TCPClient.h"

TCPClient tcp;
bool active_state = true;
int counter = 0;
std::mutex mt;

void sig_exit(int s)
{
	tcp.exit();
	exit(0);
}

void handle_send()
{
	std::lock_guard<std::mutex> guard(mt);
	std::string msg = to_string(counter);
	cout << "send " << msg << endl;
	tcp.Send(msg);
}

void handle_recv(std::string rec)
{
	std::lock_guard<std::mutex> guard(mt);
	if(rec.find("ack") != std::string::npos)
		counter++;

	if(rec.find("active") != std::string::npos)
		active_state = true;

	if(rec.find("inactive") != std::string::npos)
		active_state = false;
}

void * send_count(void * m)
{
	while(1) {
		if(active_state)
		{
			handle_send();
			sleep(1);
		}
		else {
			usleep(1000);
		}
	}
	pthread_exit(NULL);
	return 0;
}

int main(int argc, char *argv[])
{
	if(argc != 4) {
		cerr << "Usage: ./client ip port message" << endl;
		return 0;
	}
	signal(SIGINT, sig_exit);

	tcp.setup(argv[1],atoi(argv[2]));

	pthread_t thread;
	while(1)
	{
		if( pthread_create(&thread, NULL, send_count, (void *)0) == 0)
		{
			while(1) {
				std::string rec = tcp.receive();
				if (rec != "")
				{
					cout<<"recv "<<rec<<endl;
					handle_recv(rec);
				}
				usleep(1000);
			}
		}
	}
	return 0;
}
