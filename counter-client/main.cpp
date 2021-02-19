#include <iostream>
#include <signal.h>
#include <mutex>
#include "CountClient.h"

CountClient client;

void close_app(int s) {
	client.Shutdown();
	exit(0);
}

int main(int argc, char **argv)
{
	if(argc != 3) {
		cerr << "Usage: ./client serverip serverport" << endl;
		return 0;
	}

	std::signal(SIGINT, close_app);

	if(client.Setup(argv[1],atoi(argv[2])))
		client.Loop();
	else
		cout << "Setup failed." << endl;

	return 0;
}