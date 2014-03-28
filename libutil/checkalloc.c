/*
 * Copyright (c) 2006 Tama Communications Corporation
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
#include <stdlib.h>
#include <string.h>

#include "checkalloc.h"
#include "die.h"
/**
 * @file
 * Functions which allocate memory with check.
*/

/**
 * @details
 * check_malloc: memory allocator
 *
 * Uses @NAME{malloc()}.
 *
 * @note Does not return if memory is not available, calls die() instead.
 */
void *
check_malloc(size_t size)
{
	void *p = (void *)malloc(size);
	if (p == NULL)
		die("short of memory.");
	return p;
}

/**
 * @details
 * check_calloc: memory allocator
 *
 * Uses @NAME{calloc()}.
 *
 * @note Does not return if memory is not available, calls die() instead.
 */
void *
check_calloc(size_t number, size_t size)
{
	void *p = (void *)calloc(number, size);
	if (p == NULL)
		die("short of memory.");
	return p;
}
/**
 * @details
 * check_realloc: memory allocator
 *
 * Uses @NAME{realloc()}.
 *
 * @note Does not return if memory is not available, calls die() instead.
 */
void *
check_realloc(void *area, size_t size)
{
	void *p = (void *)realloc(area, size);
	if (p == NULL)
		die("short of memory.");
	return p;
}
/**
 * @details
 * check_strdup: allocate memory and copy string to it.
 *
 *	@param[in]	string	original string
 *	@return		allocated memory
 *
 * Uses check_malloc().
 */
char *
check_strdup(const char *string)
{
	char *p = check_malloc(strlen(string) + 1);
	strcpy(p, string);
	return p;
}
