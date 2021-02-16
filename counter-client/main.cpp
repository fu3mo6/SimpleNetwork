#include <iostream>
#include <signal.h>
#include <mutex>
#include "CountClient.h"

CountClient client;

void close_app(int s) {
	client.shutdown();
	exit(0);
}

int main(int argc, char **argv)
{
	if(argc != 3) {
		cerr << "Usage: ./client serverip serverport" << endl;
		return 0;
	}

	std::signal(SIGINT, close_app);

	if(client.setup(argv[1],atoi(argv[2])))
		client.loop();
	else
		cout << "Setup failed." << endl;

	return 0;
}