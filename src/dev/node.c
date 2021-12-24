#include "../Networking/config.h"
#include "../Networking/Node/server.h"

// runServer will get an instance of the server, accept incoming
// requests from clients and pair with other nodes
static void runServer()
{
	struct Node_Server server = node_server_constructor();		

	// client incoming data.
	struct sockaddr incoming;
	socklen_t length = sizeof(incoming);	
	memset(&incoming, 0, length);
	
	// server working loop.
	int isRunning = 1;
	while (isRunning) {
		// accept incoming requests from potential clients.
		printf("Here\n");
		int client = accept(server.socket, &incoming, &length);
		if (client == -1) {
			fprintf(stderr, "Accept error: %s\n", strerror(errno));
			exit(1);
		}
		printf("New client accepted\n");
		if (close(client) == -1) {
			fprintf(stderr, "Close error: %s\n", strerror(errno));
			exit(1);
		}
	}

	if (close(server.socket) == -1) {
		fprintf(stderr, "Close error: %s\n", strerror(errno));
		exit(1);
	}
	return;
}

int main(int arc, char **argv)
{
	// Node's server will run on the main thread.	
	runServer();
	return 0;
}
