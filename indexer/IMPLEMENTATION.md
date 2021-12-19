# CS50 Lab 5
## TSE Indexer Implementation Spec
## CS50 Winter 2021
Joshua Pfefferkorn

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