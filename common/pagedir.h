/* 
 * pagedir.h - header file for CS50 'pagedir' module
 *
 * Joshua Pfefferkorn, CS50 Winter 2021
 */

#include <stdio.h>
#include "../libcs50/webpage.h"

// outputs a page to the the appropriate file
void pagesaver(webpage_t *webpage, char *pageDirectory, int id);

// verifies that directory is produced by crawler
bool isCrawlerDirectory(char *dir);