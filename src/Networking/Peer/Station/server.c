/*
 *      server.c
 *      Back-end implementation for the node's concurrent TCP server.
 */
#include "server.h"
 
// constructor for the node's only server.
struct Station_Server station_server_constructor()
{
        // Fill up the node's server structure.
        struct Station_Server server;
        server.domain    = SERV_DOMAIN; // domain family
        server.type      = SOCK_DGRAM; // TCP in this case.
        server.protocol  = SERV_PROTOCOL; // 0
        server.interface = ANY_INTERFACE;  // 0xffffffff
        server.port      = START_PORT; // 2029 
        // Create socket and get the descriptor.
        server.socket    = socket(server.domain, server.type, server.protocol);
        if (server.socket == -1) {
                perror("[server] Error occurred when creating a socket\n");
                exit(1);
        }
        // Build server's address.
        memset(&server.address, 0, sizeof(server.address));
        server.address.sin_family      = server.domain;
        server.address.sin_port        = server.port;
        server.address.sin_addr.s_addr = server.interface; // any
        // Bind socket to network.
        while(bind(server.socket, (struct sockaddr *)&server.address, sizeof(struct sockaddr)) == -1) {
                server.address.sin_port++;
                server.port++;
        }
        return server;
}
 
/* server.c */

