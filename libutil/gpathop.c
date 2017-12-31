/*
 * Copyright (c) 1997, 1998, 1999, 2000, 2001, 2002, 2005, 2006
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <assert.h>
#ifdef STDC_HEADERS
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif

#include "checkalloc.h"
#include "die.h"
#include "dbop.h"
#include "getdbpath.h"
#include "gtagsop.h"
#include "gpathop.h"
#include "makepath.h"
#include "nearsort.h"
#include "strbuf.h"
#include "strlimcpy.h"

static DBOP *dbop;
static int _nextkey;
static int _mode;
static int opened;
static int created;

int openflags;
void
set_gpath_flags(int flags) {
	openflags = flags;
}
/**
 * compare_nearpath: compare function for 'nearness sort'.
 */
static const char *nearbase;
static int
compare_nearpath(const void *s1, const void *s2)
{
	int ret;

	if ((ret = COMPARE_NEARNESS(*(char **)s1, *(char **)s2, nearbase)) != 0)
		return ret;
	return strcmp(*(char **)s1, *(char **)s2);
}
/*
 * GPATH format version
 *
 * 1. gtags(1) bury version number in GPATH.
 * 2. global(1) pick up the version number from GPATH. If the number
 *    is not acceptable version number then global give up work any more
 *    and display error message.
 * 3. If version number is not found then it assumes version 1.
 * 4. GPATH version is independent with the other tag files.
 *
 * [History of format version]
 *
 * GLOBAL-4.8.7		no idea about format version.
 * GLOBAL-5.0		understand format version.
 *			support format version 2.
 *
 * - Format version 1
 *
 * GPATH has only source files.
 *
 *      key             data
 *      --------------------
 *      ./aaa.c\0       11\0
 *
 * - Format version 2
 *
 * GPATH has not only source files but also other files like "README".
 * You can distinguish them by the flag following data value.
 * At present, the flag value is only 'o' (other files).
 *
 *      key             data
 *      --------------------
 *      ./aaa.c\0       11\0
 *      ./README\0      12\0o\0         <=== 'o' means other files.
 */
static int support_version = 2;	/**< acceptable format version   */
static int create_version = 2;	/**< format version of newly created tag file */
/**
 * gpath_open: open gpath tag file
 *
 *	@param[in]	dbpath	GTAGSDBPATH
 *	@param[in]	mode	0: read only,
 *			1: create,
 *			2: modify
 *	@return		0: normal,
 *			-1: error
 */
int
gpath_open(const char *dbpath, int mode)
{
	if (opened > 0) {
		if (mode != _mode)
			die("duplicate open with different mode.");
		opened++;
		return 0;
	}
	/*
	 * We create GPATH just first time.
	 */
	_mode = mode;
	if (mode == 1 && created)
		mode = 0;
	dbop = dbop_open(makepath(dbpath, dbname(GPATH), NULL), mode, 0644, openflags);
	if (dbop == NULL)
		return -1;
	if (mode == 1) {
		dbop_putversion(dbop, create_version);
		_nextkey = 1;
	} else {
		int format_version;
		const char *path = dbop_get(dbop, NEXTKEY);

		if (path == NULL)
			die("nextkey not found in GPATH.");
		_nextkey = atoi(path);
		format_version = dbop_getversion(dbop);
		if (format_version > support_version)
			die("GPATH seems new format. Please install the latest GLOBAL.");
		else if (format_version < support_version)
                        die("GPATH seems older format. Please remake tag files."); 
	}
	opened++;
	return 0;
}
/**
 * gpath_put: put path name
 *
 *	@param[in]	path	path name
 *	@param[in]	type	path type
 *			GPATH_SOURCE: source file,
 *			GPATH_OTHER: other file
 */
const char *
gpath_put(const char *path, int type)
{
	static char sfid[MAXFIDLEN];
	STATIC_STRBUF(sb);

	assert(opened > 0);
	if (_mode == 1 && created)
		return "";
	if (dbop_get(dbop, path) != NULL)
		return "";
	/*
	 * generate new file id for the path.
	 */
	snprintf(sfid, sizeof(sfid), "%d", _nextkey++);
	/*
	 * path => fid mapping.
	 */
	strbuf_clear(sb);
	strbuf_puts(sb, sfid);
	dbop_put_path(dbop, path, strbuf_value(sb), type == GPATH_OTHER ? "o" : NULL);
	/*
	 * fid => path mapping.
	 */
	strbuf_clear(sb);
	strbuf_puts(sb, path);
	dbop_put_path(dbop, sfid, strbuf_value(sb), type == GPATH_OTHER ? "o" : NULL);
	return (const char *)sfid;
}
/**
 * gpath_path2fid: convert path into id
 *
 *	@param[in]	path	path name
 *	@param[out]	type	path type
 *			GPATH_SOURCE: source file,
 *			GPATH_OTHER: other file
 *	@return		file id
 */
const char *
gpath_path2fid(const char *path, int *type)
{
	const char *fid = dbop_get(dbop, path);
	assert(opened > 0);
	if (fid && type) {
		const char *flag = dbop_getflag(dbop);
		*type = (*flag == 'o') ? GPATH_OTHER : GPATH_SOURCE;
			
	}
	return fid;
}
/**
 * gpath_fid2path: convert id into path
 *
 *	@param[in]	fid	file id
 *	@param[out]	type	path type
 *			GPATH_SOURCE: source file,
 *			GPATH_OTHER: other file
 *	@return		path name
 */
const char *
gpath_fid2path(const char *fid, int *type)
{
	const char *path = dbop_get(dbop, fid);
	assert(opened > 0);
	if (path && type) {
		const char *flag = dbop_getflag(dbop);
		*type = (*flag == 'o') ? GPATH_OTHER : GPATH_SOURCE;
	}
	return path;
}
/*
 * gpath_path2nfid: convert path into id
 *
 *	i)	path	path name
 *	o)	type	path type
 *			GPATH_SOURCE: source file
 *			GPATH_OTHER: other file
 *	r)		file id (integer)
 */
int
gpath_path2nfid(const char *path, int *type)
{
	const char *sfid = gpath_path2fid(path, type);
	return sfid == NULL ? 0 : atoi(sfid);
}
/*
 * gpath_nfid2path: convert id into path
 *
 *	i)	nfid	file id (integer)
 *	o)	type	path type
 *			GPATH_SOURCE: source file
 *			GPATH_OTHER: other file
 *	r)		path name
 */
const char *
gpath_nfid2path(int nfid, int *type)
{
	char sfid[MAXFIDLEN];
	snprintf(sfid, sizeof(sfid), "%d", nfid);
	return gpath_fid2path(sfid, type);
}
/**
 * gpath_delete: delete specified path record
 *
 *	@param[in]	path	path name
 */
void
gpath_delete(const char *path)
{
	const char *fid;

	assert(opened > 0);
	assert(_mode == 2);
	assert(path[0] == '.' && path[1] == '/');
	fid = dbop_get(dbop, path);
	if (fid == NULL)
		return;
	dbop_delete(dbop, fid);
	dbop_delete(dbop, path);
}
/**
 * gpath_nextkey: return next key
 *
 *	@return		next id
 */
int
gpath_nextkey(void)
{
	assert(_mode != 1);
	return _nextkey;
}
/**
 * gpath_close: close gpath tag file
 */
void
gpath_close(void)
{
	char fid[MAXFIDLEN];

	assert(opened > 0);
	if (--opened > 0)
		return;
	if (_mode == 1 && created) {
		dbop_close(dbop);
		return;
	}
	if (_mode == 1 || _mode == 2) {
		snprintf(fid, sizeof(fid), "%d", _nextkey);
		dbop_update(dbop, NEXTKEY, fid);
	}
	dbop_close(dbop);
	if (_mode == 1)
		created = 1;
}

/**
 * GFIND *gfind_open(const char *dbpath, const char *local, int target, int flags)
 *
 * gfind iterator using GPATH.
 *
 * gfind_xxx() does almost same with find_xxx() but much faster,
 * because gfind_xxx() use GPATH (file index).
 * If GPATH exist then you should use this.
 */

/**
 * gfind_open: start iterator using GPATH.
 *
 *	@param[in]	dbpath  dbpath
 *	@param[in]	local   local prefix,
 *			if NULL specified, it assumes "./";
 *	@param[in]      target  GPATH_SOURCE: only source file,
 *			GPATH_OTHER: only other file,
 *			GPATH_BOTH: source file + other file
 *	@param[in]	flags	GPATH_NEARSORT
 *	@return		GFIND structure
 */
GFIND *
gfind_open(const char *dbpath, const char *local, int target, int flags)
{
	GFIND *gfind = (GFIND *)check_calloc(sizeof(GFIND), 1);

	gfind->dbop = dbop_open(makepath(dbpath, dbname(GPATH), NULL), 0, 0, 0);
	if (gfind->dbop == NULL)
		die("GPATH not found.");
	gfind->path = NULL;
	gfind->prefix = check_strdup(local ? local : "./");
	gfind->first = 1;
	gfind->eod = 0;
	gfind->target = target;
	gfind->type = GPATH_SOURCE;
	gfind->flags = flags;
	gfind->path_array = NULL;
	gfind->version = dbop_getversion(gfind->dbop);
	if (gfind->version > support_version)
		die("GPATH seems new format. Please install the latest GLOBAL.");
	else if (gfind->version < support_version)
		die("GPATH seems older format. Please remake tag files."); 
	/*
	 * Nearness sort.
	 * In fact, this timing of sort is not good for performance.
	 * Reconsideration is needed later.
	 */
	if (gfind->flags & GPATH_NEARSORT) {
		const char *path = NULL;
		VARRAY *varray = varray_open(sizeof(char *), 100);
		POOL *pool = pool_open();
		while ((path = gfind_read(gfind)) != NULL) {
			char **a = varray_append(varray);
			*a = pool_strdup(pool, path, 0);
		}
		if ((nearbase = get_nearbase_path()) == NULL)
			die("cannot get nearbase path.");
		qsort(varray_assign(varray, 0, 0), varray->length, sizeof(char *), compare_nearpath);
		gfind->path_array = varray;
		gfind->pool = pool;
		gfind->index = 0;
	}
	return gfind;
}
/**
 * gfind_read: read path using GPATH.
 *
 *	@param[in]	gfind	GFIND structure
 *	@return		path
 */
const char *
gfind_read(GFIND *gfind)
{
	const char *flag;

	if (gfind->path_array) {
		char **a = NULL;
		if (gfind->index >= gfind->path_array->length)
			return NULL;
		a = varray_assign(gfind->path_array, gfind->index++, 0);
		return *a;
	}
	gfind->type = GPATH_SOURCE;
	if (gfind->eod)
		return NULL;
	for (;;) {
		if (gfind->first) {
			gfind->first = 0;
			gfind->path = dbop_first(gfind->dbop, gfind->prefix, NULL, DBOP_KEY | DBOP_PREFIX);
		} else {
			gfind->path = dbop_next(gfind->dbop);
		}
		if (gfind->path == NULL) {
			gfind->eod = 1;
			break;
		}
		/*
		 * if gfind->target == 0, return only source files.
		 * *flag == 'o' means 'other files' like README.
		 */
		flag = dbop_getflag(gfind->dbop);
		if (flag == NULL)
			flag = "";
		gfind->type = (*flag == 'o') ? GPATH_OTHER : GPATH_SOURCE;
		if (gfind->type & gfind->target)
			break;
	}
	return gfind->path;
}
/**
 * gfind_close: close iterator.
 */
void
gfind_close(GFIND *gfind)
{
	dbop_close(gfind->dbop);
	if (gfind->flags & GPATH_NEARSORT) {
		pool_close(gfind->pool);
		varray_close(gfind->path_array);
	}
	free((void *)gfind->prefix);
	free(gfind);
}
