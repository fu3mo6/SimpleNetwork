#include <iostream>
#include <csignal>
#include <ctime>
#include "CountServer.h"

CountServer tcp;

void close_app(int s) {
	tcp.Shutdown();
	exit(0);
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cerr << "Usage: ./server port" << endl;
		return 0;
	}

	std::signal(SIGINT, close_app);

	if( tcp.Setup(atoi(argv[1])) == 0) {
		tcp.Loop();
	}
	else {
		cerr << "Error setup server" << endl;
	}

	return 0;
}
