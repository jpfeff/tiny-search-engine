# CS50 Lab 6
## TSE Querier
## CS50 Winter 2021
Joshua Pfefferkorn

### Description

The TSE Querier is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Querier, and answers search queries submitted via stdin.

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
`make valgrind` is designed to run valgrind on a brief test case that utilizes all the functions of querier
See `valgrind.out` for details of valgrind testing

To clean up, simply `make clean`

### Notes
testquery1 and testquery2 are used by testing.sh
testquery3 contains a few simple test cases for valgrind testing
