#include <iostream>
#include <csignal>
#include <ctime>
#include "CountServer.h"

CountServer tcp;

void close_app(int s) {
	tcp.shutdown();
	exit(0);
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cerr << "Usage: ./server port" << endl;
		return 0;
	}

	std::signal(SIGINT, close_app);
    vector<int> opts = { SO_REUSEPORT, SO_REUSEADDR };

	if( tcp.setup(atoi(argv[1]),opts) == 0) {
		tcp.loop();
	}
	else {
		cerr << "Error setup server" << endl;
	}

	return 0;
}
