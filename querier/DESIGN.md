# CS50 Lab 6
## TSE Querier Design Spec
## CS50 Winter 2021
Joshua Pfefferkorn

### User interface

The indexers's only interface with the user is on the command-line; it must always have two arguments.

```bash
$ querier pageDirectory indexFilename
```

- `pageDirectory` is the pathname of a directory produced by the craweler
- `maxDepth`  is the pathname of a file written by indexer

### Inputs and outputs

*Input:* The inputs are command-line parameters and the contents of the directory and file specified by them

*Output:* Querier prints a list of matching documents (or an error message if the query is invalid) in the following format:

* score [score] doc [documentID]: [url]

### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules
 2. *querier*, which prints a ranked list of documents that match the query
 3. *validateQueryCharacters*, which catches any invalid characters in the query 
 4. *countWords*, which returns the number of words in query
 5. *splitWordsIntoArray*, which stores words (tokens separated by spaces) into array
 6. *validateQueryWords*, which catches invalid order of "and" and "or"
 7. *getScores*, which retrieves a counter set of document IDs and their respective scores
 8. *counters_intersect*, which finds intersection between counter sets
 9. *counters_intersect_helper*, a helper method for counters_intersect
 10. *counters_union*, used in conjunction with combine_counters to find the union between counter sets
 11. *counters_union_helper*, a helper method for counters_union
 12. *combine_counters*, used in conjunction with counters_union to find the union between counter sets
 13. *combine_counters_helper*, a helper method for combine_counters
 14. *numCounters*, which retrieves the number of counters in a counter set
 15. *rankDocs*, which sorts the documents into decending order by score
 16. *qsort_helper*, a helper method for qsort function used in rankDocs
 17. *countFiles*, which counts the number of files in a directory
 18. *fileno*, necessary for prompt
 19. *prompt*, which prints "Query?" prompt
 20. *printDivider*, which print a visual divider

And some helper modules that provide data structures:

  1. ***hashtable*** of words as keys and counter sets as items
  2. ***counters*** of docIDs as keys and appearance numbers as counts

### Pseudo code for logic/algorithmic flow

querier will run as follows:

1. starts from a command line as shown in the User Interface
2. parses the command line, validates parameters, initializes other modules
3. create a ***hashtable*** to hold the words and counters
4. prompts for user input
5. while there is still input to read

	5.1 call *validateQueryCharacters* to ensure query has valid characters

	5.2 store words into an array by calling *splitWordsIntoArray*

	5.3 validate the and/or logic by calling *validateQueryWords*

	5.4 print the cleaned query

	5.5 create a ***counters*** set to hold the documents and their scores

	5.6 call *getScores* to retrieve a counter set of document IDs and their respective scores

	5.7 get the number of documents by calling counters_iterate with *numCounters*

	5.8 print a message if no documents match
	
	5.9 sort matching documents by calling *rankDocs*
	
	5.10 print list of matching documents in decending order by score
	
	5.11 free strings used to hold filename and url
6. free query string and data structures

### Dataflow through modules

1. *main* parses parameters and passes them to the indexer.
2. *querier* creates a ***hashtable*** to hold words and counters and calls *validateQueryCharacters*, *splitWordsIntoArray*, and *validateQueryWords* on the query; it then calls *getScores* and passes *numCounters* into counters_iterate
3. these modules rely on helper functions to perform operations as described in the "Functional decomposition into modules" section

### Major data structures

Two helper modules provide data structures:

 1. ***hashtable*** of words as keys and counter sets as items
 2. ***counters*** of docIDs as keys and appearance numbers as counts

Two internal modules also provide data structures:
 1. ***twocounters***, which holds two ***counters***
 2. ***doc***, which holds an id and a score, both ints

### Testing plan

A sampling of tests that should be run:

-  Test the program with a file containing various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.

- Test the program with a file containing various valid arguments

- Test the program using fuzzquery.c to generate random queries