/*
 * Copyright (c) 2004, 2005, 2010 Tama Communications Corporation
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
#include <stdio.h>
#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#include <errno.h>
#include "global.h"
#include "assoc.h"
#include "htags.h"
#include "cache.h"

static ASSOC *assoc[GTAGLIM];
/**
 * @file
 *
 * Cache file is used for duplicate object entry.
 *
 * @par
 * If function 'func()' is defined more than once then the cache record
 * of @NAME{GTAGS} has @STRONG{(1)} the frequency and the name of duplicate object entry file,
 * else it has @STRONG{(2)} the tag definition. <br>
 * It can be distinguished the first character of the cache record.
 * If it is a blank then it is the former else the latter.
 *
 * @par
 * @STRONG{(1)} Duplicate tag file
 * @code{.txt}
 *	+-----------------------+
 *	|' '<fid>\0<frequency>\0|
 *	+-----------------------+
 * @endcode
 *    Duplicate tag file can be referred to as @FILE{D/\<fid\>.html}.
 *	
 * @par
 * @STRONG{(2)} Tag definition
 * @code{.txt}
 *	+----------------------+
 *	|<line number>\0<fid>\0|
 *	+----------------------+
 * @endcode
 *    Tag is referred to as @FILE{S/\<fid\>.html\#\<line number\>}.
 */

/**
 * cache_open: open cache file.
 */
void
cache_open(void)
{
	assoc[GTAGS]  = assoc_open();
	assoc[GRTAGS] = assoc_open();
	assoc[GSYMS] = symbol ? assoc_open() : NULL;
}
/**
 * cache_put: put tag line.
 *
 *	@param[in]	db	db type
 *	@param[in]	tag	tag name
 *	@param[in]	line	tag line
 *	@param[in]	len
 */
void
cache_put(int db, const char *tag, const char *line, int len)
{
	if (db >= GTAGLIM)
		die("I don't know such tag file.");
	assoc_put_withlen(assoc[db], tag, line, len);
}
/**
 * cache_get: get tag line.
 *
 *	@param[in]	db	db type
 *	@param[in]	tag	tag name
 *	@return		tag line
 */
const char *
cache_get(int db, const char *tag)
{
	if (db >= GTAGLIM)
		die("I don't know such tag file.");
	return assoc_get(assoc[db], tag);
}
/**
 * cache_close: close cache file.
 */
void
cache_close(void)
{
	int i;

	for (i = GTAGS; i < GTAGLIM; i++) {
		if (assoc[i]) {
			assoc_close(assoc[i]);
			assoc[i] = NULL;
		}
	}
}
