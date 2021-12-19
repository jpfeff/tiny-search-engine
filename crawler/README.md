# CS50 Lab 4
## TSE Crawler
## CS50 Winter 2021
Joshua Pfefferkorn

### Description

The TSE crawler is a standalone program that crawls the web and retrieves webpages starting from a “seed” URL. It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages, recursively, but limiting its exploration to a given “depth”.

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
`make valgrind` is designed to run valgrind on a brief test case that utilizes all the functions of crawler
See `valgrind.out` for details of valgrind testing