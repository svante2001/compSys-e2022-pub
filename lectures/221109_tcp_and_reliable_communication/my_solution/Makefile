CC = gcc
CFLAGS =-Wall -Wextra -pedantic -std=gnu11
LD_FLAGS = -lpthread

all : client

rebuild : clean all

csapp.o : csapp.c csapp.h
	$(CC) $(CFLAGS) -c $< -o $@

client : client.c csapp.o
	$(CC) $(CFLAGS) $< *.o -o $@ $(LD_FLAGS)

clean :
	rm -rf *.o client
