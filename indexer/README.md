# CS50 Lab 5
## TSE Indexer
## CS50 Winter 2021
Joshua Pfefferkorn

### Description

The TSE indexer is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the index tester, loads an index file produced by the indexer and saves it to another file.

### Requirements

See REQUIREMENTS.md

### Design

See DESIGN.md

### Implementation

See IMPLEMENTATION.md

### Testing

To test, simply `make test`
See `testing.out` for testing output

To test with valgrind, `make valgrind`
`make valgrind` is designed to run valgrind on a brief test case that utilizes all the functions of indexer
See `valgrind.out` for details of valgrind testing

To clean up, simply `make clean`