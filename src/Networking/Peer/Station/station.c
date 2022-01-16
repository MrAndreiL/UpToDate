/*
 *	station.c
 *
 *	Andrei-Sebastian Lungu
 */
#include "station.h"

void *open_server(struct Station *station);

void *shutdown_server(struct Station *station);

void *open_client(struct Station *station, const char *ip, int port, char *text);

void *shutdown_client(struct Station *station);

// Station build-up.
struct Station station_constructor(int number)
{
	struct Station station;
	// Set station number.
	station.number = number;

	// Set station as active initially.
	station.state  = 1;

	/* Server start-up */
	station.server_obj      = station_server_constructor();
	station.open_server     = open_server;
	station.shutdown_server = shutdown_server;
	station.station_list.length = 0;

	/* Client start-up */
	station.open_client     = open_client;
	station.shutdown_client = shutdown_client;
	return station;
}

struct Delievery
{
	struct Station *station;
	struct sockaddr_in *incoming;
	int socket;
};


// Receive connectionless text messages.
void *receive_server(void *arg)
{
	struct Delievery *delievery = (struct Delievery *)arg;

	
	// Receive text message.
	char msg[255];
	int length = sizeof(*delievery->incoming);
	if (recvfrom(delievery->socket, msg, sizeof(msg), 0, (struct sockaddr *)delievery->incoming, &length) == -1) {
		perror("[Station] Error at receiving UDP packets\n");
		exit(1);
	}
	printf("[Station] Received %s\n", msg);
	pthread_exit(NULL);
}

// Server will concurrently receive text messages connectionless.
void *server_run(void *arg) 
{
	struct Station *station = (struct Station *)arg;
	printf("[Station %d] is now active at port %d\n", station->number, ntohs(station->server_obj.port));

	int recv;
	struct sockaddr_in incoming;
	int length = sizeof(incoming);

	struct Delievery delievery;
	delievery.station = station;

		int socket = station->server_obj.socket;

	// Working loop. Create a thread in order to catch UDP datagrams.
	while (station->state) {
		// receive 1 from client
		if (recvfrom(socket, &recv, sizeof(recv), 0, (struct sockaddr *)&incoming, &length) == -1) {
			perror("[Station] Error at receiving UDP packets\n");
			exit(1);
		}
		
		if (recv == 1) { // create new thread to expect text message  
			pthread_t id;
			delievery.incoming = &incoming;
			// Send confirmation bit back to the client.
			int val = 1;
			if (sendto(socket, &val, sizeof(val), 0, (struct sockaddr *)&incoming, length) == -1) {
				perror("[Station client] Error at spreading dataa\n");
				exit(1);
			}
			delievery.socket = socket;
			if (pthread_create(&id, NULL, receive_server, &delievery) != 0) {
				perror("[Station] Error at making thread\n");
				exit(1);
			}
		}
		
	}
	shutdown_server(station);
	pthread_exit(NULL);
}

// Place the server on another thread.
void *open_server(struct Station *station)
{
	if (pthread_create(&station->server_id, NULL, server_run, station) != 0) {
		perror("[Station] Error at making thread\n");
		exit(1);
	}
}

// Server closing and cleanup.
void *shutdown_server(struct Station *station)
{
	if (close(station->server_obj.socket) == -1) {
		perror("[Station server] Error at socket closing\n");
		exit(1);
	}
}

struct Data
{
	struct Station *station;
	struct sockaddr_in address;
	int socket;
	char *text;
};

void func()
{
	printf("DA");
}

// Send data text over the network connectionless.
void *client_run(void *arg) 
{
	struct Data *data = (struct Data *)arg;
	printf("[Station] Spreading data: %s\n", data->text);

	// Send val = 1 and expect to receive the same.
	int val = 1;

	signal(SIGPIPE, func);
	if (sendto(data->socket, (void*)&val, sizeof(val), 0, (struct sockaddr *)&data->address, sizeof(struct sockaddr)) == -1) {
		perror("[Station clinet] Error at spreading data\n");
		exit(1);
	}

	int recv;
	struct sockaddr_in incoming;
	int length = sizeof(struct sockaddr);
	if (recvfrom(data->socket, &recv, sizeof(recv), 0, (struct sockaddr *)&incoming, &length) == -1) {
		perror("[Station] Error at receiving UDP packets\n");
		exit(1);
	}
	
	if (recv == 1) {  
		if (sendto(data->socket, data->text, sizeof(data->text), 0, (struct sockaddr *)&data->address, sizeof(struct sockaddr)) == -1) {
			perror("[Station client] Error at spreading data\n");
			exit(1);
		}
	}
	//data->station->shutdown_client(data->station);
	pthread_exit(NULL);
}


// Client Start-up and placement on separate thread.
void *open_client(struct Station *station, const char *ip, int port, char *text)
{
	// Instantiate the server.
	printf("[Station %d]Sending at port %d\n", station->number, ntohs(port));
	station->client = station_client_constructor(ip, ntohs(port));

	struct Data data;
	data.address = station->client.address;
	data.socket  = station->client.socket;
	data.text    = text;
	data.station = station;
	pthread_t client_t[10000];
	int a = 0;
	if (pthread_create(&client_t[a++], NULL, client_run, (void *)&data) != 0) {
		perror("[Station] Error at making thread\n");
		exit(1);
	}
}

// Client closing and cleanup.
void *shutdown_client(struct Station *station)
{
	if (close(station->client.socket) == -1) {
		perror("[Station server] Error at socket closing\n");
		exit(1);
	}
}
