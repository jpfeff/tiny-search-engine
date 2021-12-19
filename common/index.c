/* 
 * index.c -  functions for indexer
 *
 * see README.md for more information
 *
 * Joshua Pfefferkorn, CS50 Winter 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"
#include "index.h"

/**************** getCrawlerFilename ****************/
// appends /ID to pageDirectory to get name of a file

char* getCrawlerFilename(char *pageDirectory, int id) {
    // allocate memory for id as a string
    char *idstr = malloc(sizeof(char *));
    // change id int to string with / in front
    sprintf(idstr, "/%d", id);

    // allocate memory for filename
    char *filename = malloc(sizeof(char *) * (strlen(pageDirectory) + strlen(idstr) + 1) + 1);

    // copy pageDirectory and /ID into filename
    strcpy(filename, pageDirectory);
    strcat(filename, idstr);

    free(idstr);
    return filename;
}

/**************** addWordToIndex ****************/
// adds word to index if not there; otherwise, increments counter

void addWordToIndex(char *word, hashtable_t *index, int id) {
    counters_t *counters = hashtable_find(index, word);
    // if word is not yet in index
    if (counters == NULL) {
        // create a new counter, increment it, and add it to the index
        counters_t *newcounter = counters_new();
        counters_add(newcounter, id);
        hashtable_insert(index, word, newcounter);
    }
    // if word is already there, increment counter
    else {
        counters_add(counters, id);
    }
}

/**************** writeIndexToFile ****************/
// writes index to file

void writeIndexToFile(hashtable_t *index, char *indexFilename) {
    // open file
    FILE *file = fopen(indexFilename, "w");
    hashtable_iterate(index, file, printHashtable);
    fclose(file);
}

/**************** readIndexFromFile ****************/
// reads index from file, copying it into the index structure
hashtable_t* readIndexFromFile(char *indexFilename) {
    FILE *file;

    // try to open for reading; if NULL, return NULL
    if ((file = fopen(indexFilename, "r")) == NULL) {
        return NULL;
    }

    // number of slots is the same as number of words in the file
    int size = lines_in_file(file);
    hashtable_t *index = hashtable_new(size);

    char *word; 
    // while still words to read
    for (int k = 0; k < size; k++) {
        // update word
        word = freadwordp(file);

        counters_t *counters = counters_new();
        int id;
        int count;

        // while there are still pairs of id and count to scan
        while (fscanf(file, "%d %d ", &id, &count) == 2) {
            // create a counter for the ID and set it to count
            counters_add(counters, id);
            counters_set(counters, id, count);
        }

        // and insert the counters into index
        hashtable_insert(index, word, counters);
        free(word);
    }

    // close the file and return completed index
    fclose(file);
    return index;
}

/**************** counterDelete ****************/
// helper method for deleting index

void counterDelete(void *item) {
    counters_t *counter = item;
    counters_delete(counter);
}

/**************** printHashtable ****************/
// used as a helper method in hashtable_iterate

void printHashtable(void *arg, const char *key, void *item) {
    // prints the word
    fprintf(arg, "%s ", key);
    // print counters and doc IDs using helper method
    counters_iterate(item, arg, printCounters);
    fprintf(arg, "\n");
}

/**************** printCounters ****************/
// used as a helper method in counters_iterate

void printCounters(void *arg, const int key, int count) {
    // print doc ID and then count, separated by spaces
    fprintf(arg, "%d %d ", key, count);
}