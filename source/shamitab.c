/* Shamitab alpha
 * Luc Kuenemann
 */


#include "shamitab.h"


int main(int argc, char* argv[])
{
	// Constants
	const int nargs = 1;	// Total number of arguments expected
	const int bsize = 256;	// Number of symbols read at once

	// Variables
	const char* filename = NULL;
	FILE* file = NULL;
	int err;	// Error return variable
	symbol buffer[1];

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

	if(DEBUG) printf("Opening file %s...\n", filename);

	// Open 3mt tab file
	file = fopen(filename, "r");
	if(DEBUG) printf("File open!\n");
	// Read first symbol
	fread(buffer, symbol_size, 1, file);
	// Loop while not EOF
	while(buffer[0]!=eof)
	{
		// Decode symbol
		if(DEBUG) printf("Read symbol: %08x\n", buffer[0]);
		err = decode();
		if(err)
		{
			printf("Error: cannot decode symbol.\n");
			return 1;
		}
		if(DEBUG) printf("Symbol decoded!\n");
		// Read next symbol
		fread(buffer, symbol_size, 1, file);
	}

	// Cleaning up
	fclose(file);

 	return 0;
}


int decode()
//int decode(symbol sym, char* ascii)
{
 	return 0;
}
