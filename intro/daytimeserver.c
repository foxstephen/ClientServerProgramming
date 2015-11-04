#include "unp.h"
#include <time.h>

int main(int argc, char **argv) {

	int listenfd, connfd; // Socket Ids; one for listening and one for the connected socket.
	struct sockaddr_in servaddr; // Address structure to hold this server's address.
	char buff[MAXLINE];
	time_t ticks; // Require to calculate date and time.

	if (argc != 2) {
		err_quit("Usage: <Program Name><Port No.>\n");
	}


	// Create a socket.
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr)); // Zero and fill in server address structure.
	servaddr.sin_family 		= AF_INET;
	servaddr.sin_addr.s_addr 	= htonl(INADDR_ANY); // Connect to any local ip address.
	servaddr.sin_port 		= htons(atoi(argv[1])); // Daytime server port number;

	// Connects the socket to an external interface.
	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	// Changes the socket to a " Passive listening" socket
	Listen(listenfd, LISTENQ);

	for ( ; ; ) {
		// Accept next connection request.
		connfd = Accept(listenfd, (SA *) NULL, NULL);

		ticks = time(NULL);

		// Construct the  data to return to the client.
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));

		// Write data to the client.
		Write(connfd, buff, strlen(buff));
	
		Close(connfd);
	}
}
// Note the use of the upper-case letters at the start of the socket primitives names.
// These are wrapper function that include error-checking functionality for each call.
// The actual socket primitives use lower case names.
