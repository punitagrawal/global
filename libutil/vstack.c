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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef STDC_HEADERS
#include <stdlib.h>
#endif
#include <stdio.h>
#include <string.h>
#include "checkalloc.h"
#include "die.h"
#include "vstack.h"

/*

Stack: usage and memory status

Function call                           Memory status
----------------------------------------------------------
					not exist
struct X *curp;
vs = vstack_open(sizeof(struct X));	||
curp = vstack_top(vs);			||	curp == NULL
curp = vstack_push(vs);			|struct X|<- curp
curp = vstack_push(vs);			|struct X|struct X|<- curp
curp = vstack_top(vs);			|struct X|struct X|<- curp
curp = vstack_pop(vs);			|struct X|<- curp
curp = vstack_pop(vs);			||	curp == NULL
vstack_close(vs);			not exist
*/

/**
 * vstack_open: make a stack
 *
 *	@param[in]	size	size of entry
 *	@param[in]	expand	expand size of array
 *	@return	vs	VSTACK structure
 */
VSTACK *
vstack_open(int size, int expand)
{
	VSTACK *vs = (VSTACK *)check_calloc(sizeof(VSTACK), 1);

	vs->varray = varray_open(size, expand);
	vs->stack_top = -1;		/* empty */
	return vs;
}
/**
 * vstack_push: push a new entry to the stack.
 *
 *	@param[in]	vs	VSTACK structure
 *	@return		pointer of the entry
 */
void *
vstack_push(VSTACK *vs)
{
	void *entry =  varray_assign(vs->varray, ++vs->stack_top, 1);
	memset(entry, 0, vs->varray->size);
	return entry;
}
/**
 * vstack_pop: pop the top entry of the stack.
 *
 *	@param[in]	vs	VSTACK structure
 *	@return		pointer of the entry
 */
void *
vstack_pop(VSTACK *vs)
{
	if (--vs->stack_top < 0) {
		vs->stack_top = -1;
		return NULL;
	}
	return varray_assign(vs->varray, vs->stack_top, 0);
}
/**
 * vstack_top: return the top entry of the stack.
 *
 *	@param[in]	vs	VSTACK structure
 *	@return		pointer of the entry
 */
void *
vstack_top(VSTACK *vs)
{
	if (vs->stack_top < 0)
		return NULL;
	return varray_assign(vs->varray, vs->stack_top, 0);
}
/**
 * vstack_close: close the stack.
 *
 *	@param[in]	vs	VSTACK structure
 */
void
vstack_close(VSTACK *vs)
{
	if (vs) {
		varray_close(vs->varray);
		(void)free(vs);
	}
}
