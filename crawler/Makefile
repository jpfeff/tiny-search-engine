# Makefile for 'crawler'
#
# Joshua Pfefferkorn, CS50 Winter 2021

PROG = crawler
TEST = testing.sh
OBJS = crawler.o
LIBS = ../common/common.a
L = ../libcs50
LLIBS = $L/libcs50.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
VALGRIND = valgrind
VFLAGS = --leak-check=full --show-leak-kinds=all
VARGS = http://cs50tse.cs.dartmouth.edu/tse/letters/ data/seed0/depth1/ 3
VOUT = valgrind.out

MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) $(LLIBS) -o $@

crawler.o: $L/hashtable.h $L/bag.h $L/file.h $L/webpage.h

.PHONY: test valgrind clean

test:
	./$(TEST) &> testing.out

valgrind:
	$(VALGRIND) $(VFLAGS) ./$(PROG) $(VARGS) &> $(VOUT)

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core
	rm -f $(PROG)