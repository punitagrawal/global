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
#include "gtagsop.h"
#include "makepath.h"
#include "gpathop.h"
#include "strbuf.h"
#include "strlimcpy.h"

static DBOP *dbop;
static int _nextkey;
static int _mode;
static int opened;
static int created;

/**
 * @file
 * @NAME{GPATH} format version
 *
 * -# @XREF{gtags,1} bury version number in #GPATH.
 * -# @XREF{global,1} pick up the version number from #GPATH. If the number
 *    is not acceptable version number then @NAME{global} give up work any more
 *    and display error message. <br>
 * -# If version number is not found then it assumes version 1.
 * -# @NAME{GPATH} version is independent with the other tag files.
 *
 * @par [History of format version]
 *
 * @NAME{GLOBAL-4.8.7}		no idea about format version. <br>
 * @NAME{GLOBAL-5.0}		understand format version. <br>
 *			support format version 2.
 *
 * - Format version 1
 * @par
 *
 * #GPATH has only source files.
 *
 * @par
 * @code{.txt}
 *      key             data
 *      --------------------
 *      ./aaa.c\0       11\0
 * @endcode
 *
 * - Format version 2
 * @par
 *
 * #GPATH has not only source files but also other files like @FILE{README}.
 * You can distinguish them by the flag following data value.
 * At present, the flag value is only @CODE{'o'} (other files).
 *
 * @par
 * @code{.txt}
 *      key             data
 *      --------------------
 *      ./aaa.c\0       11\0
 *      ./README\0      12\0o\0         <=== 'o' means other files.
 * @endcode
 */
static int support_version = 2;	/**< acceptable format version   */
static int create_version = 2;	/**< format version of newly created tag file */
/**
 * gpath_open: open gpath tag file
 *
 *	@param[in]	dbpath	@VAR{GTAGSDBPATH}
 *	@param[in]	mode	0: read only <br>
 *			1: create <br>
 *			2: modify
 *	@return		0: normal <br>
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
	dbop = dbop_open(makepath(dbpath, dbname(GPATH), NULL), mode, 0644, 0);
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
 *	@param[in]	type	path type <br>
 *			#GPATH_SOURCE: source file <br>
 *			#GPATH_OTHER: other file
 */
void
gpath_put(const char *path, int type)
{
	char fid[MAXFIDLEN];
	STATIC_STRBUF(sb);

	assert(opened > 0);
	if (_mode == 1 && created)
		return;
	if (dbop_get(dbop, path) != NULL)
		return;
	/*
	 * generate new file id for the path.
	 */
	snprintf(fid, sizeof(fid), "%d", _nextkey++);
	/*
	 * path => fid mapping.
	 */
	strbuf_clear(sb);
	strbuf_puts0(sb, fid);
	if (type == GPATH_OTHER)
		strbuf_puts0(sb, "o");
	dbop_put_withlen(dbop, path, strbuf_value(sb), strbuf_getlen(sb));
	/*
	 * fid => path mapping.
	 */
	strbuf_clear(sb);
	strbuf_puts0(sb, path);
	if (type == GPATH_OTHER)
		strbuf_puts0(sb, "o");
	dbop_put_withlen(dbop, fid, strbuf_value(sb), strbuf_getlen(sb));
}
/**
 * gpath_path2fid: convert path into id
 *
 *	@param[in]	path	path name
 *	@param[out]	type	path type <br>
 *			#GPATH_SOURCE: source file <br>
 *			#GPATH_OTHER: other file
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
 *	@param[out]	type	path type <br>
 *			#GPATH_SOURCE: source file <br>
 *			#GPATH_OTHER: other file
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
 * @fn GFIND *gfind_open(const char *dbpath, const char *local, int target)
 *
 * @remark gfind iterator using #GPATH.
 *
 * @par
 * @NAME{gfind_xxx()} does almost same with @NAME{find_xxx()} but much faster,
 * because @NAME{gfind_xxx()} use #GPATH (file index). <br>
 * If #GPATH exist then you should use this.
 */

/**
 * gfind_open: start iterator using #GPATH.
 *
 *	@param[in]	dbpath  dbpath
 *	@param[in]	local   local prefix <br>
 *			if @VAR{NULL} specified, it assumes @FILE{./};
 *	@param[in]      target  #GPATH_SOURCE: only source file <br>
 *			#GPATH_OTHER: only other file <br>
 *			#GPATH_BOTH: source file + other file
 *	@return		#GFIND structure
 */
GFIND *
gfind_open(const char *dbpath, const char *local, int target)
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
	gfind->version = dbop_getversion(gfind->dbop);
	if (gfind->version > support_version)
		die("GPATH seems new format. Please install the latest GLOBAL.");
	else if (gfind->version < support_version)
		die("GPATH seems older format. Please remake tag files."); 
	return gfind;
}
/**
 * gfind_read: read path using #GPATH.
 *
 *	@param[in]	gfind	#GFIND structure
 *	@return		path
 */
const char *
gfind_read(GFIND *gfind)
{
	const char *flag;

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
	free((void *)gfind->prefix);
	free(gfind);
}
