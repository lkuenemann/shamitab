/* Shamitab alpha
 * Luc Kuenemann
 */


#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define DEBUG 1

typedef int32_t symbol;	// Defining a 32 bits type for a 33t symbol
const int symbol_size = 4;
const symbol eof = 0xffffffff;

typedef struct CHARLIST {
	char c;
	struct CHARLIST* next;
} charlist;


int main(int argc, char* argv[]);

char* decode(char* ascii_sym, symbol sym);

void freelist(charlist* head);

charlist* initializelist(charlist* head, char c);

charlist* appendlist(charlist* tail, char c);
