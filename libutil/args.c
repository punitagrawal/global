/*
 * Copyright (c) 2010, 2014, 2020 Tama Communications Corporation
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
#include <ctype.h>
#include <stdio.h>
#include "getopt.h"
#include "checkalloc.h"
#include "die.h"
#include "env.h"
#include "locatestring.h"
#include "strbuf.h"
#include "test.h"
#include "gpathop.h"
#include "args.h"
#if (defined(_WIN32) && !defined(__CYGWIN__)) || defined(__DJGPP__)
#include "path.h"
#endif

#define ARGS_NOP	0
#define ARGS_ARGS	1
#define ARGS_FILELIST	2
#define ARGS_GFIND	3
#define ARGS_BOTH	4

static int type;
static char *const *argslist;
static FILE *ip;
static GFIND *gp;

/**
 * args_open:
 *
 *	@param[in]	args	args array
 */
void
args_open(char *const *argv)
{
	type = ARGS_ARGS;
	argslist = argv;
}
/**
 * args_open_filelist: args_open like interface for handling output of find(1).
 *
 *	@param[in]	filename	file including list of file names.
 *				When "-" is specified, read from standard input.
 */
void
args_open_filelist(const char *filename)
{
	type = ARGS_FILELIST;
	if (!strcmp(filename, "-")) {
		ip = stdin;
	} else {
		ip = fopen(filename, "r");
		if (ip == NULL)
			die("cannot open '%s'.", filename);
	}
}
/**
 * args_open_both: args_open like interface for argument and file list.
 *
 *	@param[in]	args		args array
 *	@param[in]	filename	file including list of file names.
 *				When "-" is specified, read from standard input.
 */
void
args_open_both(char *const *argv, const char *filename)
{
	type = ARGS_BOTH;
	argslist = argv;
	if (!strcmp(filename, "-")) {
		ip = stdin;
	} else {
		ip = fopen(filename, "r");
		if (ip == NULL)
			die("cannot open '%s'.", filename);
	}
}
/**
 * args_open_gfind: args_open like interface for handling output of gfind.
 *
 *	@param[in]	agp	GFIND descriptor
 */
void
args_open_gfind(GFIND *agp)
{
	type = ARGS_GFIND;
	gp = agp;
}
void
args_open_nop(void)
{
	type = ARGS_NOP;
}
/**
 * args_read: read path From args.
 *
 *	@return		path (NULL: end of argument)
 */
const char *
args_read(void)
{
	const char *p;
	STATIC_STRBUF(sb);

	strbuf_clear(sb);
	switch (type) {
	case ARGS_NOP:
		p = NULL;
		break;
	case ARGS_ARGS:
		p = *argslist++;
		break;
	case ARGS_FILELIST:
		p = strbuf_fgets(sb, ip, STRBUF_NOCRLF);
		break;
	case ARGS_GFIND:
		p = gfind_read(gp);
		break;
	case ARGS_BOTH:
		if (*argslist != NULL)
			p = *argslist++;
		else
			p = strbuf_fgets(sb, ip, STRBUF_NOCRLF);
		break;
	default:
		die("args_read: invalid type.");
	}
	return p;
}
/**
 * args_close: close args.
 */
void
args_close(void)
{
	switch (type) {
	case ARGS_NOP:
	case ARGS_ARGS:
		break;
	case ARGS_FILELIST:
	case ARGS_BOTH:
		if (ip != NULL && ip != stdin)
			fclose(ip);
		ip = NULL;
		break;
	case ARGS_GFIND:
		if (gp != NULL)
			gfind_close(gp);
		gp = NULL;
		break;
	default:
		die("something wrong.");
	}
}
/**
 * preparse_options
 *
 *	@param[in]	argc	main()'s argc integer
 *	@param[in]	argv	main()'s argv string array
 *
 * Setup the "GTAGSCONF" and "GTAGSLABEL" environment variables
 * according to the --gtagsconf and --gtagslabel options.
 * Additionally changes directory acording to the --directory.
 */
int
preparse_options(int argc, char *const *argv)
{
	int optchar;
	int option_index = 0;
	char *confpath = NULL;
	char *label = NULL;
	char *dir = NULL;
	extern const char *short_options;
	extern struct option const long_options[];

	/*
	 * restart scanning of the same argv by setting optind = 1.
	 */
        optind = 1;
	while ((optchar = getopt_long(argc, argv, short_options, long_options, &option_index)) != EOF) {
		switch (optchar) {
		case 'C':
			dir = optarg;
			break;
		case OPT_GTAGSCONF:
			confpath = optarg;
			break;
		case OPT_GTAGSLABEL:
			label = optarg;
			break;
		case '?':
			return -1;
		default:
			break;
		}
	}
	/*
	 * Change the directory before doing all the work including parameter analysis.
	 */
	if (dir)
		if (chdir(dir) < 0)
			die("cannot change directory to '%s'.", dir);
	if (confpath) {
		char real[MAXPATHLEN];

		if (!test("f", confpath))
			die("--gtagsconf file not found.");
		if (!realpath(confpath, real))
			die("cannot get absolute path of --gtagsconf file.");
		set_env("GTAGSCONF", real);
	}
	if (label)
		set_env("GTAGSLABEL", label);
	/*
	 * restart scanning of the same argv by setting optind = 1.
	 * This is needed for the calling of getopt() in main().
	 */
        optind = 1;
	return 0;
}
/**
 * prepend_options: creates a new argv main() array, by prepending (space separated)
 *		options and arguments from the string argument options.
 *
 *	@param[in,out]	argc	pointer to main()'s argc integer
 *	@param[in]	argv	main()'s argv string array
 *	@param[in]	options	string
 *	@return	The new argv array.
 *
 *	The program's name is copied back into: returned[0] (argv[0]).
 */
char **
prepend_options(int *argc, char *const *argv, const char *options)
{
	STRBUF *sb = strbuf_open(0);
	const char *p, *opt = check_strdup(options);
	int count = 1;
	int quote = 0;
	const char **newargv;
	int i = 0, j = 1;

	for (p = opt; *p && isspace(*p); p++)
		;
	for (; *p; p++) {
		int c = *p;

		if (quote) {
			if (quote == c)
				quote = 0;
			else
				strbuf_putc(sb, c);
		} else if (c == '\\') {
			if (*(p + 1))
				strbuf_putc(sb, *++p);
		} else if (c == '\'' || c == '"') {
			quote = c;
		} else if (isspace(c)) {
			strbuf_putc(sb, '\0');
			count++;
			while (*p && isspace(*p))
				p++;
			p--;
		} else {
			strbuf_putc(sb, *p);
		}
	}
	newargv = (const char **)check_malloc(sizeof(char *) * (*argc + count + 1));
	newargv[i++] = argv[0];
	p = strbuf_value(sb);
	while (count--) {
		newargv[i++] = p;
		p += strlen(p) + 1;
	}
	while (j < *argc)
		newargv[i++] = argv[j++];
	newargv[i] = NULL;
	*argc = i;
#ifdef DEBUG
	for (i = 0; i < *argc; i++)
		fprintf(stderr, "newargv[%d] = '%s'\n", i, newargv[i]);
#endif
	/* doesn't close string buffer. */

	return (char **)newargv;
}
/**
 * serialize_options
 */
char *
serialize_options(int argc, char *const *argv)
{
	STRBUF *sb = strbuf_open(0);
	char *string = NULL;
	char *p = NULL;
	int i;
	for (i = 0; i < argc; i++) {
		if (i > 0)
			strbuf_putc(sb, ' ');
		for (p = argv[i]; *p; p++) {
			/* quote spaces using url encoding */
			if (*p == ' ')
				strbuf_puts(sb, "%20");
			else
				strbuf_putc(sb, *p);
		}
	}
	string = check_strdup(strbuf_value(sb));
	strbuf_close(sb);
	return string;
}
