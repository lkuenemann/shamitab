/* Shamitab
 * Luc Kuenemann
 *
 * Shamitab files ASCII viewing module
 */

#ifndef VIEW_H
#define VIEW_H

#include "shamitab.h"

int view(char* filename, int bars_per_line);

int decode(char* ascii_sym, symbol sym);

#endif
