
// Name: Stephen Fox
// Student no: C13475462.


#include "unp.h"
#include <stdio.h>

#define USERNAME "admin"
#define PASSWORD "pass"

void cli_ip(struct sockaddr_in, char*);

struct User {
  char username[30];
  char password[30];
};


int main(int argc, char **argv) {
  int listenfd;
  int connfd;
  int n;
  socklen_t len;

  struct sockaddr_in servaddr;
  struct sockaddr_in cliaddr; // The clients address.
  struct User user;

  char buff[MAXLINE];
  char wbuff[MAXLINE];

  char *tempu;
  char *tempp;

  if (argc != 2) {
    err_quit("Port not specified.\n");
  }

  listenfd = Socket(AF_INET, SOCK_STREAM, 0);

  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family =  AF_INET;
  servaddr.sin_port = htons(atoi(argv[1]));
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

  Listen(listenfd, LISTENQ);

  for ( ; ; ) {

    len = sizeof(cliaddr); 
    connfd = Accept(listenfd, (SA *) &cliaddr, &len);

    while ((n = read(connfd, buff, MAXLINE)) > 0) {
        buff[n] = 0;
        if (fputs(buff, stdout) == EOF) {
            printf("There was a read error.\n");
        }
        tempu = strstr(buff, "username");
        tempp = strstr(buff, "password");

	if (tempu != NULL && tempp != NULL) {
	  break;
	}
    }

    sscanf(buff, "username: %s password: %s", user.username, user.password);

    if ((strcmp(user.password, PASSWORD) == 0) &&
       (strcmp(user.username, USERNAME) == 0)) {
	      char ip[INET_ADDRSTRLEN];
        cli_ip(cliaddr, ip);
        snprintf(wbuff, sizeof(wbuff), "PROCEED, your ip address is: %s\r\n", ip);
        Write(connfd, wbuff, sizeof(wbuff));
        close(connfd);
    }
    else {
       char *denied = "DENIED";
       printf("Denied\n");
       Write(connfd, denied, strlen(denied));
       close(connfd);
    }
  }
}


void cli_ip(struct sockaddr_in cliaddr, char* dest) {
  inet_ntop(AF_INET, &cliaddr.sin_addr, dest, INET_ADDRSTRLEN);
}
