/*
 * Copyright (c) 2020 Tama Communications Corporation
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
#ifndef _VSTACK_H
#define _VSTACK_H

#include "varray.h"

typedef struct {
	VARRAY *varray;
	int stack_top;	/* -1 == empty */
} VSTACK;

VSTACK *vstack_open(int, int);
void *vstack_push(VSTACK *);
void *vstack_pop(VSTACK *);
void *vstack_top(VSTACK *);
void vstack_close(VSTACK *);

#endif /* ! _VSTACK_H */
