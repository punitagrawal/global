/*
 * Copyright (c) 2016
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

#ifndef _POPEN_H
#define _POPEN_H

#include <stdio.h>

void secure_open_args(void);
void secure_add_args(char *);
char **secure_close_args(void);
FILE *secure_popen(const char *, const char *, char **);
int secure_pclose(FILE *);

#endif /* ! _POPEN_H */
