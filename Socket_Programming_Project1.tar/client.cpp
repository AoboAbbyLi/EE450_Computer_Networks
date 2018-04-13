/*
** client.cpp -- The server responds with “Ok!”  information after receiving name from the client.
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <iostream>
using namespace std;

#define MAXDATASIZE 100 // max number of bytes we can get at once

int main(void)
{
	struct addrinfo hints, *cl;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC; // family type of socket: AF_INET or AF_INET6
	hints.ai_socktype = SOCK_STREAM; //socket type tcp protocol
	hints.ai_flags = AI_PASSIVE;//fill in IP

	int status; //handle error
	if ((status = getaddrinfo(NULL, "25354", &hints, &cl)) != 0) {
		fprintf(stderr, "client: getaddrinfo: %s\n", gai_strerror(status));
		return 1;
	}

	//make a socket
	int cl_skt;
	if ((cl_skt = socket(cl->ai_family, cl->ai_socktype, cl->ai_protocol)) == -1) {
		fprintf(stderr, "client: failed to create socket(%s).\n", gai_strerror(errno));
		return 2;
	}

	//connect
	if (connect(cl_skt, cl->ai_addr, cl->ai_addrlen) == -1) {
		fprintf(stderr, "client: failed to connect(%s).\n", gai_strerror(errno));
		return 3;
	}

	//send
	const char *msg = "Aobo Li";
	if (send(cl_skt, msg, strlen(msg), 0) == -1) {
		fprintf(stderr, "client: failed to send(%s).\n", gai_strerror(errno));
		return 4;
	}
	
	//receive
	char recv_buf[MAXDATASIZE];
	int numbytes;
	if((numbytes = recv(cl_skt , recv_buf, sizeof(recv_buf)-1, 0)) == -1) {
		fprintf(stderr, "client: failed to receive(%s).\n", gai_strerror(status));
		return 5;
	}

	//print the received message.
	recv_buf[numbytes] = '\0';
	cout<<recv_buf<<endl;

	close(cl_skt); //close the client socket

	freeaddrinfo(cl); // free the linked list

	return 0;

}

