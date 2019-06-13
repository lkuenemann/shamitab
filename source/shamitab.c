/* Shamitab alpha
 * Luc Kuenemann
 *
 * Main module
 */

#include "view.h"
#include <stdio.h>

// Global constants definition
const int SYM_SIZE = 4;	// 3mt symbol size in bytes
const symbol EOF_SYM = 0xffffffff;	// 3mt end of file symbol
const symbol MAGIC_SYM = 0x334d5421;	// 3mt file magic number


int main(int argc, char* argv[])
{

	// Constants
	const int nargs = 1;	// Total number of arguments expected


	// Variables
	char* filename = NULL;
	int err = 0;	// Error return variable


	// Check if right number of arguments recieved
	if(argc != nargs+1)
	{
		printf("Error: expected %d argument(s) but got %d.\n", nargs, argc-1);
		return 1;
	}
	else
	{
		// Parsing arguments
		filename = argv[1];
	}
	
	// Call viewing routine
	err = view(filename);
	if(err) return 1;

 	return 0;
}

