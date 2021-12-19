# Makefile for 'indexer'
#
# Joshua Pfefferkorn, CS50 Winter 2021

PROG = indexer
PROG2 = indextest
TEST = testing.sh
OBJS = indexer.o 
OBJS2 = indextest.o
LIBS = ../common/common.a
L = ../libcs50
LLIBS = $L/libcs50.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
VALGRIND = valgrind
VFLAGS = --leak-check=full --show-leak-kinds=all
VOUT = valgrind.out
VARGS = ../crawler/data/seed0/depth1/ indexout

MAKE = make

.PHONY: all test valgrind clean

all: $(PROG) $(PROG2)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) $(LLIBS) -o $@

indexer.o: $L/hashtable.h $L/counters.h $L/file.h $L/webpage.h

$(PROG2): $(OBJS2)
	$(CC) $(CFLAGS) $^ $(LIBS) $(LLIBS) -o $@

indextest.o: $L/hashtable.h $L/counters.h $L/file.h

test:
	./$(TEST) &> testing.out

valgrind:
	$(VALGRIND) $(VFLAGS) ./$(PROG) $(VARGS) &> $(VOUT)

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core
	rm -f $(PROG)
	rm -f $(PROG2)