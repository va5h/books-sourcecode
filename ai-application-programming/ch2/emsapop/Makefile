#
# Makefile for emhttp
#

CC = gcc

OBJS =	emsapop.o

all:	emsapop

emsapop: $(OBJS)
	$(CC) -o emsapop $(OBJS) -lm

.c.o:
	$(CC) $(CFLAGS) -Wall -c $<

clean:
	rm -f emsapop *.o core
