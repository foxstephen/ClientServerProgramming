#include "unp.h"


int main(int argc, char **argv){

	int sockfd, n, counter = 0;
	char recvline[MAXLINE + 1];
	struct sockaddr_in servaddr;

	if(argc != 3) {
		err_quit("Usage: a.out <IPAddress>>");
	}

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		err_sys("Socket eroror");
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;

	// Daytime server port taken from the command line.
	servaddr.sin_port = htons(atoi(argv[2])); 

	// Convert dotted IP Address to network byte order.
	if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
		err_quit("inet_pton error for %s", argv[1]);
	}

	if(connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0) {
		err_sys("Connect error");
	}

	while((n = read(sockfd, recvline, MAXLINE)) > 0) {
		counter++; // Count the number of loops completed.
		recvline[n] = 0; // Null terminate.

		// Print data (time and date.)
		if(fputs(recvline, stdout) == EOF) {
			err_sys("fputs erorr");
		}
	}

	if(n < 0) {
		err_sys("read error");
	}

	printf("counter = %d\n", counter);
	exit(0);

}
