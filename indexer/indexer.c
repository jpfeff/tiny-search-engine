/* 
 * indexer.c -  web indexer for CS50 TSE
 *
 * see README.md for more information
 *
 * Joshua Pfefferkorn, CS50 Winter 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/file.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../common/word.h"

/**************** functions ****************/
// loops over pages to explore, until the list is exhausted
void indexer(char *pageDirectory, char *indexFilename);

/**************** main ****************/
// parses arguments and initializes other modules

int main (const int argc, char *argv[]) {
    // make sure there are 2 arguments; if not, print error message and exit with non-zero status
    if (argc != 3) {
        fprintf(stderr, "Must enter 2 arguments: pageDirectory indexFilename\n");
        exit(1);
    }
    
    // store arguments into variables
    char *pageDirectory = argv[1];
    char *indexFilename = argv[2];

    // verify that pageDirectory is readable and was produced by crawler
    if (!isCrawlerDirectory(pageDirectory)) {
        fprintf(stderr, "Invalid argument: %s\n%s is not readable or was not produced by crawler\n", pageDirectory, pageDirectory);
        exit(1);
    }

    // check indexFilename (if it already exists, overwrite it; if not, create it)
    FILE *file = fopen(indexFilename, "w+");

    if (file == NULL) {
        fprintf(stderr, "Invalid argument: %s is not writeable\n", indexFilename);
        exit(1);
    }
    fclose(file);

    // at this point, all the arguments have been verified, so call indexer module
    indexer(pageDirectory, indexFilename);

    return 0;
}

void indexer(char *pageDirectory, char *indexFilename) {
    // make a hashtable of size 500
    hashtable_t *index = hashtable_new(500);

    // get first filename: pageDirectory/1
    int id = 1;
    char *filename = getCrawlerFilename(pageDirectory, id);

    FILE *file;
    // while there are still readable files
    while ((file = fopen(filename, "r")) != NULL) {
        // store the url from the first line
        char *url = freadlinep(file);
        // the depth as a string from the second line 
        char *depthstr = freadlinep(file);
        // and the html from the rest of the file
        char *html = freadfilep(file);

        // convert depth to an int
        int depth = 0;
        sprintf(depthstr, "/%d", depth);

        // create webpage
        webpage_t *webpage = webpage_new(url, depth, html);

        char *word;
        int pos = 0;

        // scan through webpage and process each word
        while ((word = webpage_getNextWord(webpage, &pos)) != NULL) {
            // make sure length > 2
            if (strlen(word) > 2) {
                NormalizeWord(word);
                addWordToIndex(word, index, id);
            }
            free(word);
        }

        // free webpage and close file
        fclose(file);
        webpage_delete(webpage);

        // free allocated strings
        free(depthstr);
        free(filename);

        // increment id and get name of next file
        id++;
        filename = getCrawlerFilename(pageDirectory, id);
    }
    free(filename);

    // print the index to the file
    writeIndexToFile(index, indexFilename);

    // free index and counters
    hashtable_delete(index, counterDelete);
}

