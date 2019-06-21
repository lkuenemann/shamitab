/* Shamitab alpha
 * Luc Kuenemann
 *
 * Global header for all constants and general types declarations
 */

#ifndef SHAMITAB_H
#define SHAMITAB_H

#include <stdint.h>

#define DEBUG 1

// Types
typedef uint32_t symbol;	// Defining an unsigned 32 bits type for a 3mt symbol

// Constants
extern const int SYM_SIZE;	// Size of a 3mt symbol in bytes
extern const symbol EOF_SYM;	// 3mt end of file symbol
extern const symbol MAGIC_SYM;	// 3mt file magic number

#endif
