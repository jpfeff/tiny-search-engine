/* 
 * indextest.c -  tester for web indexer for CS50 TSE
 *
 * see README.md for more information
 *
 * Joshua Pfefferkorn, CS50 Winter 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/file.h"
#include "../common/pagedir.h"
#include "../common/index.h"

/**************** main ****************/
// parses arguments and initializes other modules

int main (const int argc, char *argv[]) {
    // make sure there are 2 arguments; if not, print error message and exit with non-zero status
    if (argc != 3) {
        fprintf(stderr, "Must enter 2 arguments: oldIndexFilename newIndexFilename\n");
        exit(1);
    }

    char *oldIndexFilename = argv[1];
    char *newIndexFilename = argv[2];

    // check that oldIndexFilename is readable
    FILE *file;
    if ((file = fopen(oldIndexFilename, "r")) == NULL) {
        fprintf(stderr, "%s is not readable\n", oldIndexFilename);
        exit(1);
    }
    fclose(file);

    // create newIndexFilename and check that it is writeable
    if ((file = fopen(newIndexFilename, "w")) == NULL) {
        fprintf(stderr, "%s is not writeable\n", newIndexFilename);
        exit(1);
    }
    fclose(file);

    hashtable_t *index = readIndexFromFile(oldIndexFilename);
    writeIndexToFile(index, newIndexFilename);

    hashtable_delete(index, counterDelete);
}
