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

#include "gcode.h"

#define SWEEP

float total_dist = 0.0f;
float total_extrude = 0.0f;
float total_time = 0.0f;

void gcodeInit(void) {
    uint8_t idx;

	for (idx = 0; idx<N_AXIS; idx++) {
		Pos.cur_pos[idx] = 0.0f;
		Pos.rate[idx] = 0.0f;
	}

	Pos.feed_rate = 0.0f;

	Pos.accel[X_AXIS] = X_ACCEL;
	Pos.accel[Y_AXIS] = Y_ACCEL;
	Pos.accel[Z_AXIS] = Z_ACCEL;
	Pos.accel[E_AXIS] = E_ACCEL;
}

void calc_travel(struct Positions *data) {

	data->travel[X_AXIS] = fabs(data->new_pos[X_AXIS] - data->cur_pos[X_AXIS]);
	data->travel[Y_AXIS] = fabs(data->new_pos[Y_AXIS] - data->cur_pos[Y_AXIS]);
	data->travel[Z_AXIS] = fabs(data->new_pos[Z_AXIS] - data->cur_pos[Z_AXIS]);
	data->travel[E_AXIS] = data->new_pos[E_AXIS] - data->cur_pos[E_AXIS];

	float hyp_travel = sqrt((data->travel[X_AXIS] * data->travel[X_AXIS]) + (data->travel[Y_AXIS] * data->travel[Y_AXIS]));
	data->travel_dist = sqrt((hyp_travel * hyp_travel) + (data->travel[Z_AXIS] * data->travel[Z_AXIS]));

	return;
}

void calc_rate(struct Positions *data) {

	if (data->travel_dist < NO_TRAVEL) {
		data->rate[E_AXIS] = data->feed_rate;
	} else {
		data->rate[X_AXIS] = data->travel[X_AXIS] / (data->travel_dist / data->feed_rate); // calculate the rate per axis
		data->rate[Y_AXIS] = data->travel[Y_AXIS] / (data->travel_dist / data->feed_rate); // calculate the rate per axis
		data->rate[Z_AXIS] = data->travel[Z_AXIS] / (data->travel_dist / data->feed_rate); // calculate the rate per axis
		data->rate[E_AXIS] = data->travel[E_AXIS] / (data->travel_dist / data->feed_rate); // calculate the rate per axis
	}

	data->rate[X_AXIS] = data->rate[X_AXIS] > X_MAX_SPEED ? X_MAX_SPEED : data->rate[X_AXIS]; // ensure MAX speed of axis is not exceeded
	data->rate[Y_AXIS] = data->rate[Y_AXIS] > Y_MAX_SPEED ? Y_MAX_SPEED : data->rate[Y_AXIS]; // ensure MAX speed of axis is not exceeded
	data->rate[Z_AXIS] = data->rate[Z_AXIS] > Z_MAX_SPEED ? Z_MAX_SPEED : data->rate[Z_AXIS]; // ensure MAX speed of axis is not exceeded
	data->rate[E_AXIS] = data->rate[E_AXIS] > E_MAX_SPEED ? E_MAX_SPEED : data->rate[E_AXIS]; // ensure MAX speed of axis is not exceeded

	return;
}

void calc_time(struct Positions *data) {

	uint8_t idx;
	data->max_time = 0.0f;

	for (idx = 0; idx<N_AXIS; idx++) {

		if (data->travel[idx] > NO_TRAVEL) { // travel move on this axis
			
			float inital_vel = 0.0f; //inital velocity
			float final_vel = 0.0f; //final velocity

			float speed_mms = data->rate[idx] / 60.0f; // mm/min to mm/s

			// calculate acceleration (time to get up to speed)
			float acc_time = (speed_mms - inital_vel) / data->accel[idx];
			float acc_dist = 0.5f * (data->accel[idx] * (acc_time * acc_time));

			// calculate deceleration
			float dec_time = (speed_mms - final_vel) / data->accel[idx];
			float dec_dist = 0.5f * (data->accel[idx] * (acc_time * acc_time));

			float ad_dist = acc_dist + dec_dist; // acceleration and deceleration distance		

			if (data->travel[idx] >= ad_dist) {
				float coast_dist = data->travel[idx] - ad_dist;
				float coast_time = coast_dist / speed_mms; // calculate the amount of coasting time

				data->time[idx] = acc_time + coast_time + dec_time;
			} else {
				float half_dist = data->travel[idx] / 2.0f;
				float max_vel = sqrt((inital_vel * inital_vel) + 2.0f * data->accel[idx] * half_dist); // v2 = u2 + 2as - calculate max velocity of this movement
				
				data->time[idx] = ((max_vel - inital_vel) / data->accel[idx]) * 2.0f; // calculate the acceleration time then double it as we will be declerating to a stop.
			}

		} else {
			data->time[idx] = 0.0f;
		}

		data->max_time = fmaxf(data->max_time, data->time[idx]); // check if this axis will take longer than any previous axis

	}
	return;
}

void print_distances(void) {
	printf("Move = %.3fmm, Extrude = %.3fmm Time = %.3fmin\n", total_dist, total_extrude, total_time/60.0f);
}

void gcodeOut(FILE *file, char *line) {
	
	Pos.new_pos[X_AXIS] = Pos.cur_pos[X_AXIS];
	Pos.new_pos[Y_AXIS] = Pos.cur_pos[Y_AXIS];
	Pos.new_pos[Z_AXIS] = Pos.cur_pos[Z_AXIS];
	Pos.new_pos[E_AXIS] = Pos.cur_pos[E_AXIS];	
	
    if (strncmp(line, "G1", 2) == 0 || strncmp(line, "G0", 2) == 0) {
        
        if (strchr(line, 'X') != NULL) {
            char *xstring = strstr(line, "X")+1;
            Pos.new_pos[X_AXIS] = strtof (xstring, NULL);
        }
        
        if (strchr(line, 'Y') != NULL) {
            char *ystring = strstr(line, "Y")+1;
            Pos.new_pos[Y_AXIS] = strtof (ystring, NULL);
        }
        
        if (strchr(line, 'Z') != NULL) {
            char *zstring = strstr(line, "Z")+1;
            Pos.new_pos[Z_AXIS] = strtof (zstring, NULL);
        }

		if (strchr(line, 'E') != NULL) {
            char *estring = strstr(line, "E")+1;
            Pos.new_pos[E_AXIS] = strtof (estring, NULL);
        }
        
        if (strchr(line, 'F') != NULL) {
            char *fstring = strstr(line, "F")+1;
            Pos.feed_rate = strtof (fstring, NULL);
        }
        
		calc_travel(&Pos);
		calc_rate(&Pos);
		calc_time(&Pos);

		total_dist = total_dist + Pos.travel_dist;
		total_extrude = total_extrude + Pos.travel[E_AXIS];
		total_time = total_time + Pos.max_time;

		fprintf(file, "TL=%.3f TM=%.3f TRL=%.3f TRM=%.3f TE=%.3f - Pos X=%.3f, Y=%.3f, Z=%.3f, E=%.3f - New X=%.3f, Y=%.3f, Z=%.3f, E=%.3f\n", Pos.max_time, total_time, Pos.travel_dist, total_dist, total_extrude, Pos.cur_pos[X_AXIS], Pos.cur_pos[Y_AXIS], Pos.cur_pos[Z_AXIS], Pos.cur_pos[E_AXIS], Pos.new_pos[X_AXIS], Pos.new_pos[Y_AXIS], Pos.new_pos[Z_AXIS], Pos.new_pos[E_AXIS]);
    
	} else if (strncmp(line, "G92", 3) == 0) {
	
		if (strchr(line, 'X') != NULL) {
            char *xstring = strstr(line, "X")+1;
            Pos.new_pos[X_AXIS] = strtof (xstring, NULL);
        }
        
        if (strchr(line, 'Y') != NULL) {
            char *ystring = strstr(line, "Y")+1;
            Pos.new_pos[Y_AXIS] = strtof (ystring, NULL);
        }
        
        if (strchr(line, 'Z') != NULL) {
            char *zstring = strstr(line, "Z")+1;
            Pos.new_pos[Z_AXIS] = strtof (zstring, NULL);
        }
		
		if (strchr(line, 'E') != NULL) {
            char *estring = strstr(line, "E")+1;
            Pos.new_pos[E_AXIS] = strtof (estring, NULL);
        }
        
        if (strchr(line, 'F') != NULL) {
            char *fstring = strstr(line, "F")+1;
            Pos.feed_rate = strtof (fstring, NULL);
        }
	    
	} 
    // set all new values to current postions
	Pos.cur_pos[X_AXIS] = Pos.new_pos[X_AXIS];
	Pos.cur_pos[Y_AXIS] = Pos.new_pos[Y_AXIS];
	Pos.cur_pos[Z_AXIS] = Pos.new_pos[Z_AXIS];
	Pos.cur_pos[E_AXIS] = Pos.new_pos[E_AXIS];
	
    return;
}

