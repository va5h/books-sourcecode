#
# Makefile for emhttp
#

CC = gcc
CFLAGS =

OBJS =	backprop.o

all:	backprop

backprop: $(OBJS)
	$(CC) -o backprop $(OBJS) -lm

backprop: backprop.c 

.c.o:
	$(CC) $(CFLAGS) -Wall -c $<

clean:
	rm -f backprop *.o stats.txt
