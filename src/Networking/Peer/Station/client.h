/*
 *	client.h
 *
 *	Front-end implementation for the station's client. It will spread text
 *	messages to other stations.
 *
 *	Andrei-Sebastian Lungu
 */
#pragma once

#include "../../../config.h"

// Main structure for creating a UDP client used in disseminating text messages to other stations.
struct Station_Client 
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

struct Station_Client station_client_constructor(const char *server_ip, const int port); 

/* client.h */

