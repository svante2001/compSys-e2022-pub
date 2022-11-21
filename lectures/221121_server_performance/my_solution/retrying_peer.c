#include <stdio.h>
#include <pthread.h>
#include "csapp.h"
#include <time.h>

pthread_t client_thread;
pthread_t server_thread;

char* hostname = "0.0.0.0";
char* port = "12346";

void* client() {
  rio_t rio;
  char buf[MAXLINE];
 
  int clientfd = Open_clientfd(hostname, port);
  Rio_readinitb(&rio, clientfd);

  char* message = "Hello from client.";
  Rio_writen(clientfd, message, 18);
  printf("C - Sending: %s\n", message);

  Rio_readlineb(&rio, buf, MAXLINE);
  printf("C - Recieved: %s\n", buf);
 
  return NULL;
}

void* server() {
  int listenfd, connfd;
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;  
  listenfd = Open_listenfd(port);

  char client_hostname[MAXLINE], client_port[MAXLINE];
  
  printf("S - Connected to (%s, %s)\n", hostname, port);
  while(1) {
    clientlen = sizeof(struct sockaddr_storage);
    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, client_port, MAXLINE, 0);   
  
    size_t n;
    rio_t rio;
    char buf[MAXLINE];

    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
      printf("S - Received: %s\n", buf);
      
      char* message = "Hello back.";
      printf("S - Sending: %s\n", message);
      Rio_writen(connfd, message, 11);
      }
  }
  return NULL;
}

int main() {
  pthread_create(&server_thread, NULL, server, NULL);
  // Needs to wait for the server to start before client.
  sleep(1);

  pthread_create(&client_thread, NULL, client, NULL);

  pthread_join(client_thread, NULL);  
  pthread_join(server_thread, NULL);

  exit(0);
}
