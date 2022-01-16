/*
 *	server.h
 *
 *	Front-end implementation for the station's server. It will accept UDP requests
 *	and will initiate the client if necesessary.
 *	
 *	Andrei-Sebastian Lungu
 */
#pragma once

#include "../../../config.h"

// Main structure for creating a TCP server used in pairing and connecting two nodes.
struct Station_Server 
{
        // internet domain family (IPv4 or IPv6)
        int domain;
        // service type (SOCK_STREAM or SOCK_DGRAM)
        int type;
        // any additional protocol.
        int protocol;
        // interface - pair to all if server.
        unsigned int interface;
        // port
        int port;
        // socket descriptor.
        int socket;
        // address.
        struct sockaddr_in address;
};

struct Station_Server station_server_constructor();
/* server.h */

