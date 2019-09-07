/* Shamitab
 * Shamitab files ASCII viewing module
 *
 * Luc Kuenemann
 */

#include "view.h"
#include "charlist.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>


int view(char* filename)
{
	// Variables
	int err = 0;	// Error return variable
	FILE* file = NULL;
	symbol buffer[1];
	char ascii_sym[29];	// Max length is 3 columns * 7 lines + 1 end of string character
	charlist* line_head[7];	// Table of 7 linked lists (lines) of characters
	charlist* line_tail[7];	// Table of pointers to last element on above lists
	charlist* line_iter[7];
	for(int i=0; i<7; i++)
	{
		line_head[i] = NULL;
		line_tail[i] = line_head[i];
		line_iter[i] = NULL;
	}

	if(DEBUG) printf("Opening file %s...\n", filename);

	// Open 3mt tab file
	file = fopen(filename, "r");
	if(DEBUG) printf("File open!\n");
	// Read first symbol
	fread(buffer, SYM_SIZE, 1, file);
	// Check for magic number
	if(buffer[0]!=MAGIC_SYM)
	{
		printf("Error: not a proper 3mt file!\n");
		return 1;
	}

	// Loop while not EOF
	fread(buffer, SYM_SIZE, 1, file);
	while(buffer[0]!=EOF_SYM)
	{
		// Decode symbol
		if(DEBUG) printf("Read symbol: %08x\n", buffer[0]);
		err = decode(ascii_sym, buffer[0]);
		if(err)
		{
			printf("Error: cannot decode symbol.\n");
			return 1;
		}
		if(DEBUG) printf("Symbol decoded!\n");
		if(DEBUG) printf("Got symbol: %s\n", ascii_sym);

		// Unscramble ASCII symbol
		for(int i=0; ascii_sym[i]!='\0' && i<28; i++)	// i<21 is a preventive safety measure in case ascii_sym does not end with a \0 end of string character
		{
			// If line still empty
			if(line_head[i%7]==NULL)
			{
				// Initialize the head of the line
				line_head[i%7] = initializelist(line_head[i%7], ascii_sym[i]);
				line_tail[i%7] = line_head[i%7];
			}
			else
			{
				// Append to the line
				line_tail[i%7] = appendlist(line_tail[i%7], ascii_sym[i]);
			}
		}
		// Read next symbol
		fread(buffer, SYM_SIZE, 1, file);
	}

	// Display the whole tab
	for(int i=0; i<7; i++)
	{
		line_iter[i] = line_head[i];
		while(line_iter[i]->next != NULL)
		{
			printf("%c", line_iter[i]->c);
			line_iter[i] = line_iter[i]->next;
		}
		printf("\n");
	}

	// Cleaning up
	for(int i=0; i<7; i++)
	{
		freelist(line_head[i]);
		line_head[i] = NULL;
		line_tail[i] = NULL;
		line_iter[i] = NULL;
	}
	fclose(file);

	return 0;
}


int decode(char* ascii_sym, symbol sym)
{
	// Constants
	const symbol duration_mask =	0xe0000000;
	const symbol triplet_mask = 	0x10000000;
	const symbol slide_mask = 		0x08000000;
	const symbol effect_mask = 		0x07000000;
	const symbol bachi_mask =		0x00800000;
	const symbol finger_mask = 		0x00700000;
	const symbol note1_mask = 		0x00020000;
	const symbol position1_mask = 	0x0001f000;
	const symbol note2_mask = 		0x00000800;
	const symbol position2_mask = 	0x000007c0;
	const symbol note3_mask = 		0x00000020;
	const symbol position3_mask = 	0x0000001f;
	const symbol special_mask =		0x00020820;

	//Variables
	symbol special = 0;
	char* tmp_ascii_sym = NULL;
	char effect_marker = ' ';
	char duration_marker = ' ';
	bool triplet = false;
	bool slide = false;
	bool bachi = false;
	bool note1 = false;
	bool note2 = false;
	bool note3 = false;
	int8_t duration = 0;
	int8_t effect = 0;
	int8_t finger = 0;
	int8_t position1 = 0;
	int8_t position2 = 0;
	int8_t position3 = 0;
	int8_t columns = 1;

	// Special symbols
	if( ( sym & special_mask ) == 0 )
	{
		if(DEBUG) printf("I am special!\n");
		// We only need 3 most significant bits from the symbol
		// Masking them (by precaution) then switching on all possible values
		special = sym & effect_mask;
		// Then switching to find out which special symbol it is
		switch(special)
		{
			// Silence
			case 0x00000000:
				if(DEBUG) printf(">>Silence\n");
				strcpy(ascii_sym, " - x - ");
				// Get duration
				duration = (int8_t)((sym&duration_mask)>>29);
				// Get the right duration marker
				switch(duration)
				{
					case 2:
						duration_marker = '-';
						break;
					case 3:
						duration_marker = '=';
						break;
					/*case 4:
						duration_marker = '≡'; // TODO That's not ASCII, may be unsafe...
						break;*/
					default: // Display nothing (space)
						duration_marker = ' ';
						break;
				}
				// Place duration marker under the silence symbol (middle string)
				ascii_sym[4] = duration_marker;
				break;
			// Bar
			case 0x01000000:
				if(DEBUG) printf(">>Bar\n");
				strcpy(ascii_sym, " ||||| ");
				break;
			// Double bar
			case 0x02000000:
				if(DEBUG) printf(">>Double bar\n");
				strcpy(ascii_sym, " |||||  ||||| ");
				break;
			// Left repeat sign
			case 0x03000000:
				if(DEBUG) printf(">>Left repeat sign\n");
				strcpy(ascii_sym, " |||||  |||||  -.-.- ");
				break;
			// Right repeat sign
			case 0x04000000:
				if(DEBUG) printf(">>Right repeat sign\n");
				strcpy(ascii_sym, " -.-.-  |||||  ||||| ");
				break;
			// Default: undefined
			default:
				printf("Error: Undefined symbol.\n");
				strcpy(ascii_sym, " ERROR ");
				break;
		}
		if(DEBUG) printf("%s\n", ascii_sym);
	}
	// General case
	else
	{
		if(DEBUG) printf("I am normal.\n");
		// Get duration
		duration = (int8_t)((sym&duration_mask)>>29);
		// Get triplet
		if((sym&triplet_mask)>>28) triplet = true;
		// Get slide
		if((sym&slide_mask)>>27) slide = true;
		// Get effect
		effect = (int8_t)((sym&effect_mask)>>24);
		// Get bachi position
		if((sym&bachi_mask)>>23) bachi = true;
		// Get finger annotation
		finger = (int8_t)((sym&finger_mask)>>20);
		// Get 1st string note/silence
		if((sym&note1_mask)>>17) note1 = true;
		// Get 1st string note position
		position1 = (int8_t)((sym&position1_mask)>>12);
		// Get 2nd string note/silence
		if((sym&note2_mask)>>11) note2 = true;
		// Get 2nd string note position
		position2 = (int8_t)((sym&position2_mask)>>6);
		// Get 3rd string note/silence
		if((sym&note3_mask)>>5) note3 = true;
		// Get 3rd string note position
		position3 = (int8_t)((sym&position3_mask)>>0);

		// TODO Error checking

		// Creatng the ASCII representation of the symbol
		// Check if we need 1 or 2 columns (default 1)
		if((position1>9)||(position2>9)||(position3>9))
		{
			// Create a double column symbol
			columns = 2;
			tmp_ascii_sym = malloc(15*sizeof(char));
			strcpy(tmp_ascii_sym, " - - -  - - - ");
			// Place notes
			if(note1)
			{
				if(position1>9)
				{
					// Placing the dozen digit on the first column
					tmp_ascii_sym[5] = (position1/10)+'0';
				}
				// Placing the unit digit on the second column
				tmp_ascii_sym[12] = (position1%10)+'0';
			}
			if(note2)
			{
				if(position2>9)
				{
					// Placing the dozen digit on the first column
					tmp_ascii_sym[3] = (position2/10)+'0';
				}
				// Placing the unit digit on the second column
				tmp_ascii_sym[10] = (position2%10)+'0';
			}
			if(note3)
			{
				if(position3>9)
				{
					// Placing the dozen digit on the first column
					tmp_ascii_sym[1] = (position3/10)+'0';
				}
				// Placing the unit digit on the second column
				tmp_ascii_sym[8] = (position3%10)+'0';
			}
		}
		else
		{
			// Create a single column symbol
			columns = 1;
			tmp_ascii_sym = malloc(8*sizeof(char));
			strcpy(tmp_ascii_sym, " - - - ");
			// Place notes
			if(note1) tmp_ascii_sym[5] = (position1%10)+'0';
			if(note2) tmp_ascii_sym[3] = (position2%10)+'0';
			if(note3) tmp_ascii_sym[1] = (position3%10)+'0';
		}
		// Get the right duration marker
		switch(duration)
		{
			case 2:
				duration_marker = '-';
				break;
			case 3:
				duration_marker = '=';
				break;
			/*case 4:
				duration_marker = '≡'; // TODO That's not ASCII, may be unsafe...
				break;*/
			default: // Display nothing (space)
				duration_marker = ' ';
				break;
		}
		// Place duration marker on the lowest note in the chord/symbol
		if(note1) tmp_ascii_sym[columns*7-1] = duration_marker;
		else if(note2) tmp_ascii_sym[columns*7-3] = duration_marker;
		else tmp_ascii_sym[columns*7-5] = duration_marker;
		// Place triplet marker
		if(triplet) tmp_ascii_sym[7*columns-1] = '3'; // TODO Change as it can mess with duration representation
		// Place slide marker
		if(slide) tmp_ascii_sym[7*columns-7] = '^';
		if(effect>0 && effect<5)
		{
			// Get the right effect marker
			switch(effect)
			{
				case 1: // Hajiki
					effect_marker = 'h';
					break;
				case 2: // Uchi
					effect_marker = 'u';
					break;
				case 3: // Sukui
					effect_marker = 's';
					break;
				case 4: // Suberi
					effect_marker = '/';
					break;
				default: // No effect or undefined
					effect_marker = ' ';
					break;
			}
			// Place effect marker
			tmp_ascii_sym[7*columns-7] = effect_marker;
		}
		// Maebachi representation: put an m over the symbol
		if(bachi) tmp_ascii_sym[7*columns-7] = 'm'; // TODO Change as it can override effect representation
		// Place finger marker
		if(finger>0 && finger<5) tmp_ascii_sym[7*columns-7] = finger + '0'; // TODO Change as it can override effect representation

		strcpy(ascii_sym, tmp_ascii_sym);
		
	}
	free(tmp_ascii_sym);
	// Append a tab "space" right after the symbol
	strcat(ascii_sym, " - - - ");

 	return 0;
}

