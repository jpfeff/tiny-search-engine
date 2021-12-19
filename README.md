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
