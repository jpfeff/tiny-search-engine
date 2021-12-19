# Makefile for 'common' library
#
# Joshua Pfefferkorn, CS50 Winter 2021

OBJS = pagedir.o index.o word.o
LIBS = common.a
L = ../libcs50

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$L
MAKE = make

$(LIBS): $(OBJS)
	ar cr $(LIBS) $(OBJS)

pagedir.o: pagedir.h $L/file.h $L/webpage.h
index.o: index.h
word.o: word.h

.PHONY: clean

all: 
	$(LIBS)

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core