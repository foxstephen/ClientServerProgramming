#include "unp.h"

void sf_read(int);
void sf_write(int);

int main(int argc, char **argv){

	int sockfd = 0;
	struct sockaddr_in servaddr;

	if(argc != 3) {
		err_quit("Usage: a.out <IPAddress>>");
	}

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		err_sys("Socket eroror");
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));

	// Convert dotted IP Address to network byte order.
	if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
		err_quit("inet_pton error for %s", argv[1]);
	}

	if(connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) < 0) {
		err_sys("Connect error");
	}

	sf_read(sockfd);
	sf_write(sockfd);
}

void sf_read(int sockfd) {
	int n = 0;
	char recvline[MAXLINE + 1];
	int counter = 0;

	while((n = read(sockfd, recvline, MAXLINE)) > 0) {
                recvline[n] = 0;

                if(fputs(recvline, stdout) == EOF) {
                        err_sys("fputs erorr");
                }
		break;
        }

	if (n < 0) {
		err_sys("read error.\n");
	}
}


void sf_write(int sockfd) {
	char buff[MAXLINE + 1];
	snprintf(buff, sizeof(buff), "HELO attu.ict.ad.dit.ie\r\n");
        Write(sockfd, buff, strlen(buff));
	sf_read(sockfd);
}
