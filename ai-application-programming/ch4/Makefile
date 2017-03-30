#
# Makefile for emhttp
#

CC = gcc
CFLAGS = -g

OBJS =	antalg.o

all:	antalg

antalg: $(OBJS)
	$(CC) -o antalg $(OBJS) -lm

antalg: antalg.c


.c.o:
	$(CC) $(CFLAGS) -Wall -c $<

clean:
	rm -f core antalg cities.dat solution.dat *.o
