/*
 *	client.h
 *
 *	Front-end used in creating client/server pairs TCP/IP for pairing with other nodes  
 * 	Andrei-Sebastian Lungu
 */
#pragma once

#include "../../../config.h"

// Main structure for creating a TCP client used in pairing and connecting two nodes.
struct Node_Client 
{
	// internet domain family (IPv4 or IPv6)
	int domain;
	// service type (SOCK_STREAM or SOCK_DGRAM)
	int type;
	// any additional protocol.
	int protocol;
	// ip address of partner server.
	unsigned int interface;
	// port
	int port;
	// socket descriptor.
	int socket;
	// address.
	struct sockaddr_in address;
};

struct Node_Client node_client_constructor(const char *ip, const int port);

/* client.h */
