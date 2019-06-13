/* Shamitab alpha
 * Luc Kuenemann
 */


#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "charlist.h"

#define DEBUG 0
#define MAGIC 0x334d5421

typedef int32_t symbol;	// Defining a 32 bits type for a 3mt symbol
const int symbol_size = 4;
const symbol eof = 0xffffffff;


int main(int argc, char* argv[]);

int decode(char* ascii_sym, symbol sym);

