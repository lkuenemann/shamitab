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
	char ascii_sym[22];

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
		err = decode(buffer[0], ascii_sym);
		if(err)
		{
			printf("Error: cannot decode symbol.\n");
			return 1;
		}
		if(DEBUG) printf("Symbol decoded!\n\n");
		// Read next symbol
		fread(buffer, symbol_size, 1, file);
	}

	// Cleaning up
	fclose(file);

 	return 0;
}


//int decode()
int decode(symbol sym, char* ascii_sym)
{
	// Special symbols
	if( ( sym & 0x00020820 ) == 0 )
	{
		if(DEBUG) printf("I am special!\n");
		// We only need 3 most significant bits from the symbol
		// Extracting them by shifting 29 bits to the right
		char a = sym >> 29;
		// Then switching to find out which special symbol it is
		switch(a)
		{
			// Silence
			case 0:
				if(DEBUG) printf(">>Silence\n");
				ascii_sym = " - x - ";
				if(DEBUG) printf("%s\n", ascii_sym);
				break;
			// Bar
			case 1:
				if(DEBUG) printf(">>Bar\n");
				ascii_sym = " ||||| ";
				if(DEBUG) printf("%s\n", ascii_sym);
				break;
			// Double bar
			case 2:
				if(DEBUG) printf(">>Double bar\n");
				ascii_sym = " |||||  ||||| ";
				break;
			// Left repeat sign
			case 3:
				if(DEBUG) printf(">>Left repeat sign\n");
				ascii_sym = " |||||  |||||  -.-.- ";
				break;
			// Bar
			case 4:
				if(DEBUG) printf(">>Right repeat sign\n");
				ascii_sym = " -.-.-  |||||  ||||| ";
				break;
			// Default: undefined
			default:
				printf("Error: Undefined symbol.\n");
				return 1;
		}
	}	
	// General case
	else
	{
		if(DEBUG) printf("I am normal.\n");
	}

 	return 0;
}
