/*
 * 	peer.c
 *
 * 	Back-end implementation for the peer-to-peer network.
 *
 * 	Andrei-Sebastian Lungu.
 */
#include "peer.h"

void *run_server(struct Peer *peer);	

void *close_server(struct Peer *peer);

void *run_client(struct Peer *peer, const char *ip, int port);

void *close_client(struct Peer *peer);

void *main_thread_loop(struct Peer *peer);

void *activate_stations();

void *plan_spread(struct Peer *peer);

// Peer constructor. Return default config of the peer node.
struct Peer peer_constructor()
{
	struct Peer peer;
	memset(&peer, 0, sizeof(peer));

	/* Build node's server */	
	peer.server 	   = node_server_constructor();  	
	peer.serverRunning = 0;
	peer.run_server    = run_server;
	peer.close_server  = close_server;
	peer.K		   = 3;
	
	peer.run_client    = run_client;
	peer.close_client  = close_client;
	peer.clientRunning = 0;
	peer.plan_spread = plan_spread;

	peer.peerRunning  = 1;
	peer.main_thread_loop = main_thread_loop;

	// Instantiate the linked list.
	peer.station_list = linked_list_constructor();

	// Station management.
	peer.activate_stations = activate_stations;
	return peer;
}

// Template structure for sending station info in order to realise the pairing.
struct Station_Info
{
	char Ip[20];
	int port;
};

struct thread_comms
{
	struct Peer *peer;
	int client;
};


void *spread(void *arg)
{
	struct Peer *peer = (struct Peer *)arg;
	while (1) {
		if (peer->station_list.length >= 15) { 
			printf("More than 15\n");
			for (int i = 0; i < peer->K; i++) {
				int ch = 1;
				while (ch < 3) {
					int r = rand() % peer->station_list.length; 
					void *d = peer->station_list.retrieve(&peer->station_list, r);
					struct Station_Info *data = (struct Station_Info *)d;
					peer->stations[i].open_client(&peer->stations[i], data->Ip, data->port, "Andrei");
					ch++;
				}
			}
			sleep(5);
		}
	}	
	pthread_exit(NULL);
}

void *plan_spread(struct Peer *peer)
{
	pthread_t id;
	if (pthread_create(&id, NULL, spread, peer) != 0) {
		perror("Error at creating thread\n");
		exit(1);
	}
}


// Iterate over each station to call their constructor and servers.
void *activate_stations(struct Peer *peer)
{
	for(int i = 0; i < STATIONS; i++) {
		peer->stations[i] = station_constructor(i);	
		peer->stations[i].open_server(&peer->stations[i]);
	}
}

// Maintain an infinite loop in order to prevent main thread exiting before 
// child threads.
void *main_thread_loop(struct Peer *peer)
{
	char IP[15];
	int port;
	while(peer->peerRunning) {
		scanf("%s", IP);		
		scanf("%d", &port);
		peer->run_client(peer, IP, port);
	}
}



void updateStations(struct Peer *peer) 
{
	// Iterate over each station and update their linked list.
	for (int i = 0; i < STATIONS; i++)
		peer->stations[i].station_list = peer->station_list;
}

void *thread_recv(void *arg)
{
	struct thread_comms *val = (struct thread_comms *)arg;

	// client incoming data.
	struct sockaddr_in incoming;
	int length = sizeof(incoming);

	// Prepare the 5 station info.
	struct Station_Info package[100];
	for (int i = 0; i < STATIONS; i++) {
		package[i].port = val->peer->stations[i].server_obj.port;		
		memcpy(&package[i].Ip, "127.0.0.1", 10);
		val->peer->station_list.insert(&val->peer->station_list, val->peer->station_list.length,&package[i], sizeof(package[i]));
	}

	// 1. Send pairing response after connection.
	if (sendto(val->client, &package, sizeof(package), 0, (struct sockaddr*)&incoming, length) == -1) {
		perror("[node_server]: Error when sending message\n");
		exit(1);
	}

	// 2. Receive pairing response in order to achieve peer-to-peer status.
	memset(package, 0, sizeof(package));
	if (recvfrom(val->client, &package, sizeof(package), 0, (struct sockaddr *)&incoming, &length) == -1) {
		perror("[node_client]: Pairing error\n");
		exit(1);
	}
	// 3. Append every elemenent to the list.
	for (int i = 0; i < STATIONS; i++) 
		val->peer->station_list.insert(&val->peer->station_list, val->peer->station_list.length,&package[i], sizeof(package[i]));
	updateStations(val->peer);
	printf("Connected with a new client\n");
	printf("Length: %d", val->peer->station_list.length);
	//for (int i = 0; i < val->peer->station_list.length; i++) {
	//	void *data = val->peer->station_list.retrieve(&val->peer->station_list, i);
	//	struct Station_Info *new_data = (struct Station_Info *)data;
	//	printf("%d\n", new_data->port);
	//}
	close(val->client);
	pthread_exit(NULL);	
}

// Allow the server to continuously accept requests from clients
// while serverRunning = 1.
void *server(void *arg) 
{
	struct Peer *peer = (struct Peer*)arg;
	printf("[node_server]Node server is running on IP 127.0.0.1 and port %d\n", ntohs(peer->server.port));

	// client incoming data.
	struct sockaddr_in incoming;
	int length = sizeof(incoming);
	
	// Server working loop.
	while (peer->serverRunning) {
		// 1. Accept pairing request.
		int client = accept(peer->server.socket, (struct sockaddr*)&incoming, &length);
		if (client == -1) {
			perror("[node_server]: Connection error\n");
			exit(1);
		}
		struct thread_comms comms;
		comms.peer = peer;
		comms.client = client;
		pthread_t id;
		if (pthread_create(&id, NULL, thread_recv, &comms) != 0) {
			perror("[node_server_thread] error\n");
			exit(1);
		}
	}
	pthread_exit(NULL);
}

// Run the server on a separate thread.
void *run_server(struct Peer *peer)
{
	peer->serverRunning = 1;
	pthread_t serverId;	
	if (pthread_create(&serverId, NULL, server, peer) != 0) {
		perror("[node_server_thread] error\n");
		exit(1);
	}
}

// Server closing cleanup.
void *close_server(struct Peer *peer) 
{
	peer->serverRunning = 0;
	if (close(peer->server.socket) == -1) {
		perror("Error when closing the server\n");
		exit(1);
	}
}

// Send pairing request to another node.
// Make sure the nodes are not already paired.
void *client(void *arg)
{
	struct Peer *peer = (struct Peer*)arg;
	// pairing information
	struct Station_Info package[STATIONS];
	
	// 1. Receive pairing confirmation. 
	int length = sizeof(peer->client.address);
	if (recvfrom(peer->client.socket, &package, sizeof(package), 0, (struct sockaddr *)&peer->client.address, &length) == -1) {
		perror("[node_client]: Pairing error\n");
		exit(1);
	}

	// 2. Append every elemenent to the list.
	for (int i = 0; i < STATIONS; i++) 
		peer->station_list.insert(&peer->station_list, peer->station_list.length,&package[i], sizeof(package[i]));

	// 3. Send own info list.
	memset(package, 0, sizeof(package));
	for (int i = 0; i < STATIONS; i++) {
		package[i].port = peer->stations[i].server_obj.port;		
		memcpy(&package[i].Ip, "127.0.0.1", 10);
	}
	if (sendto(peer->client.socket, &package, sizeof(package), 0, (struct sockaddr*)&peer->client.address, length) == -1) {
			perror("[node_server]: Error when sending message\n");
			exit(1);
	}
	pthread_exit(NULL);
}

// Client start-up routine. Place the client
// on another thread.
void *run_client(struct Peer *peer, const char *ip, int port) 
{
	// Instantiate client structure.
	peer->client = node_client_constructor(ip, port);

	peer->clientRunning = 1;
	pthread_t clientId;
	if (pthread_create(&clientId, NULL, client, peer) != 0) {
		perror("[node_client_thread]: error\n");
		exit(1);
	}
}

// Client clean-up routine.
void *close_client(struct Peer *peer) 
{
	peer->clientRunning = 0;
	if (close(peer->client.socket) == -1) {
		perror("[node_client]: Error when closing client socket\n");
		exit(1);
	}
}
