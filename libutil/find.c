/*
 * Copyright (c) 1997, 1998, 1999, 2000, 2001, 2002, 2005, 2006, 2008
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
#include <ctype.h>
#ifdef HAVE_DIRENT_H
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#endif
#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
#include <stdio.h>
#ifdef STDC_HEADERS
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "gparam.h"
#include "regex.h"

#include "abs2rel.h"
#include "char.h"
#include "checkalloc.h"
#include "conf.h"
#include "die.h"
#include "find.h"
#include "is_unixy.h"
#include "locatestring.h"
#include "makepath.h"
#include "path.h"
#include "strbuf.h"
#include "strlimcpy.h"
#include "test.h"

/*
 * usage of find_xxx()
 *
 *	find_open(NULL);
 *	while (path = find_read()) {
 *		...
 *	}
 *	find_close();
 *
 */
static regex_t skip_area;
static regex_t *skip;			/* regex for skipping units */
static regex_t suff_area;
static regex_t *suff = &suff_area;	/* regex for suffixes */
static STRBUF *list;
static int list_count;
static char **listarray;		/* list for skipping full path */
static FILE *ip;
static FILE *temp;
static char rootdir[MAXPATHLEN+1];
static char cwddir[MAXPATHLEN+1];
static int status;
#define FIND_OPEN	1
#define FILELIST_OPEN	2
#define END_OF_FIND	3

static void trim(char *);
static char *find_read_traverse(void);
static char *find_read_filelist(void);

extern int qflag;
#ifdef DEBUG
extern int debug;
#endif
/*
 * trim: remove blanks and '\'.
 */
static void
trim(char *s)
{
	char *p;

	for (p = s; *s; s++) {
		if (isspace((unsigned char)*s))
			continue;	
		if (*s == '\\' && *(s + 1))
			s++;
		*p++ = *s;
	}
	*p = 0;
}
/*
 * prepare_source: preparing regular expression.
 *
 *	i)	flags	flags for regcomp.
 *	go)	suff	regular expression for source files.
 */
static void
prepare_source(void)
{
	STRBUF *sb = strbuf_open(0);
	char *sufflist = NULL;
	int flags = REG_EXTENDED;

	/*
	 * load icase_path option.
	 */
	if (getconfb("icase_path"))
		flags |= REG_ICASE;
#if defined(_WIN32) || defined(__DJGPP__)
	flags |= REG_ICASE;
#endif
	strbuf_reset(sb);
	if (!getconfs("suffixes", sb))
		die("cannot get suffixes data.");
	sufflist = check_strdup(strbuf_value(sb));
	trim(sufflist);
	{
		const char *suffp;
		int retval;

		strbuf_reset(sb);
		strbuf_puts(sb, "\\.(");       /* ) */
		for (suffp = sufflist; suffp; ) {
			const char *p;

			for (p = suffp; *p && *p != ','; p++) {
				if (!isalnum((unsigned char)*p))
					strbuf_putc(sb, '\\');
				strbuf_putc(sb, *p);
			}
			if (!*p)
				break;
			assert(*p == ',');
			strbuf_putc(sb, '|');
			suffp = ++p;
		}
		strbuf_puts(sb, ")$");
		/*
		 * compile regular expression.
		 */
		retval = regcomp(suff, strbuf_value(sb), flags);
#ifdef DEBUG
		if (debug)
			fprintf(stderr, "find regex: %s\n", strbuf_value(sb));
#endif
		if (retval != 0)
			die("cannot compile regular expression.");
	}
	strbuf_close(sb);
	if (sufflist)
		free(sufflist);
}
/*
 * prepare_skip: prepare skipping files.
 *
 *	go)	skip	regular expression for skip files.
 *	go)	listarry[] skip list.
 *	go)	list_count count of skip list.
 */
static void
prepare_skip(void)
{
	char *skiplist;
	STRBUF *reg = strbuf_open(0);
	int reg_count = 0;
	char *p, *q;
	int flags = REG_EXTENDED|REG_NEWLINE;

	/*
	 * load icase_path option.
	 */
	if (getconfb("icase_path"))
		flags |= REG_ICASE;
#if defined(_WIN32) || defined(__DJGPP__)
	flags |= REG_ICASE;
#endif
	/*
	 * initinalize common data.
	 */
	if (!list)
		list = strbuf_open(0);
	else
		strbuf_reset(list);
	list_count = 0;
	if (listarray)
		(void)free(listarray);
	listarray = (char **)0;
	/*
	 * load skip data.
	 */
	if (!getconfs("skip", reg)) {
		strbuf_close(reg);
		return;
	}
	skiplist = check_strdup(strbuf_value(reg));
	trim(skiplist);
	strbuf_reset(reg);
	/*
	 * construct regular expression.
	 */
	strbuf_putc(reg, '(');	/* ) */
	for (p = skiplist; p; ) {
		char *skipf = p;
		if ((p = locatestring(p, ",", MATCH_FIRST)) != NULL)
			*p++ = 0;
		if (*skipf == '/') {
			list_count++;
			strbuf_puts0(list, skipf);
		} else {
			reg_count++;
			strbuf_putc(reg, '/');
			for (q = skipf; *q; q++) {
				if (isregexchar(*q))
					strbuf_putc(reg, '\\');
				strbuf_putc(reg, *q);
			}
			if (*(q - 1) != '/')
				strbuf_putc(reg, '$');
			if (p)
				strbuf_putc(reg, '|');
		}
	}
	strbuf_unputc(reg, '|');
	strbuf_putc(reg, ')');
	if (reg_count > 0) {
		int retval;

		/*
		 * compile regular expression.
		 */
		skip = &skip_area;
		retval = regcomp(skip, strbuf_value(reg), flags);
#ifdef DEBUG
		if (debug)
			fprintf(stderr, "skip regex: %s\n", strbuf_value(reg));
#endif
		if (retval != 0)
			die("cannot compile regular expression.");
	} else {
		skip = (regex_t *)0;
	}
	if (list_count > 0) {
		int i;
		listarray = (char **)check_malloc(sizeof(char *) * list_count);
		p = strbuf_value(list);
#ifdef DEBUG
		if (debug)
			fprintf(stderr, "skip list: ");
#endif
		for (i = 0; i < list_count; i++) {
#ifdef DEBUG
			if (debug) {
				fprintf(stderr, "%s", p);
				if (i + 1 < list_count)
					fputc(',', stderr);
			}
#endif
			listarray[i] = p;
			p += strlen(p) + 1;
		}
#ifdef DEBUG
		if (debug)
			fputc('\n', stderr);
#endif
	}
	strbuf_close(reg);
	free(skiplist);
}
/*
 * skipthisfile: check whether or not we accept this file.
 *
 *	i)	path	path name (must start with ./)
 *	r)		1: skip, 0: dont skip
 */
static int
skipthisfile(const char *path)
{
	const char *first, *last;
	int i;

	/*
	 * unit check.
	 */
	if (skip && regexec(skip, path, 0, 0, 0) == 0)
		return 1;
	/*
	 * list check.
	 */
	if (list_count == 0)
		return 0;
	for (i = 0; i < list_count; i++) {
		first = listarray[i];
		last = first + strlen(first);
		/*
		 * the path must start with "./".
		 */
		if (*(last - 1) == '/') {	/* it's a directory */
			if (!strncmp(path + 1, first, last - first))
				return 1;
		} else {
			if (!strcmp(path + 1, first))
				return 1;
		}
	}
	return 0;
}

#define STACKSIZE 50
static  char dir[MAXPATHLEN+1];			/* directory path */
static  struct {
	STRBUF *sb;
	char *dirp, *start, *end, *p;
} stack[STACKSIZE], *topp, *curp;		/* stack */

/*
 * getdirs: get directory list
 *
 *	i)	dir	directory
 *	o)	sb	string buffer
 *	r)		-1: error, 0: normal
 *
 * format of directory list:
 * |ddir1\0ffile1\0llink\0|
 * means directory 'dir1', file 'file1' and symbolic link 'link'.
 */
static int
getdirs(const char *dir, STRBUF *sb)
{
	DIR *dirp;
	struct dirent *dp;
	struct stat st;

	if ((dirp = opendir(dir)) == NULL)
		return -1;
	while ((dp = readdir(dirp)) != NULL) {
		if (!strcmp(dp->d_name, "."))
			continue;
		if (!strcmp(dp->d_name, ".."))
			continue;
		if (stat(makepath(dir, dp->d_name, NULL), &st) < 0) {
			warning("cannot stat '%s'. (Ignored)", dp->d_name);
			continue;
		}
		if (S_ISDIR(st.st_mode))
			strbuf_putc(sb, 'd');
		else if (S_ISREG(st.st_mode))
			strbuf_putc(sb, 'f');
		else
			strbuf_putc(sb, ' ');
		strbuf_puts(sb, dp->d_name);
		strbuf_putc(sb, '\0');
	}
	(void)closedir(dirp);
	return 0;
}
/*
 * find_open: start iterator without GPATH.
 *
 *	i)	start	start directory
 *			If NULL, assumed '.' directory.
 */
void
find_open(const char *start)
{
	assert(status == 0);
	status = FIND_OPEN;

	if (!start)
		start = ".";
	/*
	 * setup stack.
	 */
	curp = &stack[0];
	topp = curp + STACKSIZE; 
	strlimcpy(dir, start, sizeof(dir));
	curp->dirp = dir + strlen(dir);
	curp->sb = strbuf_open(0);
	if (getdirs(dir, curp->sb) < 0)
		die("cannot open '.' directory.");
	curp->start = curp->p = strbuf_value(curp->sb);
	curp->end   = curp->start + strbuf_getlen(curp->sb);

	/*
	 * prepare regular expressions.
	 */
	prepare_source();
	prepare_skip();
}
/*
 * find_open_filelist: find_open like interface for handling output of find(1).
 *
 *	i)	filename	file including list of file names.
 *				When "-" is specified, read from standard input.
 *	i)	root		root directory of source tree
 */
void
find_open_filelist(const char *filename, const char *root)
{
	assert(status == 0);
	status = FILELIST_OPEN;

	if (!strcmp(filename, "-")) {
		/*
		 * If the filename is '-', copy standard input onto
		 * temporary file to be able to read repeatedly.
		 */
		if (temp == NULL) {
			char buf[MAXPATHLEN+1];

			temp = tmpfile();
			while (fgets(buf, sizeof(buf), stdin) != NULL)
				fputs(buf, temp);
		}
		rewind(temp);
		ip = temp;
	} else {
		ip = fopen(filename, "r");
		if (ip == NULL)
			die("cannot open '%s'.", filename);
	}
	/*
	 * rootdir always ends with '/'.
	 */
	if (!strcmp(root, "/"))
		strlimcpy(rootdir, root, sizeof(rootdir));
	else
		snprintf(rootdir, sizeof(rootdir), "%s/", root);
	strlimcpy(cwddir, root, sizeof(cwddir));
	/*
	 * prepare regular expressions.
	 */
	prepare_skip();
	prepare_source();
}
/*
 * find_read: read path without GPATH.
 *
 *	r)		path
 */
char *
find_read(void)
{
	static char *path;

	assert(status != 0);
	if (status == END_OF_FIND)
		path = NULL;
	else if (status == FILELIST_OPEN)
		path = find_read_filelist();
	else if (status == FIND_OPEN)
		path = find_read_traverse();
	else
		die("find_read: internal error.");
	return path;
}
/*
 * find_read_traverse: read path without GPATH.
 *
 *	r)		path
 */
char *
find_read_traverse(void)
{
	static char val[MAXPATHLEN+1];

	for (;;) {
		while (curp->p < curp->end) {
			char type = *(curp->p);
			const char *unit = curp->p + 1;

			curp->p += strlen(curp->p) + 1;
			if (type == 'f') {
				char path[MAXPATHLEN];

				/* makepath() returns unsafe module local area. */
				strlimcpy(path, makepath(dir, unit, NULL), sizeof(path));
				if (skipthisfile(path))
					continue;
				/*
				 * Skip the following:
				 * o directory
				 * o file which does not exist
				 * o dead symbolic link
				 */
				if (!test("f", path)) {
					if (!qflag) {
						if (test("d", path))
							warning("'%s' is a directory. (Ignored)", path);
						else
							warning("'%s' not found. (Ignored)", path);
					}
					continue;
				}
				/*
				 * GLOBAL cannot treat path which includes blanks.
				 * It will be improved in the future.
				 */
				if (locatestring(path, " ", MATCH_FIRST)) {
					if (!qflag)
						warning("'%s' ignored, because it includes blank.", &path[2]);
					continue;
				}
				/*
				 * A blank at the head of path means
				 * other than source file.
				 */
				if (regexec(suff, path, 0, 0, 0) == 0) {
					/* source file */
					strlimcpy(val, path, sizeof(val));
				} else {
					/* other file like 'Makefile' */
					val[0] = ' ';
					strlimcpy(&val[1], path, sizeof(val) - 1);
				}
				val[sizeof(val) - 1] = '\0';
				return val;
			}
			if (type == 'd') {
				STRBUF *sb = strbuf_open(0);
				char *dirp = curp->dirp;

				strcat(dirp, "/");
				strcat(dirp, unit);
				if (getdirs(dir, sb) < 0) {
					warning("cannot open directory '%s'. (Ignored)", dir);
					strbuf_close(sb);
					*(curp->dirp) = 0;
					continue;
				}
				/*
				 * Push stack.
				 */
				if (++curp >= topp)
					die("directory stack over flow.");
				curp->dirp = dirp + strlen(dirp);
				curp->sb = sb;
				curp->start = curp->p = strbuf_value(sb);
				curp->end   = curp->start + strbuf_getlen(sb);
			}
		}
		strbuf_close(curp->sb);
		curp->sb = NULL;
		if (curp == &stack[0])
			break;
		/*
		 * Pop stack.
		 */
		curp--;
		*(curp->dirp) = 0;
	}
	status = END_OF_FIND;
	return NULL;
}
/*
 * find_read_filelist: read path from file
 *
 *	r)		path
 */
static char *
find_read_filelist(void)
{
	STATIC_STRBUF(ib);
	static char buf[MAXPATHLEN + 1];
	static char *path;

	strbuf_clear(ib);
	for (;;) {
		path = strbuf_fgets(ib, ip, STRBUF_NOCRLF);
		if (path == NULL) {
			/* EOF */
			status = END_OF_FIND;
			return NULL;
		}
		if (*path == '\0') {
			/* skip empty line.  */
			continue;
		}
		/*
		 * Skip the following:
		 * o directory
		 * o file which does not exist
		 * o dead symbolic link
		 */
		if (!test("f", path)) {
			if (!qflag) {
				if (test("d", path))
					warning("'%s' is a directory. (Ignored)", path);
				else
					warning("'%s' not found. (Ignored)", path);
			}
			continue;
		}
		/*
		 * normalize path name.
		 *
		 *	rootdir  /a/b/
		 *	buf      /a/b/c/d.c -> c/d.c -> ./c/d.c
		 */
		path = normalize(path, rootdir, cwddir, buf, sizeof(buf));
		/*
		 * GLOBAL cannot treat path which includes blanks.
		 * It will be improved in the future.
		 */
		if (locatestring(path, " ", MATCH_LAST)) {
			if (!qflag)
				warning("'%s' ignored, because it includes blank.", path + 2);
			continue;
		}
		if (skipthisfile(path))
			continue;
		/*
		 * A blank at the head of path means
		 * other than source file.
		 */
		if (regexec(suff, path, 0, 0, 0) != 0)
			*--path = ' ';
		return path;
	}
}
/*
 * find_close: close iterator.
 */
void
find_close(void)
{
	assert(status != 0);
	if (status == FIND_OPEN) {
		for (curp = &stack[0]; curp < topp; curp++)
			if (curp->sb != NULL)
				strbuf_close(curp->sb);
	} else if (status == FILELIST_OPEN) {
		/*
		 * The --file=- option is specified, we don't close file
		 * to read it repeatedly.
		 */
		if (ip != temp)
			fclose(ip);
	} else if (status != END_OF_FIND) {
		die("illegal find_close");
	}
	regfree(suff);
	if (skip)
		regfree(skip);
	status = 0;
}
