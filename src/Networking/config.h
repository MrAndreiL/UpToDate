/*
 *	Config file. 
 *	All standard or external libraries and macros used are declared here.
 *	Andrei-Sebastian Lungu
 */
#pragma once

/* ----- Standard libraries ----- */
#include <stdio.h> 	// standard input and output
#include <sys/types.h>  // portability for sys/socket
#include <sys/socket.h> // socket functionality
#include <netinet/in.h> // addresses and binding macros
#include <arpa/inet.h>  // byte order conversion functions
#include <errno.h>	// error messages
#include <string.h>	// string comparing and error formatting
#include <stdlib.h>     // exit
#include <unistd.h>	// file closing


/* ------ Macros ------ */

/* Server */ 
#define ANY_INTERFACE htonl(INADDR_ANY)

#define SERV_DOMAIN AF_INET

#define NODE_SERVICE SOCK_STREAM

#define STATION_SERVICE SOCK_DGRAM

#define SERV_PROTOCOL 0

#define BACKLOG 10

#define NODE_PORT htons(2024)

#define STATION_1_PORT htons(2025)

#define STATION_2_PORT htons(2026)

#define STATION_3_PORT htons(2027)

#define STATION_4_PORT htons(2028)

#define STATION_5_PORT htons(2029)
