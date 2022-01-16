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
#include <unistd.h>	// file closing and creating child processes.
#include <pthread.h>    // thread creation and manipulation.
#include <signal.h>	// signal for treating zombie processes.
#include <sys/wait.h>	// wait signal for child processes.
#include <time.h> 	// counting process time.


/* ------ Macros ------ */

/* Server */ 
#define ANY_INTERFACE htonl(INADDR_ANY)

#define SERV_DOMAIN AF_INET

#define NODE_SERVICE SOCK_STREAM

#define STATION_SERVICE SOCK_DGRAM

#define SERV_PROTOCOL 0

#define BACKLOG 10

#define START_PORT htons(2029)

#define STATIONS 5

