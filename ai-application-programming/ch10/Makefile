#
# Makefile for bf
#

CC = gcc
CFLAGS = -Wall

OBJS =	bigram.o

all:	bigram

bigram: $(OBJS)
	$(CC) -o bigram $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f bigram *.o
