#!/bin/bash
#
#testing.sh
#Joshua Pfefferkorn
#2/18/21
#
# A program designed for testing querier.c

crawler_directory="/thayerfs/home/d29265d/shared/tse-output/crawler/crawler.data/data2/"
index_file="/thayerfs/home/d29265d/shared/tse-output/indexer/indexer.data/index.2"

# test with testquery1 (all invalid arguments)

echo All the following arguments should fail:
echo

./querier $crawler_directory $index_file < testquery1
echo

# test with testquery2 (valid arguments)

echo The following arguments are valid and should produce the correct output:
echo

./querier $crawler_directory $index_file < testquery2

# test with fuzzquery produced arguments

echo The following arguments have been randomly produced by fuzzquery:
echo

./fuzzquery $index_file 10 0 | ./querier $crawler_directory $index_file