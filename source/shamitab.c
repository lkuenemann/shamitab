/* Shamitab alpha
 * Luc Kuenemann
 *
 * Main module
 */

#include "view.h"
#include <stdio.h>
#include <string.h>


// Global constants definition
const int SYM_SIZE = 4;	// 3mt symbol size in bytes
const symbol EOF_SYM = 0xffffffff;	// 3mt end of file symbol
const symbol MAGIC_SYM = 0x334d5421;	// 3mt file magic number


void help()
{
		printf("Usage: shamitab [command] [files]\n\n"
			"Available commands:\n\n"
			"  view [tab.3mt]"
			"			Displays specified tab in ASCII format.\n"
			"  convert [ascii.txt] [tab.3mt]"
			"		Converts ASCII tab into shamitab format.\n"
			"\nExample:\n"
			"  shamitab view ringo-bushi.3mt"
			"		Displays the ringo-bushi.3mt tablature.\n"
			);
	return;
}


int main(int argc, char* argv[])
{

	// Constants
	const int nargs = 1;	// Minimum number of arguments expected


	// Variables
	char* command = NULL;
	char* filename = NULL;
	int err = 0;	// Error return variable


	// Check if right number of arguments recieved
	if(argc <= nargs)
	{
		printf("Error: expected at least %d argument(s) but got %d.\n", nargs, argc-1);
		return 1;
	}

	// Parsing arguments
	command = argv[1];
	filename = argv[2];

	// Help routine
	if(!strcmp(command, "help") || !strcmp(command, "h"))
	{
		help();
	}

	// Viewing routine
	else if(!strcmp(command, "view") || !strcmp(command,"v"))
	{
		// Call viewing routine
		err = view(filename);
		if(err) return 1;
	}

	// Conversion routine
	else if(!strcmp(command, "convert") || !strcmp(command, "c"))
	{
		printf("Not implemented yet.\n");
	}

	// Undefined command
	else
	{
		printf("Error: unknown command. To see available commands: shamitab help.\n");
		return 1;
	}

 	return 0;
}

