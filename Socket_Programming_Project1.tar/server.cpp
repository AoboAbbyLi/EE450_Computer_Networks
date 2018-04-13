/*
** server.cpp -- The client says "Aobo Li" to the server.
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
#include <sys/wait.h>
#include <signal.h>
#include <iostream>
using namespace std;

#define MAXDATASIZE 100 // max number of bytes we can get at once
#define BACKLOG 5 // the number of connections allowed on the incoming queue

int main(void)
{
	//socket type
	struct addrinfo hints, *hs;
	memset(&hints, 0, sizeof hints);
	hints.ai_family =  AF_UNSPEC;// family type of socket
	hints.ai_socktype = SOCK_STREAM;// socket type tcp protocol
	hints.ai_flags = AI_PASSIVE;//fill in IP

	//getaddinfo NULL means local, TCP port is 2+student id.
	int status;
	if ((status = getaddrinfo(NULL, "25354", &hints, &hs)) != 0) {
    		fprintf(stderr, "server: getaddrinfo: %s\n", gai_strerror(status));
    		return 1;
	}

	//declare sockets 
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_STREAM,0)) == -1) {
	//if ((sockfd = socket(hs->ai_family, hs->ai_socktype, hs->ai_protocol)) == -1) {
    		fprintf(stderr, "server: create socket: %s\n", gai_strerror(sockfd));
    		return 2;
	}

	//if "Address already in use.",  add code to reuse the port.
	int yes = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		fprintf(stderr, "server: failed to setsockopt(%s).\n", gai_strerror(errno));
		return 3;
	}
	// bind the sockect to the port we passed in to getaddrinfo():
	if (bind(sockfd, hs->ai_addr, hs->ai_addrlen) == -1) {
		close(sockfd);
		fprintf(stderr, "server: failed to bind(%s).\n", gai_strerror(errno));
		return 4;
	}

	if (listen(sockfd, BACKLOG) == -1) {
		close(sockfd);
		fprintf(stderr, "serckfd = socket(hs->ai_familyver: failed to listen(%s).\n", gai_strerror(errno));
		return 5;
	}

	//send
	struct sockaddr_storage clt_addr;
	socklen_t addr_size = sizeof(clt_addr);

	//accept
	int new_fd;
	if ((new_fd = accept(sockfd, (struct sockaddr *)&clt_addr, &addr_size)) == -1) {
		close(sockfd); //?
		fprintf(stderr, "server: failed to accept(%s).\n", gai_strerror(errno));
		return 6;
	}

	//receive
	char recv_buf[MAXDATASIZE];
	int numbytes;
	if ((numbytes = recv(new_fd , recv_buf, sizeof(recv_buf)-1, 0)) == -1) {
		//close(new_fd); //
		fprintf(stderr, "server: failed to receive(%s).\n", gai_strerror(errno));
		return 7;
	}

	//print the received message.
	recv_buf[numbytes] = '\0';
	cout<<recv_buf<<endl;

	const char *msg = "OK!";

	if (send(new_fd, msg, strlen(msg), 0) == -1) {
		close(new_fd); //
		fprintf(stderr, "server: failed to send(%s).\n", gai_strerror(errno));
		return 8;
	}

	close(new_fd); //close the new socket

	close(sockfd); //close the socket

	freeaddrinfo(hs); //free the linked list

	return 0;

}




