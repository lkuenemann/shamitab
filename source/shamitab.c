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
	char* ascii_sym;	// Max length is 3 columns * 7 lines + 1 end of string character
	charlist* line[7];	// Table of 7 linked lists (lines) of characters
	charlist* linelast[7];	// Table of pointers to last element on above lists
	charlist* iter[7];
	for(int i=0; i<7; i++)
	{
		line[i] = malloc(sizeof(charlist*));
		line[i]->c = '\0';
		line[i]->next = NULL;
		linelast[i] = line[i];
		iter[i] = NULL;
	}


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
		ascii_sym = decode(buffer[0]);
		if(err) 	// TODO change error checking to check for something as err isn't used here anymore
		{
			printf("Error: cannot decode symbol.\n");
			return 1;
		}
		if(DEBUG) printf("Symbol decoded!\n");
		if(DEBUG) printf("Got symbol: %s\n", ascii_sym);

		// Unscramble ASCII symbol
		for(int i=0; ascii_sym[i]!='\0' && i<21; i++)	// i<21 is a preventive safety measure in case ascii_sym does not end with a \0 end of string character
		{
			// Append character to line
			linelast[i%7]->c = ascii_sym[i];
			if(DEBUG) printf("ascii_sym[%d] is: %c\n", i, ascii_sym[i]);
			// Allocate a new element to the line
			linelast[i%7]->next = malloc(sizeof(charlist));
			// Let's point on this new element
			linelast[i%7] = linelast[i%7]->next;
			// Setting next to NULL before we mess up something
			linelast[i%7]->next = NULL;
			// Setting last character to end of string \0
			linelast[i%7]->c = '\0';
		}

		// Read next symbol
		fread(buffer, symbol_size, 1, file);
	}

	// Display the whole tab
	for(int i=0; i<7; i++)
	{
		iter[i] = line[i];
		while(iter[i]->c != '\0')
		{
			printf("%c", iter[i]->c);
			iter[i] = iter[i]->next;
			//getchar();
		}
		printf("\n");
	}

	// Cleaning up
	fclose(file);

 	return 0;
}


char* decode(symbol sym)
{
	//Variables
	char* ascii_sym = " ERROR ";

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
				break;
			// Bar
			case 1:
				if(DEBUG) printf(">>Bar\n");
				ascii_sym = " ||||| ";
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
				return ascii_sym;
		}
		if(DEBUG) printf("%s\n", ascii_sym);
	}	
	// General case
	else
	{
		if(DEBUG) printf("I am normal.\n");
	}

 	return ascii_sym;
}
