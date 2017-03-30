
CC = gcc

OBJS =	fuzzy.o mbrship.o simulate.o control.o main.o

all:	fuzzytest

fuzzytest: $(OBJS)
	$(CC) -o fuzzytest $(OBJS) -lm

.c.o:
	$(CC) $(CFLAGS) -Wall -c $<

clean:
	rm -f fuzzytest *.o
