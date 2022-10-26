/*
 * echoserveri.c - An iterative echo server
 */
/* $begin echoserverimain */
#include "csapp.h"

void echo(int connfd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;

    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //line:netp:echo:
      printf("server received %d bytes\n", (int)n);

      if ((int)n == 5 && !strcmp(buf, "ping\n")) { // check size and value.
        Rio_writen(connfd, "pong\n", 5);
      } else {
	Rio_writen(connfd, buf, n);
      }	      
    }
}

void store(int connfd) {
  size_t n;
  char buf[MAXLINE];
  char db [MAXLINE];
  rio_t rio;
  
  Rio_readinitb(&rio, connfd);

  while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) { //line:netp:echo:
    printf("server received %d bytes\n", (int)n);

    if (!strncmp(buf, "PUT", 3)) {
      strcpy(db, &buf[4]);
      Rio_writen(connfd, "Wrote in string.\n", 17);
    } else if (!strncmp(buf, "GET\n", 4)) {
      Rio_writen(connfd, db, strlen(db));
    } else {
      Rio_writen(connfd, "Not a valid request\n", 20);
    }
  } 
}


int main(int argc, char **argv)
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;  /* Enough space for any address */  //line:netp:echoserveri:sockaddrstorage
    char client_hostname[MAXLINE], client_port[MAXLINE];

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listenfd = Open_listenfd(argv[1]);
    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE,
                   client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);

	//echo(connfd);
	store(connfd);

	Close(connfd);
    }
    exit(0);
}
/* $end echoserverimain */
