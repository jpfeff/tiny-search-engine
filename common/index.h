/* 
 * index.h - header file for CS50 'index' module
 *
 * Joshua Pfefferkorn, CS50 Winter 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"

// appends /ID to pageDirectory to get name of a file
char* getCrawlerFilename(char *pageDirectory, int id);

// adds word to the index 
void addWordToIndex(char *word, hashtable_t *index, int id);

// writes index to file
void writeIndexToFile(hashtable_t *index, char *indexFilename);

// reads index from file, copying it into the index structure
hashtable_t* readIndexFromFile(char *indexFilename);

// helper method for deleting index
void counterDelete(void *item);

// used as a helper method in hashtable_iterate
void printHashtable(void *arg, const char *key, void *item);

// used as a helper method in counters_iterate
void printCounters(void *arg, const int key, int count);

