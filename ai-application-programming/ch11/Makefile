#
# Makefile for webagent
#

CC = gcc

OBJS =	main.o prsconf.o monitor.o nntpc.o parsers.o newsmon.o \
	https.o newsgen.o fileio.o

all:	webagent

webagent: $(OBJS)
	$(CC) -o webagent $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -g -Wall -c $<

clean:
	rm -f webagent *.o core
