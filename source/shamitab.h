/* Shamitab alpha
 * Luc Kuenemann
 */


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define DEBUG 1

typedef int32_t symbol;	// Defining a 32 bits type for a 33t symbol
const int symbol_size = 4;
const symbol eof = 0xffffffff;

typedef struct CHARLIST {
	char c;
	struct CHARLIST* next;
} charlist;


int main(int argc, char* argv[]);

//int decode();
int decode(symbol sym, char* ascii_sym);

