#
# Makefile for emhttp
#

CC = gcc
CFLAGS =

OBJS =	rcmnd.o

all:	rcmnd

rcmnd: $(OBJS)
	$(CC) -o rcmnd $(OBJS) -lm

rcmnd: rcmnd.c 

.c.o:
	$(CC) $(CFLAGS) -Wall -c $<

clean:
	rm -f rcmnd *.o
