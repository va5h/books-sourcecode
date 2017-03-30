#
# Makefile for emhttp
#

CC = gcc
CFLAGS = -g -O2

OBJS =	main.o

all:	sim

sim: $(OBJS)
	$(CC) -o sim $(OBJS)

sim: main.c


.c.o:
	$(CC) $(CFLAGS) -Wall -c $<

clean:
	rm -f core sim *.o
