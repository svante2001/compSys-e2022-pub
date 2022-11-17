#include "csapp.h"
#include <time.h>
#include <pthread.h>

int clientfd;
char buf[MAXLINE];
rio_t rio;
pthread_t handler_thread;
pthread_t timer_thread;

void* handler() {
  Rio_writen(clientfd, "a", 1);
  Rio_readlineb(&rio, buf, MAXLINE);
  printf("Recieved: %s\n", buf);
  pthread_cancel(timer_thread);
  return NULL;
}

void* timer() {
  sleep(1);
  printf("Timed out.\n");
  pthread_cancel(handler_thread);
  return NULL;
}

int main() {
  clientfd = Open_clientfd("0.0.0.0", "12345");
  Rio_readinitb(&rio, clientfd);
  
  printf("Press enter to send a byte.");
  getchar();

  pthread_create(&handler_thread, NULL, handler, NULL);
  pthread_create(&timer_thread, NULL, timer, NULL);

  pthread_join(handler_thread, NULL);
  pthread_join(timer_thread, NULL);
  
  Close(clientfd);
  exit(0);
}
