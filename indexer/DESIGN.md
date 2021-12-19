# CS50 Lab 5
## TSE Indexer Design Spec
## CS50 Winter 2021
Joshua Pfefferkorn

### User interface

The indexers's only interface with the user is on the command-line; it must always have two arguments.

```bash
$ indexer pageDirectory indexFilename
```

- `pageDirectory` is the pathname of a directory produced by the craweler
- `maxDepth`  is the pathname of a file into which the index should be written; the indexer creates the file (if needed) and overwrites the file (if it already exists)

### Inputs and outputs

*Input:* The inputs are command-line parameters and the contents of the directory specified by the first command-line parameter.

*Output:* Indexer will build an inverted-index data structure mapping from words to (`documentID`, `count`) pairs, wherein each `count` represents the number of occurrences of the given word in the given document. It will then create a file indexFilename and write the index to that file.

Within the file, indexer will write the index in the following format:

* one line per word, one word per line
* each line provides the word and one or more (docID, count) pairs, in the format
    * word docID count [docID count]...
* where word is a string of lower-case letters,
* where docID is a positive non-zero integer,
* where count is a positive non-zero integer,
* where the word and integers are separated by spaces.

### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules
 2. *indexer*, which initializes the index and iterates over words in html retrieved by crawler
 3. *NormalizeWord*, which converts words to lowercase
 4. *addWordToIndex*, which adds a word to the index or increments the counter for the current document if it is already present
 5. *addWordToIndex*, which prints the index to a specified file
 6. *readIndexFromFile*, which stores a printed index back into the data structure

And some helper modules that provide data structures:

  1. ***hashtable*** of words as keys and counter sets as items
  2. ***counters*** of docIDs as keys and appearance numbers as counts

### Pseudo code for logic/algorithmic flow

The indexer will run as follows:

1. starts from a command line as shown in the User Interface
2. parses the command line, validates parameters, initializes other modules
3. create a ***hashtable*** to hold the words and counters
4. iterate over files in the `pageDirectory`, creating a webpage for each one
5. loop over words in the webpage, calling *NormalizeWord* on each one

	5.1 *NormalizeWord* loops over each char in the string, converting each to lowercase
6. call *addWordToIndex* on each word

	6.1 *addWordToIndex* checks if the word is in the index

		6.1.1 if it is not yet in the index, creates a new counter for it
		6.1.2 if it is already in the index, increments the counter at the current id
7. call *writeIndexToFile* once all words have been added

	7.1 *writeIndexToFile* takes advantage of the iterate functions of ***hashtable*** and ***counters*** to print them in the specified format
 
indextest will run as follows:

1. starts from a command line as shown in the User Interface
2. parses the command line, validates parameters, calls *readIndexFromFile*

	2.1 *readIndexFromFile* creates an index the size of the number of lines in the file
	2.2 creates a counter set for each word in the file
	2.3 scans pairs of id and count pairs and sets the counters accordingly
	2.4 inserts the counter into the hashtable
3. calls *writeIndexToFile*

	3.2 *writeIndexToFile* takes advantage of the iterate functions of ***hashtable*** and ***counters*** to print them in the specified format

### Dataflow through modules

indexer:

1. *main* parses parameters and passes them to the indexer.
2. *indexer* creates a ***hashtable*** to hold words and counters and calls *NormalizeWord* and *addWordToIndex* on each word before calling *writeIndexToFile*
3. *NormalizeWord* converts words to lowercase
4. *addWordToIndex* adds a word to the index or increments its counter 
5. *writeIndexToFile* prints the index into a file

 indextest:
1. *main* parses anc validates parameters before calling *readIndexFromFile* and *writeIndexToFile*
2. *readIndexFromFile* stores printed a index into the data structure
3. *writeIndexToFile* prints the index into a file

### Major data structures

Two helper modules provide data structures:

 1. ***hashtable*** of words as keys and counter sets as items
 2. ***counters*** of docIDs as keys and appearance numbers as counts

### Testing plan

A sampling of tests that should be run:

-  Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

-  Test the indexer with a `pageDirectory` that doesn't exist.

-  Test the indexer with a `pageDirectory` that wasn't produced by crawler.

-  Test the indexer with a `pageDirectory` that isn't readable

1. Index several directories of sample crawler output files: data0, data1, data2, and data4

2. Run indextest on the produced index files, sorting both and then checking for differences
