/* 
 * crawler.c -  web crawler for CS50 TSE
 *
 * see README.md for more information
 *
 * Joshua Pfefferkorn, CS50 Winter 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/file.h"
#include "../libcs50/bag.h"
#include "../libcs50/webpage.h"
#include "../common/pagedir.h"

/**************** functions ****************/
// loops over pages to explore, until the list is exhausted
void crawler(char *seedURL, char *pageDirectory, int maxDepth);
// fetches a page from a URL
bool pagefetcher(webpage_t *webpage);
// extracts URLs from a page and processes each one
void pagescanner(webpage_t *webpage, hashtable_t *hashtable, bag_t *bag);
// logs one word (1-9 chars) about a given url 
inline static void logr(const char *word, const int depth, const char *url);

/**************** main ****************/
// parses arguments and initializes other modules

int main (const int argc, char *argv[]) {
    // make sure there are 3 arguments; if not, print error message and exit with non-zero status
    if (argc != 4) {
        fprintf(stderr, "Must enter 3 arguments: seedURL pageDirectory maxDepth\n");
        exit(1);
    }
    
    // store arguments into variables
    char *seedURL = argv[1];
    char *pageDirectory = argv[2];
    char *maxDepth = argv[3];

    // verify that maxDepth is an integer
    int maxDepthNum;
    if (sscanf(maxDepth, "%d", &maxDepthNum) != 1) {
        fprintf(stderr, "Invalid argument: %s\nnmaxDepth argument must be an integer\n", maxDepth);
        exit(1);
    }
    // and is within the correct range
    else if (maxDepthNum < 0 || maxDepthNum > 10) {
        fprintf(stderr, "Invalid argument: %s\nmaxDepth argument must be in the range [0,10]\n", maxDepth);
        exit(1);
    }
    
    // verify that pageDirectory exists by adding ".crawler" to the parameter and trying to open it

    char *suffix = ".crawler";
    // allocate space for string to hold pageDirectory and suffix
    char *filename = malloc(sizeof(char *) * (strlen(suffix) + strlen(pageDirectory) + 1) + 1);
    // concatenate the strings without changing pageDirectory
    strcpy(filename, pageDirectory);
    strcat(filename, suffix);

    // try to open file
    FILE *file;    
    if ((file = fopen(filename, "w")) == NULL) {
        fprintf(stderr, "Invalid argument: %s\n%s is not a directory or is not writeable\n", pageDirectory, pageDirectory);
        exit(1);
    }
    else {
        // if not NULL, pageDirectory exists and is writeable; close
        fclose(file);
    }
    // deallocate memory after check
    free(filename);

    // verify that seedURL is internal using webpage.c function
    if (!IsInternalURL(seedURL)) {
        fprintf(stderr, "Invalid argument: %s\nseedURL argument must be an internal URL\n", seedURL);
        exit(1);
    }

    // at this point, all the arguments have been verified, so call crawler module
    crawler(seedURL, pageDirectory, maxDepthNum);

    return 0;
}

/**************** crawler ****************/
// loops over pages to explore, until the list is exhausted

void crawler(char *seedURL, char *pageDirectory, int maxDepth) {

    char *seedURLcpy = malloc(sizeof(char *) * strlen(seedURL) + 1);
    strcpy(seedURLcpy, seedURL);

    // make a webpage for the seedURL, marked with depth = 0 and NULL html
    webpage_t *webpage = webpage_new(seedURLcpy, 0, NULL);

    // check that webpage was created properly
    if (webpage == NULL) {
        fprintf(stderr, "Failed creating webpage for seedURL\n");
        exit(1);
    }

    // make a bag to hold unvisited webpages
    bag_t *bag = bag_new();
    // check that bag was created properly
    if (bag == NULL) {
        fprintf(stderr, "Failed creating bag\n");
        exit(1);
    }

    // make a hashtable of size 200 * (maxDepth + 1) to hold visited webpages
    hashtable_t *hashtable = hashtable_new(200 * (maxDepth+1));
    // check that hashtable was created properly
    if (hashtable == NULL) {
        fprintf(stderr, "Failed creating hashtable\n");
        exit(1);
    }

    // add webpage to bag of webpages to crawl
    bag_insert(bag, webpage);
    // add seedURL to hashtbale of URLs seen (with empty string item)
    hashtable_insert(hashtable, seedURL, "");

    // document id to be written by pagesaver and incremented for each visited page 
    int id = 1;

    // while there are more webpages to crawl
    while ((webpage = bag_extract(bag)) != NULL) {  // store in webpage variable

        // calls webpage_fetch, which pauses automatically for 1s between fetches
        if (pagefetcher(webpage)) { // if true, page was fetched
            logr("Fetched", webpage_getDepth(webpage), webpage_getURL(webpage));  // print useful progress statement
            // save page to file
            pagesaver(webpage, pageDirectory, id);
            logr("Saved", webpage_getDepth(webpage), webpage_getURL(webpage)); // print useful progress statement
            // increment document id
            id++;

            // if maxDepth has not been reached
            if (webpage_getDepth(webpage) < maxDepth) {
                // parse the webpage to extract all its embedded URLs
                pagescanner(webpage, hashtable, bag);
            }   
        }
        else {
            printf("Unable to fetch webpage at URL: %s\n", webpage_getURL(webpage));
        }
        // once webpage has been fetched, saved, and scanned, delete it
        webpage_delete(webpage);
    }
    // delete empty bag
    bag_delete(bag, webpage_delete);
    // items are empty strings, so no need to define itemfunc to delete hashtable
    hashtable_delete(hashtable, NULL);
}

/**************** pagefetcher ****************/
// fetches a page from a URL

bool pagefetcher(webpage_t *webpage) {
    return webpage_fetch(webpage);
}

/**************** pagescanner ****************/
// parses a webpage to extract all its embedded URLs

void pagescanner(webpage_t *webpage, hashtable_t *hashtable, bag_t *bag) {
    logr("Scanning", webpage_getDepth(webpage), webpage_getURL(webpage));  // print useful progress statement
    // adapted from usage example in webpage.h:
    int pos = 0;
    char *result;

    while ((result = webpage_getNextURL(webpage, &pos)) != NULL) {
        // depth is the current webpage's depth + 1
        logr("Found", (webpage_getDepth(webpage)+1), result);// print useful progress statement

        if (NormalizeURL(result)) {
            if (IsInternalURL(result)) {
                if (hashtable_insert(hashtable, result, "")) {
                    // create a new webpage for the URL
                    webpage_t *newwebpage = webpage_new(result, (webpage_getDepth(webpage)+1), NULL);
                    // add it to the bag
                    bag_insert(bag, newwebpage);
                    logr("Added", (webpage_getDepth(webpage)+1), result);   // print useful progress statement
                }
                else {
                    // if URL is already in hashtable (i.e. has already been visited)
                    logr("IgnDupl", (webpage_getDepth(webpage)+1), result);  // print useful progress statement
                    free(result);
                }
            }
            else {
                // if URL is not internal
                logr("IgnExtrn", (webpage_getDepth(webpage)+1), result);  // print useful progress statement
                free(result);
            }
        }
    }
    free(result);
}

/**************** logr ****************/
// logs one word (1-9 chars) about a given url 
     
// taken from https://www.cs.dartmouth.edu/~cs50/Lectures/crawler/#tse-crawler-specs                             
inline static void logr(const char *word, const int depth, const char *url) {
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
}
