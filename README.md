# Joshua Pfefferkorn
## Tiny Search Engine

## TSE Crawler Implementation Spec

### Data Structures

Crawler relies on `hashtable` (which uses `set`) and `bag` in order to crawl through the web. `bag` is used to store `webpages` of pages that have not yet been visited, while `hashtable` tracks URLs of pages that have already been visited.

### Modules and Parameters

```c
void crawler(char *seedURL, char *pageDirectory, int maxDepth);
bool pagefetcher(webpage_t *webpage);
void pagescanner(webpage_t *webpage, hashtable_t *hashtable, bag_t *bag);
// used by crawler but stored in pagedir in common library
void pagesaver(webpage_t *webpage, char *pageDirectory, int id);
inline static void logr(const char *word, const int depth, const char *url);
```

### Data Flow

`main`
* Validates arguments
    * Checks for 3 arguments
    * Checks that maxDepth is an integer and within the correct range
    * Checks that pageDirectory exists and is writeable
    * Checks that seedURL is internal
* Calls `crawler`

`crawler`
* Copies the seedURL
* Creates a `webpage` for it
    * Checks that `webpage` is not NULL
* Creates a `bag` to hold unvisited webpages
    * Checks that `bag` is not NULL
* Creates a `hashtable` to hold visited URLs
    * Size 200 * (maxDepth + 1) adjusts size for number of webpages expected to be visited in order to maximize efficiency & protects against maxDepth = 0
    * Checks that `hashtable` is not NULL
* Inserts seedURL `webpage` into `bag`
* Inserts seedURL into `hashtable`
* Document ID initialized to 1
* While there are still webpages to visit
    * Extracts `webpage` from `bag`
    * Calls `pagefetcher` on `webpage`
    * Prints message using `logr`
    * Calls `pagesaver` on `webpage`
    * Prints message using `logr`
    * Increments document ID
    * If maximum depth has not been reached
        * Calls `pagescanner` on `webpage`
    * Once `webpage` has been fetched, saved, and scanned, deletes it
* Deletes empty `bag`
* Deletes empty `hashtable`

`pagefetcher`
* Calls webpage_fetch on `webpage`

`pagescanner`
* Prints message using `logr`
* While there are still URLs on the page
    * Tries to normalize URL
    * Verifies the URL is internal
        * If URL is not already in `hashtable`
            * Creates a `webpage` for it
            * Inserts the `webpage` into `bag`
            * Prints message using `logr`
    * Otherwise prints message using `logr` if URL is not internal or is already in `hashtable`
* Frees string for current URL

`pagesaver`
* Allocates memory and converts ID int to a string
* Allocates memory to hold filename, concatenating pageDirectory and ID
* Tries to open and write `file`, exists if unsuccessful
* Writes URL `file`
* Writes depth to `file`
* Writes HTML to `file`
* Closes `file`
* Frees ID string and filename

### Error Handling and Recovery

Exits with non-zero error code if:
* Any arguments are invalid
* Out of memory

## TSE Indexer Implementation Spec

### Data Structures

Indexer relies on `hashtable` and `counters` in order to build an index of words, document IDs, and counts. `hashtable` is used to store words as keys and counters as items. `counters`, in turn, stores documents IDs and word counts within each. 

### Modules and Parameters

```c
void indexer(char *pageDirectory, char *indexFilename);
// in common directory (pagedir.c)
bool isCrawlerDirectory(char *dir);
// in common directory (word.c)
char* NormalizeWord(char *word);
// in common directory (index.c)
char* getCrawlerFilename(char *pageDirectory, int id);
void addWordToIndex(char *word, hashtable_t *index, int id);
void writeIndexToFile(hashtable_t *index, char *indexFilename);
hashtable_t* readIndexFromFile(char *indexFilename);
// used as a helper method in hashtable_delete
void counterDelete(void *item);
// used in writeIndexToFile as a helper method in hashtable_iterate
void printHashtable(void *arg, const char *key, void *item);
// used as a helper metjood in printHashtable
void printCounters(void *arg, const int key, int count);
```

### Data Flow

indexer:

`main`
* validates arguments
    * checks for 2 arguments
    * stores arguments into strings
    * calls `isCrawlerDirectory` on pageDirectory
        * `isCrawlerDirectory` verifies that pageDirectory exists, is readable, and was produced by crawler
    * writes indexFilename if it does not yet exists or overwrites if it does
* calls `indexer`

`indexer`
* creates `hashtable` to store index contents
* initializes id to 1
* calls `getCrawlerFilename`
    * `getCrawlerFilename` generates the filename of the first document in the directory (id 1)
* while there are still files in the directory
    * stores url, depth, and html as strings
    * converts depth to an int
    * creates a `webpage` from the parsed parameters
    * initializes a string for the current word and begins pos at 0
    * while there are still words in the webpage
        * if the word is at least 3 chars long
            * calls `NormalizeWord` on it
                * `NormalizeWord` converts it to lowercase
            * calls `addWordToIndex` on it
                * `addWordToIndex` adds the word to index or increments a `counter`
        * frees word
    * closes file
    * deletes webpage
    * increments id
    * gets next filename
* frees filename
* calls `writeIndexToFile`
    * `writeIndexToFile` prints the index to a file in the specified format
* deletes the `hashtable` index

indextest:

`main`
* validates arguments
    * checks for 2 arguments
    * stores arguments into strings
    * checks that oldIndexFilename is readable
    * checks that newIndexFilename is writeable
    * calls `readIndexFromFile` on oldIndexFilename
        * `readIndexFromFile` stores a printed index back into the `hashtable` data structure
    * calls `writeIndexToFile` on newFilename
        * `writeIndexToFile` prints the index to a file in the specified format
    * deletes the `hashtable` index


### Error Handling and Recovery

Exits with non-zero error code if:
* Any arguments are invalid

## TSE Querier Implementation Spec

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
