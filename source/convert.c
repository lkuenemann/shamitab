/* Shamitab
 * ASCII files conversion module
 *
 * Luc Kuenemann
 */

#include "convert.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


int convert(char* in_filename, char* out_filename)
{
	// Constants

	// Variables
	int err = 0;
	FILE* in_file = NULL;
	FILE* out_file = NULL;
	symbol* sym = NULL;
	char** column = NULL;
	char* line[7];
	size_t length_line[7];
	size_t allocated_size[7];
	size_t column_size = 0;
	size_t column_pos = 0;
	for(int i=0; i<7; i++) 
	{
		line[i] = NULL;
		length_line[i] = 0;
		allocated_size[i] = 0;
	}

	if(DEBUG) printf("Converting %s to %s.\n", in_filename, out_filename);

	// Opening files
	in_file = fopen(in_filename, "r");
	if(in_file == NULL)
	{
		printf("Error: could not open file %s.\n", in_filename);
		return 1;
	}
	out_file = fopen(out_filename, "w");
	if(out_file == NULL)
	{
		printf("Error: could not open file %s.\n", in_filename);
		return 1;
	}
	if(DEBUG) printf("Files open!\n");
	
	// Parsing input ASCII file
	column = parse(line, allocated_size, in_file, length_line, column);
	if(column == NULL)
	{
		printf("Error: could not parse file %s\n", in_filename);
		return 1;
	}
	column_size = length_line[0]-1;

	// Initializing write
	if(DEBUG) printf("Writing magic symbol\n");
	sym = malloc(sizeof(symbol));
	*sym = MAGIC_SYM;
	fwrite(sym, sizeof(symbol), 1, out_file);

	// Iterate on columns
	while(column_pos<column_size)
	{
		// Encode symbol
		err = encode(sym, column, column_size, column_pos);
		if(err>0) // No skipping
		{
			// Write symbol
			if(DEBUG) printf("Symbol encoded to %08x\n", *sym);
			fwrite(sym, sizeof(symbol), 1, out_file);
			// Increment position by number of columns read
			if(DEBUG) printf("Incrementing position by %d\n", err);
			column_pos += err;
		}
		else // Skipping
		{
			// Increment position by 1
			if(DEBUG) printf("Skipping column\n");
			column_pos++;
		}
	}

	// Finalize writing
	if(DEBUG) printf("Writing EOF\n");
	*sym = EOF_SYM;
	fwrite(sym, sizeof(symbol), 1, out_file);

	// Closing files
	fclose(in_file);
	fclose(out_file);

	// Memory clean-up
	if(sym) free(sym);

	return 0;
}


char** parse(char** line, size_t* allocated_size, FILE* in_file, size_t* length_line, char** column)
{
	// Variables
	size_t length = 0;

	// Read 7 lines
	for(int i=0; i<7; i++)
	{
		length_line[i] = getline(&line[i], &allocated_size[i], in_file);
		if(DEBUG) printf("Line read: %s\n", line[i]);
	}
	if(DEBUG) printf("Input file read!\n");
	// Check lengths are equal
	length = length_line[0];
	for(int i=1; i<7; i++)
	{
		if(length_line[i] != length)
		{
			printf("Error: is not a proper ASCII tab.\n");
			return NULL;
		}
	}
	if(DEBUG) printf("Lines of equal length: %ld\n", length);
	// Allocate array of columns of 7 characters
	column = malloc((length-1)*sizeof(char*)); // Ignore last character (end of line)
	for(int j=0; j<length-1; j++) // Skip last character (end of line)
	{
		// Allocate each column
		column[j] = malloc(8*sizeof(char)); // 7 characters + end of string = 8 characters
		// Fill the 7 characters
		for(int i=0; i<7; i++)
		{
			column[j][i] = *(line[i]+j);
		}
		// Add end of string character
		column[j][7] = '\0';
		if(DEBUG) printf("Column written is: %s\n", column[j]);
	}

	// Local clean-up
	for(int i=0; i<7; i++)
	{
		free(line[i]); // Deallocate lines allocated by getline(), we're done using them
		line[i] = NULL; // Prevent chaos
		allocated_size[i] = 0; // Change allocated memory size to 0 accordingly
	}

	return column;
}

int encode(symbol* sym, char** column, size_t column_size, size_t column_pos)
{
	// Variables
	char char1 = 0;
	char char2 = 0;
	bool double_sym = false;
	size_t pos = column_pos;

	if(DEBUG) printf("Encoding column %ld out of %ld\n", column_pos, column_size-1);

	// Filler
	if(!strcmp(column[pos], " - - - ")) return -1; // Skipping

	// Special symbols
	// Bar, double bar, and left repeat
	if(!strcmp(column[pos], " ||||| "))
	{
		// Double or triple symbol
		if( (pos+1<column_size) && (!strcmp(column[pos+1], " ||||| ")) )
		{
			// Left repeat symbol
			if( (pos+2<column_size) && (!strcmp(column[pos+2], " -.-.- ")) )
			{
				*sym = 0x03000000; // Left repeat symbol
				return 3;
			}
			// Double symbol, has to be a double bar
			else
			{
				*sym = 0x02000000; // Double bar
				return 2;
			}
		}
		// Single symbol, has to be a bar
		else
		{
			*sym = 0x01000000; // Bar
			return 1;
		}
	}
	// Right repeat symbol
	else if(!strcmp(column[pos], " -.-.- "))
	{
		*sym = 0x04000000; // Right repeat symbol
		return 3;
	}
	// Silence
	else if(column[pos][3] == 'x')
	{
		// Check duration
		switch(column[pos][4])
		{
			case '-': // 1 beat
				*sym = 0x40000000;
				break;
			case '=': // 1/2 beat
				*sym = 0x60000000;
				break;
			default: // 2 beats
				*sym = 0x20000000;
				break;
		}
		return 1;
	}
	
	// Normal symbols
	else
	{
		if(DEBUG) printf("It's a normal symbol\n");
		*sym = 0x0;

		// Check 3rd string for a note
		char1 = column[pos][1];
		if(char1 >= '0' && char1 <= '9') // There's a position marker
		{
			*sym += 0x00000020; // Change G3 to 1
			if(pos+1 < column_size) // Would a double column symbol overflow?
			{
				char2 = column[pos+1][1];
			}
			if(char2 >= '0' && char2 <= '9') // Double column symbol
			{
				*sym += (uint32_t) 10*(char1-'0')+(char2-'0'); // Change H3 to position
				if(DEBUG) printf("Got double symbol %u\n", (uint32_t) 10*(char1-'0')+(char2-'0'));
				double_sym = true;
			}
			else // Single column symbol
			{
				*sym += (uint32_t) (char1-'0'); // Change H3 to position
				if(DEBUG) printf("Got single symbol %u\n", (uint32_t) (char1-'0'));
			}
		}

		// Check 2nd string for a note
		char1 = column[pos][3];
		if(char1 >= '0' && char1 <= '9') // There's a position marker
		{
			*sym += 0x00000800; // Change G2 to 1
			if(pos+1 < column_size) // Would a double column symbol overflow?
			{
				char2 = column[pos+1][3];
			}
			if(char2 >= '0' && char2 <= '9') // Double column symbol
			{
				*sym += ((uint32_t) 10*(char1-'0')+(char2-'0')) << 6; // Change H2 to position
				if(DEBUG) printf("Got double symbol %u\n", (uint32_t) 10*(char1-'0')+(char2-'0'));
				double_sym = true;
			}
			else // Single column symbol
			{
				*sym += ((uint32_t) (char1-'0')) << 6; // Change H2 to position
				if(DEBUG) printf("Got single symbol %u\n", (uint32_t) (char1-'0'));
			}
		}

		// Check 1st string for a note
		char1 = column[pos][5];
		if(char1 >= '0' && char1 <= '9') // There's a position marker
		{
			*sym += 0x00020000; // Change G1 to 1
			if(pos+1 < column_size) // Would a double column symbol overflow?
			{
				char2 = column[pos+1][5];
			}
			if(char2 >= '0' && char2 <= '9') // Double column symbol
			{
				*sym += ((uint32_t) 10*(char1-'0')+(char2-'0')) << 12; // Change H1 to position
				if(DEBUG) printf("Got double symbol %u\n", (uint32_t) 10*(char1-'0')+(char2-'0'));
				double_sym = true;
			}
			else // Single column symbol
			{
				*sym += ((uint32_t) (char1-'0')) << 12; // Change H1 to position
				if(DEBUG) printf("Got single symbol %u\n", (uint32_t) (char1-'0'));
			}
		}

		// Now we know if it's a single or double column symbol
		if(double_sym) pos++; // Always check second column if it is double
		if(DEBUG) printf("column_pos is %ld and we check pos %ld\n", column_pos, pos);

		// Check duration marker
		// 1 beat
		if(column[pos][6] == '-' || column[pos][4] == '-' || column[pos][2] == '-') { *sym += 0x40000000; if(DEBUG) printf("1 beat\n"); }
		// 1/2 beat
		else if(column[pos][6] == '=' || column[pos][4] == '=' || column[pos][2] == '=') { *sym += 0x60000000; if(DEBUG) printf("1/2 beat\n"); }
		// Everything else is 2 beats
		else { *sym += 0x20000000; if(DEBUG) printf("2 beats\n"); }

		// Check effect/slide/finger
		if(DEBUG) printf("Switching on marker: %c\n", column[pos][0]);
		switch(column[pos][0])
		{
			case '^':
				*sym += 0x08000000;
				break;
			case 'h':
				*sym += 0x01000000;
				break;
			case 'u':
				*sym += 0x02000000;
				break;
			case 's':
				*sym += 0x03000000;
				break;
			case '/':
				*sym += 0x04000000;
				break;
			case '1':
				*sym += 0x00100000;
				break;
			case '2':
				*sym += 0x00200000;
				break;
			case '3':
				*sym += 0x00300000;
				break;
			case '4':
				*sym += 0x00400000;
				break;
			default:
				break;
		} 

		// Check triplet

		if(double_sym){ return 2; }
		else{ return 1; }
	}

	// Should not end up here
	return 0;
}
