/*
 *      client.c
 *
 *      Back-end implementation for the node's TCP client.
 *      It's job is to send request to other nodes, usually in order to pair with them.
 * 
 *      Andrei-Sebastian Lungu
 */
#include "client.h"

// client's node constructor, will return the client to the caller.
struct Station_Client station_client_constructor(const char *server_ip,const int port) 
{
        // Fill up client structure.
        struct Station_Client client;
        memset(&client, 0, sizeof(struct Station_Client));
        client.domain       = SERV_DOMAIN; // IPv4
        client.type         = SOCK_DGRAM; // SOCK_STREAM
        client.protocol     = SERV_PROTOCOL; // 0
        client.interface    = inet_addr(server_ip); // server ip address
        client.port         = htons(port);
        // Ask for a socket descriptor.
        if ((client.socket = socket(client.domain, client.type, client.protocol)) == -1) {
                perror("[node_client] error occurred when creating a socket");
                exit(1);
        }
        // Fill up destination address structure.
        client.address.sin_family      = client.domain;
        client.address.sin_port        = client.port;
        client.address.sin_addr.s_addr = client.interface;
        return client;
}

/* client.c */

