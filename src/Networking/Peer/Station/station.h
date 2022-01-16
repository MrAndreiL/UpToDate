/*
 *	station.h
 *
 *	Main station structure blueprint. It should contain a server strcture and a client structure,
 *	methods to initialize these structures and sending messages across the network.
 *
 *	Andrei-Sebastian Lungu
 */
#pragma once

#include "server.h"
#include "client.h"
#include "../../../DataStructures/Lists/linkedlist.h"

struct Station
{
	int state; // 0 <- station is off; 1 <- station is on (default)

	int number; // station number [1..5]

	/* Server structure and methods */
	struct Station_Server server_obj;

	// server thread id to easily manage the thread.
	pthread_t server_id; 

	// place server on another thread and run it's working loop. 
	void* (*open_server)(struct Station *station);

	// clean potentially leaking resources.
	void* (*shutdown_server)(struct Station *station);

	/* Client structure and methods */
	struct Station_Client client; 

	// LinkedListr structure.
	struct LinkedList station_list;

	// client thread id.
	pthread_t client_id;

	// send message across the network on separate thread.
	void *(*open_client)(struct Station *station, const char *ip, int port,char *text);

	void* (*shutdown_client)(struct Station *station);
};

struct Station station_constructor(int number);
