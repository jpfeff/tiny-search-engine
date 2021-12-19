/* 
 * word.c - implements NormalizeWord
 *
 * see README.md for more information
 *
 * Joshua Pfefferkorn, CS50 Winter 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "word.h"

/**************** NormalizeWord ****************/
// converts all letters in a word to lowercase

char* NormalizeWord(char *word) {
    if (word != NULL) {
        for(int k = 0; word[k] != '\0'; k++) {
            word[k] = tolower(word[k]);
        }
    }
    return word;
}