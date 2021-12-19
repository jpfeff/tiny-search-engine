/* 
 * pagedir.c -  functions for crawler and indexer
 *
 * see README.md for more information
 *
 * Joshua Pfefferkorn, CS50 Winter 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pagedir.h"
#include "../libcs50/webpage.h"

/**************** pagesaver ****************/
// fetches a page from a URL

void pagesaver(webpage_t *webpage, char *pageDirectory, int id) {
    // allocate memory for id as a string
    char *idstr = malloc(sizeof(char *));
    // change id int to string
    sprintf(idstr, "%d", id);

    // allocate memory for new filename
    char *filename = malloc(sizeof(char *) * (strlen(idstr) + strlen(pageDirectory) + 1) + 1);
    // copy pageDirectory into filename
    strcpy(filename, pageDirectory);
    // append id
    strcat(filename, idstr);

    FILE *file;
    // if successfullly opened file
    if ((file = fopen(filename, "w")) != NULL) {
        fprintf(file, "%s\n", webpage_getURL(webpage));  // write URL to file
        fprintf(file, "%d\n", webpage_getDepth(webpage));    // write depth to file
        fprintf(file, "%s\n", webpage_getHTML(webpage));  // write HTML to file
        fclose(file); // close the file
    }
    else {
        fclose(file);
        free(filename);
        free(idstr);
        fprintf(stderr, "Failed to write %s to %s\n", filename, pageDirectory);
    }
    
    // deallocate memory used for filename and id string
    free(filename);
    free(idstr);
}

/**************** isCrawlerDirectory ****************/
// verifies that directory is produced by crawler

bool isCrawlerDirectory(char *dir) {
    char *suffix = ".crawler";
    // allocate space for string to hold dir and suffix
    char *filename = malloc(sizeof(char *) * (strlen(suffix) + strlen(dir) + 1) + 1);
    strcpy(filename, dir);
    strcat(filename, suffix);

    // try to open file
    FILE *file;    
    if ((file = fopen(filename, "r")) == NULL) {
        free(filename);
        return false;
    }
    free(filename);
    fclose(file);
    return true;
}