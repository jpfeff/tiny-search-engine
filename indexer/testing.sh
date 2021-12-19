#!/bin/bash
#
#testing.sh
#Joshua Pfefferkorn
#2/18/21
#
# A program designed for testing indexer.c and indextest.c

crawler_directory="/thayerfs/home/d29265d/shared/tse-output/crawler/crawler.data/data"

# create a framework of directories to store file output
mkdir indexout
mkdir indexout/data0
mkdir indexout/data1
mkdir indexout/data2
mkdir indexout/data3

# Invalid arguments test

echo Too few arguments, should fail:
./indexer arg
echo

echo Too many arguments, should fail:
./indexer arg1 arg2 arg3
echo

echo pageDirectory argument not a directory, should fail:
./indexer notadir/ notadireither/
echo

echo pageDirectory argument not produced by crawler, should fail:
./indexer ../common notadir/
echo

echo pageDirectory argument not readable, should fail:
mkdir test
chmod -r test
./indexer test notadir/
rm -rf test
echo

# test with data0

echo indexer and indextest test with data0:
./indexer ${crawler_directory}0/ indexout/data0/data0
./indextest indexout/data0/data0 indexout/data0/data0_test

echo sort and check for differences:
gawk -f indexsort.awk indexout/data0/data0 > indexout/data0/data0.sorted
gawk -f indexsort.awk indexout/data0/data0_test > indexout/data0/data0_test.sorted

differences=$(diff -q indexout/data0/data0.sorted indexout/data0/data0_test.sorted)
	echo "Comparing indexout/data0/data0.sorted with indexout/data0/data0_test.sorted:"
	if [ ${#differences} -gt 0 ]; then
		echo "$differences"
	else
		echo "No differences"
	fi
echo

# test with data1

echo indexer and indextest test with data1:
./indexer ${crawler_directory}1/ indexout/data1/data1
./indextest indexout/data1/data1 indexout/data1/data1_test

echo sort and check for differences:
gawk -f indexsort.awk indexout/data1/data1 > indexout/data1/data1.sorted
gawk -f indexsort.awk indexout/data1/data1_test > indexout/data1/data1_test.sorted

differences=$(diff -q indexout/data1/data1.sorted indexout/data1/data1_test.sorted)
	echo "Comparing indexout/data1/data1.sorted with indexout/data1/data1_test.sorted:"
	if [ ${#differences} -gt 0 ]; then
		echo "$differences"
	else
		echo "No differences"
	fi
echo

# test with data2

echo indexer and indextest test with data2:
./indexer ${crawler_directory}1/ indexout/data2/data2
./indextest indexout/data2/data2 indexout/data2/data2_test

echo sort and check for differences:
gawk -f indexsort.awk indexout/data2/data2 > indexout/data2/data2.sorted
gawk -f indexsort.awk indexout/data2/data2_test > indexout/data2/data2_test.sorted

differences=$(diff -q indexout/data2/data2.sorted indexout/data2/data2_test.sorted)
	echo "Comparing indexout/data2/data2.sorted with indexout/data2/data2_test.sorted:"
	if [ ${#differences} -gt 0 ]; then
		echo "$differences"
	else
		echo "No differences"
	fi
echo

# test with data3

echo indexer and indextest test with data3:
./indexer ${crawler_directory}1/ indexout/data3/data3
./indextest indexout/data3/data3 indexout/data3/data3_test

echo sort and check for differences:
gawk -f indexsort.awk indexout/data3/data3 > indexout/data3/data3.sorted
gawk -f indexsort.awk indexout/data3/data3_test > indexout/data3/data3_test.sorted

differences=$(diff -q indexout/data3/data3.sorted indexout/data3/data3_test.sorted)
	echo "Comparing indexout/data3/data3.sorted with indexout/data3/data3_test.sorted:"
	if [ ${#differences} -gt 0 ]; then
		echo "$differences"
	else
		echo "No differences"
	fi
echo
