/*
 *	server.h
 *	
 *	Front-end used in creating client/server pairs TCP/IP for pairing with other nodes  
 * 	Andrei-Sebastian Lungu
 */
#include "../config.h"

// Main structure for creating a TCP server used in pairing and connecting two nodes.
struct Node_Server 
{
	// internet domain family (IPv4 or IPv6)
	int domain;
	// service type (SOCK_STREAM or SOCK_DGRAM)
	int type;
	// any additional protocol.
	int protocol;
	// interface - pair to all if server.
	int interface;
	// port
	int port;
	// socket descriptor.
	int socket;
	// address.
	struct sockaddr_in address;
};

struct Node_Server node_server_constructor();

/* server.h */
