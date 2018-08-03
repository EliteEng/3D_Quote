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

#ifndef GCODE_H
#define GCODE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"

struct Positions {
	float travel_dist;
	float max_time;
	float feed_rate;
	float cur_pos[N_AXIS]; // store current position
	float new_pos[N_AXIS]; // store new position
	float travel[N_AXIS]; // distance to travel on axis
	float rate[N_AXIS]; // speed per axis mm/min
	float accel[N_AXIS];
	float time[N_AXIS];
} Pos;


void calc_travel(struct Positions *data);
void calc_rate(struct Positions *data);
void calc_time(struct Positions *data);
void print_distances(void);
void gcodeInit(void);
void gcodeOut(FILE *file, char *line);

#endif // GCODE_H
