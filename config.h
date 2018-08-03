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

#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>

#define X_MAX_SPEED						4000 // mm/min
#define Y_MAX_SPEED						4000 // mm/min
#define Z_MAX_SPEED						1000 // mm/min
#define E_MAX_SPEED						1000 // mm/min

#define X_ACCEL							3000.0f // mm/s2
#define Y_ACCEL							3000.0f // mm/s2
#define Z_ACCEL							1000.0f // mm/s2
#define E_ACCEL							100.0f // mm/s2

#define X_MAX_JERK						42000 // mm/s
#define Y_MAX_JERK						42000 // mm/s
#define Z_MAX_JERK						42000 // mm/s
#define E_MAX_JERK						42000 // mm/s

#define N_AXIS							4
#define X_AXIS							0
#define Y_AXIS							1
#define Z_AXIS							2
#define E_AXIS							3

#define NO_TRAVEL						0.001f

#define FLOAT_EP						0.001

#define FILEBUFFER						200
#define LINEBUFFER						200

#define bit(x)							(1 << x)
#define bit_true(x,mask)				(x) |= (mask)
#define bit_false(x,mask)				(x) &= ~(mask)
#define bit_istrue(x,mask)				((x & mask) != 0)
#define bit_isfalse(x,mask)				((x & mask) == 0)
#define max(a,b)						(((a) > (b)) ? (a) : (b))
#define min(a,b)						(((a) < (b)) ? (a) : (b))
#define MM_PER_INCH						(25.40f)
#define INCH_PER_MM						(0.0393701f)

#define LINE_FLAG_NONE					bit(0)
#define LINE_FLAG_COMMENT_PARENTHESES 	bit(1)
#define LINE_FLAG_COMMENT_SEMICOLON 	bit(2)
#define LINE_FLAG_COMMENT_PERCENT		bit(3)
#define LINE_FLAG_COMMENT_SLASH			bit(4)
#define LINE_FLAG_COMMENT_HASH			bit(5)
#define LINE_FLAG_OVERFLOW 				bit(6)
#define MAX_INT_DIGITS 					8 // Maximum number of digits in int32 (and float)

bool Float_Equal(float a, float b, float epsilon);

#endif // CONFIG_H
