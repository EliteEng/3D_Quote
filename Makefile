#      
# 3D Quote by Elite Engineering WA
#                                           
# This file is part of the 3D Quote distribution (https://github.com/EliteEng/3D_Quote).
# Copyright (c) 2018 Rob Brown - Elite Engineering WA.
# 
# This program is free software: you can redistribute it and/or modify  
# it under the terms of the GNU General Public License as published by  
# the Free Software Foundation, version 3.
#
# This program is distributed in the hope that it will be useful, but 
# WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License 
# along with this program. If not, see <http://www.gnu.org/licenses/>.

CC=gcc
CFLAGS=-c -Wall -O2
LDFLAGS=-lm
SOURCES=main.c gcode.c
OBJDIR=obj
OBJECTS=$(SOURCES:%.c=$(OBJDIR)/%.o)
EXECUTABLE=3dquote

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) -g $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.c
	$(CC) -g $(CFLAGS) $< -o $@
	
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)
