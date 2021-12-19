# Makefile for 'querier' and 'fuzzquery'
#
# Joshua Pfefferkorn, CS50 Winter 2021

PROG = querier
PROG2 = fuzzquery
TEST = testing.sh
OBJS = querier.o
OBJS2 = fuzzquery.o
LIBS = ../common/common.a
L = ../libcs50
LLIBS = $L/libcs50.a

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
VALGRIND = valgrind
VFLAGS = --leak-check=full --show-leak-kinds=all
VARGS = /thayerfs/home/d29265d/shared/tse-output/crawler/crawler.data/data2/ /thayerfs/home/d29265d/shared/tse-output/indexer/indexer.data/index.2
VOUT = valgrind.out

MAKE = make

.PHONY: all test valgrind clean

all: $(PROG) $(PROG2)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) $(LLIBS) -o $@

querier.o: $L/hashtable.h $L/counters.h $L/file.h

$(PROG2): $(OBJS2)
	$(CC) $(CFLAGS) $^ $(LIBS) $(LLIBS) -o $@

fuzzquery.o: $L/memory.h $L/file.h

test:
	./$(TEST) &> testing.out

valgrind:
	$(VALGRIND) $(VFLAGS) ./$(PROG) $(VARGS) < testquery3 &> $(VOUT)

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core
	rm -f $(PROG)
	rm -f $(PROG2)