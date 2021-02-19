#include <iostream>
#include <csignal>
#include <ctime>
#include "EchoServer.h"

EchoServer tcp;

void close_app(int s) {
	tcp.do_shutdown();
	exit(0);
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		cerr << "Usage: ./server port" << endl;
		return 0;
	}

	std::signal(SIGINT, close_app);

	if( tcp.setup(atoi(argv[1])) == 0) {
		tcp.loop();
	}
	else {
		cerr << "Error setup server" << endl;
	}

	return 0;
}
