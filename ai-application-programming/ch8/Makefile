#
# Makefile for emhttp
#

CC = gcc

OBJS =	main.o parser.o interp.o rulechk.o rulefire.o timers.o

all:	rbs

rbs: $(OBJS)
	$(CC) -o rbs $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -g -Wall -c $<

clean:
	rm -f rbs *.o
