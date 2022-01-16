/*
 * 	peer.h
 *	Front-end implementation for the peer-to-peer network.
 *	Here the underlying structure of a peer node is detailed.
 *	Andrei-Sebastian Lungu.
 */
#pragma once

#include "../../config.h"
#include "./Node/server.h"
#include "./Node/client.h"
#include "./Station/station.h"
#include "../../DataStructures/Lists/linkedlist.h"

// Peer structure. It will host pointers to it's network ready server and client
// and methods for basic communication and pairing between peers.
struct Peer
{
	/* Server structure and methods. */
	struct Node_Server server;

	int serverRunning; // 0 -> not running ; 1 -> is running (default)

	void *(*run_server)(struct Peer *peer); // run server on separate thread.

	void *(*close_server)(struct Peer *peer);
	
	void *(*plan_spread)(struct Peer *peer);

	int K;


	/* Client structure and methods. */
	struct Node_Client client;

	int clientRunning; // 0 -> not running (default); 1 -> is running

	// Open client in order to interact with other nodes.
	void *(*run_client)(struct Peer *peer, const char *ip, int port);

	void *(*close_client)(struct Peer *peer);

	// LinkedList used in network pairing.
	struct LinkedList station_list;
	
	// Main thread loop.
	int peerRunning; // 0 -> not running; 1-> is running (default)
	void *(*main_thread_loop)();

	/* Stations */
	struct Station stations[STATIONS];

	// Activate every station. (their servers specifically)
	void *(*activate_stations)(struct Peer *peer);
};

struct Peer peer_constructor();

/* peer.c */
