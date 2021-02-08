#include <iostream>
#include <signal.h>
#include "TCPClient.h"

TCPClient tcp;

void sig_exit(int s)
{
	tcp.exit();
	exit(0);
}

int main(int argc, char *argv[])
{
	if(argc != 4) {
		cerr << "Usage: ./client ip port message" << endl;
		return 0;
	}
	signal(SIGINT, sig_exit);

	tcp.setup(argv[1],atoi(argv[2]));

	int counter = 0;
//	bool active_state = True;
	std::string msg;
	while(1)
	{
		msg = "Counter " + to_string(counter);
		counter ++;
		cout << msg << endl;
		tcp.Send(msg);
/*		string rec = tcp.receive();
		if( rec != "" )
		{
			cout << rec << endl;
		} */
		sleep(1);
	}
	return 0;
}
