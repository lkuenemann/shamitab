/* Shamitab
 * Luc Kuenemann
 *
 * ASCII files conversion module
 */

#ifndef CONVERT_H
#define CONVERT_H

#include "shamitab.h"

#include <stdio.h>
#include <stdlib.h>

int convert(char* in_filename, char* out_filename);

char** parse(FILE* in_file, char** column, int* total_length);

int encode(symbol* sym, char** column, size_t column_size, size_t column_pos);

#endif
