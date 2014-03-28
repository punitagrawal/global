/*
 * Copyright (c) 1996, 1997, 1998, 1999, 2000, 2006
 *	Tama Communications Corporation
 *
 * This file is part of GNU GLOBAL.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _TAB_H_
#define _TAB_H_

#include <stdio.h>

void settabs(int);
size_t read_file_detabing(char *, size_t, FILE *, int *, int *);
void detab_replacing(FILE *op, const char *buf, const char *(*replace)(int c));


#endif /* ! _TAB_H_ */
