/* 
 * querier.c -  querier for CS50 TSE
 *
 * see README.md for more information
 *
 * Joshua Pfefferkorn, CS50 Winter 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../common/word.h"
#include "../libcs50/file.h"

/**************** local types ****************/

typedef struct twocounters {
    counters_t *one;
    counters_t *two;
} twocounters_t ;

typedef struct doc {
    int id;
    int score;
} doc_t;

/**************** functions ****************/

// prints a ranked list of documents that match the query
void querier(char *pageDirectory, char *indexFilename);
// catches any invalid characters in the query 
bool validateQueryCharacters(char *query);
// returns number of words in query
int countWords(char *query);
// stores words (tokens separated by spaces) into array
void splitWordsIntoArray(char *query, int numwords, char **words);
// catches invalid order of "and" and "or"
bool validateQueryWords(int numwords, char **words);
// retrieves a counter set of document IDs and their respective scores
counters_t* getScores(hashtable_t *index, int numwords, char **words);
// finds intersection between counter sets
counters_t *counters_intersect(counters_t *countersOne, counters_t *countersTwo);
// helper method for counters_intersect
void counters_intersect_helper(void *arg, const int key, int count);
// used in conjunction with combine_counters to find the union between counter sets
counters_t *counters_union(counters_t *countersOne, counters_t *countersTwo);
// helper method for counters_union
void counters_union_helper(void *arg, const int key, int count);
// used in conjunction with counters_union to find the union between counter sets
counters_t *combine_counters(counters_t *counter1, counters_t *counter2);
// helper method for combine_counters
void combine_counters_helper(void *arg, const int key, int count);
// retrieves the number of counters in a counter set
void numCounters(void *arg, const int key, int count);
// sorts the documents into decending order by score
doc_t *rankDocs(counters_t *scores, int size, int totalFiles);
// helper method for qsort function used in rankDocs
int qsort_helper(const void *one, const void *two);
// counts the number of files in a directory
int countFiles(char *pageDirectory);
// necessary for prompt
int fileno(FILE *stream);
// print "Query?" prompt
static void prompt(void);
// print visual divider
static void printDivider(void);

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

    // check that indexFilename is readable
    FILE *file = fopen(indexFilename, "r");

    if (file == NULL) {
        fprintf(stderr, "Invalid argument: %s is not readable\n", indexFilename);
        fclose(file);
        exit(1);
    }
    fclose(file);

    // at this point, all the arguments have been verified, so call querier module
    querier(pageDirectory, indexFilename);

    return 0;
}

/**************** querier ****************/
// prints a ranked list of documents that match the query

void querier(char *pageDirectory, char *indexFilename) {
    // initialize a hashtable and copy information from file into the data structure
    hashtable_t *index = readIndexFromFile(indexFilename);

    // prompt for query
    prompt();

    // current query
    char *cur;
    while ((cur = readlinep()) != NULL) {

        // ensure that query contains only valid characters and is not empty
        if (!validateQueryCharacters(cur)) {
            // prompt again without performing the query
            prompt();
            free(cur);
            continue;
        }
        
        int numwords = countWords(cur);
        char *words[numwords];
        splitWordsIntoArray(cur, numwords, words);

        // ensure that query contains valid order of "and" and "or"
        if (!validateQueryWords(numwords, words)) {
            // prompt again without performing the query
            prompt();
            free(cur);
            continue;
        }

        // echo cleaned query
        printf("Query: ");
        for (int k = 0; k < numwords; k++) {
            printf("%s ", words[k]);
        }
        printf("\n");

        counters_t *scores = getScores(index, numwords, words);

        int size = 0;
        counters_iterate(scores, &size, numCounters);

        if (size == 0) {
            printf("No documents match\n");
            printDivider();
            prompt();
            continue;
        }

        int totalFiles = countFiles(pageDirectory);
        // get sorted array of matching documents
        doc_t *rankedFiles = rankDocs(scores, size, totalFiles);

        printf("Matches %d documents (ranked):\n", size);
        for (int k = 0; k < size; k++) {
            // store id and score of current doc
            int id = rankedFiles[k].id;
            int score = rankedFiles[k].score;

            // open file and store url to print
            char *filename = getCrawlerFilename(pageDirectory, id);
            FILE *file = fopen(filename, "r");
            char *url = freadlinep(file);
            fclose(file);

            // print info
            printf("score\t%d\tdoc\t%d:\t%s\n", score, id, url);

            free(filename);
            free(url);
        }
    
        // free current query
        free(cur);

        // free data structures
        free(rankedFiles);
        //counters_delete(scores);

        // prompt for next query
        printDivider();
        prompt();
    }
    free(cur);
    printf("\n");
}

/**************** validateQueryCharacters ****************/
// catches any invalid characters in the query

bool validateQueryCharacters(char *query) {
    
    // false if query is empty or NULL
    if ((query[0] == '\0') || (query == NULL)) {
        printf("Error: query is empty or NULL\n");
        printDivider();
        return false;
    }

    // create counter for spaces
    int numspaces = 0;
    int numchars = 0;

    for (int k = 0; query[k] != '\0'; k++) {

        // if non-alpha and not a space, return false
        if (!isalpha(query[k]) && !isspace(query[k])) {
            printf("Error: bad character '%c' in query\n", query[k]);
            printDivider();
            return false;
        }

        // increment space counter if space
        if (isspace(query[k])) {
            numspaces++;
        }
        numchars++;
    }

    // false if query is all spaces
    if (numspaces == numchars) {
        printf("Error: query is all spaces\n");
        printDivider();
        return false;
    }

    return true;
}

/**************** countWords ****************/
// returns number of words in query

int countWords(char *query) {

    query = NormalizeWord(query);

    int pos = 0;
    int numwords = 0;

    while (query[pos] != '\0') {
        // when this while loop finishes, pos should be at the beginning of a word
        while (query[pos] != '\0' && !isalpha(query[pos])) {
            pos++;
        }
        // make sure we have not reached the end of the query
        if (query[pos] == '\0') {
            return numwords;
        }

        // ensure we are at the beginning of a word and increment word count
        if (isalpha(query[pos])) {
            numwords++;
        }

        // when this loop finishes, pos should be at the end of a word
        while(isalpha(query[pos])) {
            pos++;
        }
    }

    return numwords;
}

/**************** splitWordsIntoArray ****************/
// stores words (tokens separated by spaces) into array

void splitWordsIntoArray(char *query, int numwords, char **words) {

    // pointer to first char in word
    char *word = query;
    // pointer to char directly after word
    char *rest = query;

    // loop over array
    for (int k = 0; k < numwords; k++) {
        // when this loop finishes, word should point to first char in word
        while(!isalpha(*word)) {
            word++;
        }
        // move rest up to word
        rest = word;

        // when this loop finishes, rest should point to the first space after the word
        while (isalpha(*rest)) {
            rest++;
        }
        *rest = '\0';

        // add the normalized word to the array
        words[k] = NormalizeWord(word);

        // move word up to rest
        word = rest;
    }
}

/**************** validateQueryWords ****************/
// catches invalid order of "and" and "or"

bool validateQueryWords(int numwords, char **words) {
    // return false if first word is "and"
    if (strcmp(words[0], "and") == 0) {
        printf("Error: 'and' cannot be first\n");
        printDivider();
        return false;
    }

    // return false if first word is "or"
    if (strcmp(words[0], "or") == 0) {
        printf("Error: 'or' cannot be first\n");
        printDivider();
        return false;
    }

    // return false if last word is "and"
    if (strcmp(words[numwords-1], "and") == 0) {
        printf("Error: 'and' cannot be last\n");
        printDivider();
        return false;
    }

    // return false if last word is "or"
    if (strcmp(words[numwords-1], "or") == 0) {
        printf("Error: 'or' cannot be last\n");
        printDivider();
        return false;
    }

    // return false if "and" is adjacent to another "and" or an "or"
    for (int k = 0; k < numwords-1; k++) {
        if (strcmp(words[k], "and") == 0) {
            char *previous = words[k-1];
            char *next = words[k+1];

            // return false if previous or next is "and"
            if (strcmp(previous, "and") == 0 || strcmp(next, "and") == 0) {
                printf("Error: 'and' cannot be adjacent to 'and'\n");
                printDivider();
                return false;
            }

            // return false if previous or next is "or"
            if (strcmp(previous, "or") == 0 || strcmp(next, "or") == 0) {
                printf("Error: 'and' cannot be adjacent to 'or'\n");
                printDivider();
                return false;
            }
        }
    }
    return true;
}

/**************** getScores ****************/
// get documents and their scores for the first word

counters_t* getScores(hashtable_t *index, int numwords, char **words) {
    // get documents and their scores for the first word
    counters_t *sum = counters_new();
    counters_t *product = NULL;

    for (int k = 0; k < numwords; k++) {
        // skip if word is "and"
        if ((strcmp(words[k], "and") == 0)) {
            continue;
        }
        // otherwise if word is "or"
        if ((strcmp(words[k], "or") == 0)) {
            if (product != NULL) {
                counters_t *temp1 = counters_union(sum, product);
                counters_t *temp2 = counters_union(product, sum);
                sum = combine_counters(temp1, temp2);

                product = NULL;
            }
            continue;
        }
        // word must be a normal word
        if (product == NULL) {
            // if current total is empty (word is first or directly after an "or")
            // set it to the index counter for the current word, no need to merge
            product = hashtable_find(index, words[k]);
            continue;
        }

        counters_t *next = hashtable_find(index, words[k]);
        // merge current total and next word
        product = counters_intersect(product, next);
    }

    counters_t *temp1 = counters_union(sum, product);
    counters_t *temp2 = counters_union(product, sum);
    sum = combine_counters(temp1, temp2);

    return sum;
}

/**************** counters_intersect ****************/
// finds intersection between counter sets

counters_t *counters_intersect(counters_t *countersOne, counters_t *countersTwo) {
    counters_t *intersection = counters_new();
    twocounters_t *twocounters = malloc(sizeof(twocounters_t));
    twocounters -> one = countersOne;
    twocounters -> two = intersection;

    counters_iterate(countersTwo, twocounters, counters_intersect_helper);

    intersection = twocounters -> two;
    free(twocounters);

    return intersection;
}

/**************** counters_intersect_helper ****************/
// helper method for counters_intersect

void counters_intersect_helper(void *arg, const int key, int count) {
    twocounters_t *twocounters = arg;

    int ct = counters_get(twocounters -> one, key);
    if (ct != 0) {
        if (ct < count) {
            counters_set(twocounters -> two, key, ct);
        }
        else {
            counters_set(twocounters -> two, key, count);
        }
    }
}

/**************** counters_union ****************/
// used in conjunction with combine_counters to find the union between counter sets

counters_t *counters_union(counters_t *countersOne, counters_t *countersTwo) {

    // if first is empty, just return second 
    int size1 = 0;
    counters_iterate(countersOne, &size1, numCounters);
    if (size1 == 0) {
        return countersTwo;
    }

    // if second is empty, just return first
    int size2 = 0;
    counters_iterate(countersTwo, &size2, numCounters);
    if (size2 == 0) {
        return countersOne;
    }

    // create new counter set to hold union of counters
    counters_t *merged = counters_new();

    // create new twocounters so that both can be passed to counters_iterate
    twocounters_t *twocounters = malloc(sizeof(twocounters_t));
    twocounters -> one = countersOne;
    twocounters -> two = merged;

    counters_iterate(countersTwo, twocounters, counters_union_helper);
    free(twocounters);

    return merged;
}

/**************** counters_union_helper ****************/
// helper method for counters_union

void counters_union_helper(void *arg, const int key, int count) {
    twocounters_t *twocounters = arg;

    // store the score from the first counter
    int ct = counters_get(twocounters -> one, key);

    // replace the score of the second counter with the sum of that of both counters
    counters_set(twocounters -> two, key, count + ct);
}

void numCounters(void *arg, const int key, int count) {
    if (count != 0) {
        int *numItems = arg;
        (*numItems)++;
    }
}

/**************** combine_counters ****************/
// used in conjunction with counters_union to find the union between counter sets

counters_t *combine_counters(counters_t *counter1, counters_t *counter2) {
    counters_t *result = counter1;
    counters_iterate(counter2, result, combine_counters_helper);
    return result;
}

/**************** combine_counters_helper ****************/
// helper method for combine_counters

void combine_counters_helper(void *arg, const int key, int count) {
    counters_t *counter1 = arg;
    if (counters_get(counter1, key) == 0) {
        counters_set(counter1, key, count);
    }
}

/**************** rankDocs ****************/
// sorts the documents into decending order by score

doc_t *rankDocs(counters_t *scores, int size, int totalFiles) {
    doc_t *rankedDocs = calloc(size, sizeof(doc_t));
    int pos = 0;
    for (int k = 1; k <= totalFiles; k++) {
        // get score of document
        int score = counters_get(scores, k);
        // if 0, ignore
        if (score != 0) {
            // create a document to insert and set its id and score
            doc_t cur;
            cur.id = k;
            cur.score = score;

            // insert it to the doc array at current pos and increment pos
            rankedDocs[pos] = cur;
            pos++;
        }
    }

    // sort the array of docs by score
    qsort(rankedDocs, size, sizeof(doc_t), qsort_helper);

    return rankedDocs;
}

/**************** qsort_helper ****************/
// helper method for qsort function used in rankDocs

int qsort_helper(const void *one, const void *two) {
    doc_t *doc1 = (doc_t*)one;
    doc_t *doc2 = (doc_t*)two;

    return(doc2 -> score - doc1 -> score);
}

/**************** countFiles ****************/
// counts the number of files in a directory

int countFiles(char *pageDirectory) {
    int curID = 1;
    char *filename = getCrawlerFilename(pageDirectory, curID);
    FILE *file;

    while ((file = fopen(filename, "r")) != NULL) {
        curID++;
        fclose(file);
        free(filename);
        filename = getCrawlerFilename(pageDirectory, curID);
    }
    free(filename);
    
    return curID;
}

/**************** prompt ****************/
// print "Query?" prompt

static void prompt(void) {
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}

/**************** printDivider ****************/
// print visual divider

static void printDivider(void) {
    printf("-----------------------------------------------\n");
}
