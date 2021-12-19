# CS50 Lab 6
## TSE Querier Implementation Spec
## CS50 Winter 2021
Joshua Pfefferkorn

### Data Structures

Querier relies on `hashtable` and `counters` in order to build an index of words based off an querier-produced file and retrieve a list of documents using iteration and the unions and intersections of `counters`

Querier also relies on two internal data structures:
* typedef struct `twocounters` to iterate over multiple counter sets
* typedef struct `doc` to hold documentIDs and scores

### Modules and Parameters

```c
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
```

### Data Flow

`main`
* validates arguments
    * checks for 2 arguments
    * stores arguments into strings
    * calls `isCrawlerDirectory` on pageDirectory
        * `isCrawlerDirectory` verifies that pageDirectory exists, is readable, and was produced by crawler
    * attempts to open indexFilename to read
* calls `querier`

`querier`
* creates `hashtable` to store index contents
* prompts user for query
* creates a string cur to hold current query
* while there are still queries to read
    * calls `validateQueryCharacters` on cur
        * `validateQueryCharacters` returns false if there are any invalid characters in the query 
        * rejects if false and prompt again
    * stores result of `countWords` on cur into numwords
        * `countWords` returns the number of words in the query
    * creates a an array of strings called words of size numwords
    * calls `validateQueryWords` on words
        * `validateQueryCharacters` returns false if there is an invalid order of "and" and "or" in the query
        * rejects if false and prompt again
    * prints cleaned up query for user to see
    * stores result of `getScores` in new `counters` scores
        * `getScores` uses `counters_union`, `combine_counters`, and `counters_intersect` to retrieve a counter set of document IDs and their respective scores
    * passes `numCounters` into counters_iterate to determine number of documents
    * if no matching documents
        * prints a descriptive message and prompts again
    * retrieves the total number of files in pageDirectory using `countFiles`
    * ranks the docs by score in decending order by calling `rankDocs`, which incorporates `qsort_helper` 
    * iterates over documents
        * prints documents along withe their documentIDs, urls and, scores in a specified format
        * frees strings used to store filname and url
    * frees string that holds current query
    * frees the set of `doc`s 
    * frees the `counters` used to store scores
    * prompts again
* frees cur

### Error Handling and Recovery

Exits with non-zero error code if:
* Any arguments are invalid