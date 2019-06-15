DEST := dest
CC := gcc
CFLAGS := -g -Wall -Wextra -std=c11
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

.PHONY: test clean

test: 9cc
	./9cc -test
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*

9cc: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^
