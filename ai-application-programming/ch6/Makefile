#
# Makefile for emhttp
#

CC = gcc
CFLAGS =

OBJS =	stm.o select.o fitness.o init.o main.o

all:	stm 

stm: $(OBJS)
	$(CC) -o stm $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -Wall -c $<

clean:
	rm -f stm *.o stats.txt core
