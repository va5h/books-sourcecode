#
# Makefile for emhttp
#

CC = gcc

OBJS =	emsa.o

all:	emsa

emsa: $(OBJS)
	$(CC) -g -o emsa $(OBJS) -lm

.c.o:
	$(CC) $(CFLAGS) -g -Wall -c $<

emsa.c:	common.h

clean:
	rm -f emsa *.o core
