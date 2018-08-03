/*      
 * 3D Quote by Elite Engineering WA
 *                                           
 * This file is part of the 3D Quote distribution (https://github.com/EliteEng/3D_Quote).
 * Copyright (c) 2018 Rob Brown - Elite Engineering WA.
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "gcode.h"

const char build_version[] = "V0.0.1";
const char build_date[] = "30-07-2018";

int main(int argc, char *argv[])
{
	char *inputf = argv[1];
	
	setbuf(stdout, NULL);

	printf("Elite Engineering WA\n");
	printf("3D Printing Quoter\n");
	printf("%s - %s\n", build_version, build_date);
	printf("-------------------------------------------------\n");

	gcodeInit();

	if (argc == 2) {

		char infile[FILEBUFFER];
		char outfile[FILEBUFFER];
		char lineBuffer[LINEBUFFER];
		
		char *dotptr;

		strcpy(infile, inputf);
		strcpy(outfile, inputf);
		
		dotptr = strrchr (outfile, '.');
		*dotptr= '\0';
    
		strcat(outfile, ".txt");

		printf("Processing File - input= %s - output= %s\n", infile, outfile);

		FILE *gcode, *output;

		gcode = fopen(infile, "r");
		output = fopen(outfile, "w");		

		uint8_t line_flags = 0;
		uint8_t char_counter = 0;
		uint8_t c = fgetc(gcode);

		while (!feof(gcode)) {
			if ((c == '\n') || (c == '\r')) { // End of line reached
				if (lineBuffer[0] == 0) {
					// Empty or comment line. For syncing purposes.
				}
				else {
					// Parse and execute g-code block.
					lineBuffer[char_counter] = '\0';
					gcodeOut(output, lineBuffer);
				}
				// Reset tracking data for next line.
				line_flags = 0;
				char_counter = 0;
			}
			else {
				if (line_flags) {
					// Throw away all (except EOL) comment characters and overflow characters.
					if (c == ')') {
						// End of '()' comment. Resume line allowed.
						if (line_flags & LINE_FLAG_COMMENT_PARENTHESES) { line_flags &= ~(LINE_FLAG_COMMENT_PARENTHESES); }
					}
				}
				else {
					if (c == '(') {
						line_flags |= LINE_FLAG_COMMENT_PARENTHESES;
					}
					else if (char_counter >= (LINEBUFFER - 1)) {
						// Detect line buffer overflow and set flag.
						line_flags |= LINE_FLAG_OVERFLOW;
					}
					else {
						lineBuffer[char_counter++] = c;
					}
				}
			}
			c = fgetc(gcode);
		}

		print_distances();
		
		fclose(gcode);
		fclose(output);
	}
	else if (argc > 2) {
		printf("Too many arguments supplied.\n");
	}
	else {
		printf("One argument expected.\n");
	}

	return 0;
}

