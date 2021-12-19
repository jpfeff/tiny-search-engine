# Joshua Pfefferkorn
## CS50 Winter 2021, Tiny Search Engine

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
