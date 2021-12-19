#!/bin/bash
#
#testing.sh
#Joshua Pfefferkorn
#2/10/21
#
# A program designed for testing crawler.c

# create a framework of directories to store file output

mkdir data
mkdir data/seed0
mkdir data/seed0/depth0
mkdir data/seed0/depth1
mkdir data/seed0/depth2
mkdir data/seed0/depth3
mkdir data/seed0/depth4
mkdir data/seed0/depth5
mkdir data/seed1
mkdir data/seed1/depth0
mkdir data/seed1/depth1
mkdir data/seed1/depth2
mkdir data/seed1/depth3
mkdir data/seed1/depth4
mkdir data/seed1/depth5
mkdir data/wiki
mkdir data/wiki/depth0
mkdir data/wiki/depth1
mkdir data/wiki/depth2
mkdir data/tse
mkdir data/tse/depth2

# Invalid arguments test

echo Too few arguments, should fail:
./crawler arg1 arg2
echo

echo Too many arguments, should fail:
./crawler arg1 arg2 arg3 arg4
echo

echo maxDepth argument not an integer, should fail:
./crawler http://cs50tse.cs.dartmouth.edu/tse/ data/ one
echo

echo maxDepth argument not in range [0,10], should fail:
./crawler http://cs50tse.cs.dartmouth.edu/tse/ data/ 11
echo

echo pageDirectory argument not a directory, should fail:
./crawler http://cs50tse.cs.dartmouth.edu/tse/ notadir/ 2
echo

# seedURL tests

echo seedURL not internal, should fail:
./crawler https://en.wikipedia.org/wiki/Main_Page data/ 2
echo

echo seedURL non-existent server, should fail:
./crawler http://cs5000tse.cs.dartmouth.edu/tse/ data/ 2
echo

echo seedURL non-existent page, should be unable to fetch:
./crawler http://cs50tse.cs.dartmouth.edu/tse/nope data/ 2
echo

# simple crawl

echo Simple test crawl at depth 0:
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ data/seed0/depth0/ 0
echo

echo Simple test crawl at depth 1:
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ data/seed0/depth1/ 1
echo

echo Simple test crawl at depth 2:
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ data/seed0/depth2/ 2
echo

echo Simple test crawl at depth 3:
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ data/seed0/depth3/ 3
echo

echo Simple test crawl at depth 4:
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ data/seed0/depth4/ 4
echo

echo Simple test crawl at depth 5:
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/ data/seed0/depth5/ 5
echo

# simple crawl at new start point

echo Simple test crawl -- new start point -- at depth 0:
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/E.html data/seed1/depth0/ 0
echo

echo Simple test crawl -- new start point -- at depth 1:
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/E.html data/seed1/depth1/ 1
echo

echo Simple test crawl -- new start point -- at depth 2:
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/E.html data/seed1/depth2/ 2
echo

echo Simple test crawl -- new start point -- at depth 3:
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/E.html data/seed1/depth3/ 3
echo

echo Simple test crawl -- new start point -- at depth 4:
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/E.html data/seed1/depth4/ 4
echo

echo Simple test crawl -- new start point -- at depth 5:
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/E.html data/seed1/depth5/ 5
echo

# wikipedia crawl

echo Wikipedia crawl at depth 0:
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ data/wiki/depth0/ 0
echo

echo Wikipedia crawl at depth 1:
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ data/wiki/depth1/ 1
echo

echo Wikipedia crawl at depth 2:
./crawler http://cs50tse.cs.dartmouth.edu/tse/wikipedia/ data/wiki/depth2/ 2
echo

# general playground test

echo TSE site crawl at depth 2:
./crawler http://cs50tse.cs.dartmouth.edu/tse/ data/tse/depth2/ 2

exit 0