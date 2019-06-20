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

char** parse(char** line, size_t* allocated_size, FILE* in_file, size_t* length_line, char** column);

int encode(symbol* sym, char** column, size_t column_size, size_t column_pos);

#endif
