/*
 * Copyright (c) 1996, 1997, 1998, 1999
 *            Shigio Yamaguchi. All rights reserved.
 * Copyright (c) 1999
 *            Tama Communications Corporation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Tama Communications
 *      Corporation and its contributors.
 * 4. Neither the name of the author nor the names of any co-contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *      global.c                                12-Dec-99
 *
 */

#include <sys/types.h>
#include <sys/stat.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "global.h"
#include "regex.h"

const char *progname  = "global";		/* command name */

static void	usage(void);
static void	setcom(int);
int	main(int, char **);
void	makefilter(char *);
FILE	*openfilter(void);
void	closefilter(FILE *);
void	completion(char *, char *, char *);
void	relative_filter(STRBUF *, char *, char *);
void	glimpse(char *, char *);
void	grep(char *, char *);
void	pathlist(char *, char *);
void	parsefile(int, char **, char *, char *, char *, int);
void	printtag(FILE *, char *);
int	search(char *, char *, char *, int);
int	includepath(char *, char *);
void	ffformat(char *, char *);

char	sort_command[MAXFILLEN+1];	/* sort command		*/
char	sed_command[MAXFILLEN+1];	/* sed command		*/
STRBUF	*sortfilter;			/* sort filter		*/
STRBUF	*pathfilter;			/* path convert filter	*/
char	*localprefix;			/* local prefix		*/
int	aflag;				/* [option]		*/
int	cflag;				/* command		*/
int	fflag;				/* command		*/
int	gflag;				/* command		*/
int	iflag;				/* command		*/
int	lflag;				/* [option]		*/
int	nflag;				/* [option]		*/
int	pflag;				/* command		*/
int	Pflag;				/* command		*/
int	rflag;				/* [option]		*/
int	sflag;				/* [option]		*/
int	tflag;				/* [option]		*/
int	vflag;				/* [option]		*/
int	xflag;				/* [option]		*/
int	pfilter;			/* undocumented command */

static void
usage()
{
	fprintf(stderr, "usage: ");
	fprintf(stderr, "\t%s\n", "global [-alnrstvx] pattern");
	fprintf(stderr, "\t%s\n", "global -c[s] [prefix]");
	fprintf(stderr, "\t%s\n", "global -f[alnrstx] files");
	fprintf(stderr, "\t%s\n", "global -g[alntvx] pattern");
	fprintf(stderr, "\t%s\n", "global -i[v]");
	fprintf(stderr, "\t%s\n", "global -p[v]");
	fprintf(stderr, "\t%s\n", "global -P[alntx] [pattern]");
	exit(1);
}

static int	command;
static void
setcom(c)
int	c;
{
	if (command == 0)
		command = c;
	else
		usage();
}
int
main(argc, argv)
int	argc;
char	*argv[];
{
	char	*av;
	int	count;
	int	db;
	int	optchar;
	int	option_index = 0;
	char	cwd[MAXPATHLEN+1];		/* current directory	*/
	char	root[MAXPATHLEN+1];		/* root of source tree	*/
	char	dbpath[MAXPATHLEN+1];		/* dbpath directory	*/

	while (--argc > 0 && (++argv)[0][0] == '-') {
		char	*p;

		if (!strcmp(argv[0], "--filter")) {
			pfilter++;
			continue;
		} else if (!strcmp(argv[0], "--version")) {
			fprintf(stdout, "%s\n", VERSION);
			exit(0);
		}
		for (p = argv[0] + 1; *p; p++) {
			switch (*p) {
			case 'a':
				aflag++;
				break;
			case 'c':
				cflag++;
				setcom(*p);
				break;
			case 'f':
				fflag++;
				xflag++;
				setcom(*p);
				break;
			case 'l':
				lflag++;
				break;
			case 'n':
				nflag++;
				break;
			case 'g':
				gflag++;
				setcom(*p);
				break;
			case 'i':
				iflag++;
				setcom(*p);
				break;
			case 'p':
				pflag++;
				setcom(*p);
				break;
			case 'P':
				Pflag++;
				setcom(*p);
				break;
			case 'r':
				rflag++;
				break;
			case 's':
				sflag++;
				break;
			case 't':
				tflag++;
				break;
			case 'v':
				vflag++;
				break;
			case 'x':
				xflag++;
				break;
			default:
				usage();
				break;
			}
		}
	}
	av = (argc > 0) ? *argv : NULL;
	/*
	 * invalid options.
	 */
	if (sflag && rflag)
		die("both of -s and -r are not allowed.");
	/*
	 * only -c, -i, -P and -p allows no argment.
	 */
	if (!av && !pfilter) {
		switch (command) {
		case 'c':
		case 'i':
		case 'p':
		case 'P':
			break;
		default:
			usage();
			break;
		}
	}
	/*
	 * -i and -p cannot have any arguments.
	 */
	if (av) {
		switch (command) {
		case 'i':
		case 'p':
			usage();
		default:
			break;
		}
	}
	/*
	 * remove leading blanks.
	 */
	if (av && !gflag) {
		for (; *av == ' ' || *av == '\t'; av++)
			;
	}
	if (cflag && av && notnamechar(av))
		die("only name char is allowed with -c option.");
	/*
	 * get path of following directories.
	 *	o current directory
	 *	o root of source tree
	 *	o dbpath directory
	 *
	 * if GTAGS not found, getdbpath doesn't return.
	 */
	if (pflag && vflag) {
		char *gtagsdbpath = getenv("GTAGSDBPATH");
		char *gtagsroot = getenv("GTAGSROOT");
		if (gtagsdbpath && !gtagsroot)
			fprintf(stdout, "warning: GTAGSDBPATH is ignored becase GTAGSROOT is not set.\n");
	}
	getdbpath(cwd, root, dbpath);
	/*
	 * print dbpath.
	 */
	if (pflag) {
		fprintf(stdout, "%s\n", dbpath);
		exit(0);
	}
	/*
	 * incremental update of tag files.
	 */
	if (iflag) {
		STRBUF	*sb = stropen();

		if (chdir(root) < 0)
			die1("cannot change directory to '%s'.", root);
		strputs(sb, "gtags -i");
		if (vflag)
			strputc(sb, 'v');
		strputc(sb, ' ');
		strputs(sb, dbpath);
		if (system(strvalue(sb)))
			exit(1);
		strclose(sb);
		exit(0);
	}

	/*
	 * complete function name
	 */
	if (cflag) {
		completion(dbpath, root, av);
		exit(0);
	}
	/*
	 * get command name of sort and sed.
	 */
	{
		STRBUF	*sb = stropen();
		if (!getconfs("sort_command", sb))
			die("cannot get sort command name.");
#ifdef _WIN32
		if (!locatestring(strvalue(sb), ".exe", MATCH_LAST))
			strputs(sb, ".exe");
#endif
		strcpy(sort_command, strvalue(sb));
		strstart(sb);
		if (!getconfs("sed_command", sb))
			die("cannot get sed command name.");
#ifdef _WIN32
		if (!locatestring(strvalue(sb), ".exe", MATCH_LAST))
			strputs(sb, ".exe");
#endif
		strcpy(sed_command, strvalue(sb));
		strclose(sb);
	}
	/*
	 * make local prefix.
	 */
	if (lflag) {
		char	*p = cwd + strlen(root);
		STRBUF	*sb = stropen();
		/*
		 * getdbpath() assure follows.
		 * cwd != "/" and cwd includes root.
		 */
		strputc(sb, '.');
		if (*p)
			strputs(sb, p);
		strputc(sb, '/');
		localprefix = strdup(strvalue(sb));
		if (!localprefix)
			die("short of memory.");
		strclose(sb);
	} else {
		localprefix = NULL;
	}
	/*
	 * make sort filter.
	 */
	sortfilter = stropen();
	strputs(sortfilter, sort_command);
	strputc(sortfilter, ' ');
	if (tflag) 			/* ctags format */
		strputs(sortfilter, "+0 -1 +1 -2 +2n -3");
	else if (fflag)
		strputs(sortfilter, "+2 -3 +1n -2");
	else if (xflag)			/* print details */
		strputs(sortfilter, "+0 -1 +2 -3 +1n -2");
	else				/* print just file name */
		strputs(sortfilter, "-u");
	/*
	 * make path filter.
	 */
	pathfilter = stropen();
	if (aflag) {				/* absolute path name */
		strputs(pathfilter, sed_command);
		strputc(pathfilter, ' ');
		strputs(pathfilter, "-e \"s@\\.@");
		strputs(pathfilter, root);
		strputs(pathfilter, "@\"");
	} else if (lflag) {
		strputs(pathfilter, sed_command);
		strputc(pathfilter, ' ');
		strputs(pathfilter, "-e \"s@\\");
		strputs(pathfilter, localprefix);
		strputs(pathfilter, "@@\"");
	} else {				/* relative path name */
		relative_filter(pathfilter, root, cwd);
	}
	/*
	 * print filter.
	 */
	if (pfilter) {
		char    filter[MAXFILLEN+1];

		makefilter(filter);
		fprintf(stdout, "%s\n", filter);
		exit(0);
	}
	/*
	 * grep the pattern in a source tree.
	 */
	if (gflag) {
		chdir(root);
		grep(av, dbpath);
		exit(0);
	}
	/*
	 * locate the path including the pattern in a source tree.
	 */
	if (Pflag) {
		pathlist(dbpath, av);
		exit(0);
	}
	db = (rflag) ? GRTAGS : ((sflag) ? GSYMS : GTAGS);
	/*
	 * print function definitions.
	 */
	if (fflag) {
		parsefile(argc, argv, cwd, root, dbpath, db);
		exit(0);
	}
	/*
	 * search in current source tree.
	 */
	count = search(av, root, dbpath, db);
	/*
	 * search in library path.
	 */
	if (count == 0 && !lflag && !rflag && !sflag && !notnamechar(av) && getenv("GTAGSLIBPATH")) {
		char	buf[MAXENVLEN+1];
		char	libdbpath[MAXPATHLEN+1];
		char	*p, *lib;

		strcpy(buf, getenv("GTAGSLIBPATH"));
		p = buf;
		while (p) {
			lib = p;
			if ((p = locatestring(p, PATHSEP, MATCH_FIRST)) != NULL)
				*p++ = 0;
			if (!gtagsexist(lib, libdbpath))
				continue;
			if (!strcmp(dbpath, libdbpath))
				continue;
			if (aflag) {		/* absolute path name */
				strstart(pathfilter);
				strputs(pathfilter, sed_command);
				strputc(pathfilter, ' ');
				strputs(pathfilter, "-e \"s@\\.@");
				strputs(pathfilter, lib);
				strputs(pathfilter, "@\"");
			} else {
				strstart(pathfilter);
				relative_filter(pathfilter, lib, cwd);
			}
			count = search(av, lib, libdbpath, db);
			if (count > 0) {
				strcpy(dbpath, libdbpath);
				break;
			}
		}
	}
	if (vflag) {
		if (count) {
			if (count == 1)
				fprintf(stderr, "%d object located", count);
			if (count > 1)
				fprintf(stderr, "%d objects located", count);
		} else {
			fprintf(stderr, "'%s' not found", av);
		}
		fprintf(stderr, " (using '%s').\n", makepath(dbpath, dbname(db), NULL));
	}
	exit(0);
}
/*
 * makefilter: make filter string.
 *
 *	o)	filter buffer
 */
void
makefilter(filter)
char	*filter;
{
	if (nflag)
		filter[0] = 0;
	else if (!strbuflen(sortfilter) && !strbuflen(pathfilter))
		filter[0] = 0;
	else if (strbuflen(sortfilter) && strbuflen(pathfilter))
		sprintf(filter, "%s | %s", strvalue(sortfilter), strvalue(pathfilter));
	else if (strbuflen(sortfilter))
		strcpy(filter, strvalue(sortfilter));
	else
		strcpy(filter, strvalue(pathfilter));
}
/*
 * openfilter: open output filter.
 *
 *	gi)	pathfilter
 *	gi)	sortfilter
 *	r)		file pointer for output filter
 */
FILE	*
openfilter(void)
{
	FILE	*op;
	char	filter[MAXFILLEN+1];

	makefilter(filter);
	if (filter[0] == 0)
		op = stdout;
	else
		op = popen(filter, "w");
	return op;
}
void
closefilter(op)
FILE	*op;
{
	if (op != stdout)
		pclose(op);
}
/*
 * completion: print completion list of specified prefix
 *
 *	i)	dbpath	dbpath directory
 *	i)	root	root directory
 *	i)	prefix	prefix of primary key
 */
void
completion(dbpath, root, prefix)
char	*dbpath;
char	*root;
char	*prefix;
{
	char	*p;
	int	flags = GTOP_KEY;
	GTOP	*gtop;
	int	db;

	if (prefix && *prefix == 0)	/* In the case global -c '' */
		prefix = NULL;
	if (prefix)
		flags |= GTOP_PREFIX;
	db = (sflag) ? GSYMS : GTAGS;
	gtop = gtagsopen(dbpath, root, db, GTAGS_READ, 0);
	for (p = gtagsfirst(gtop, prefix, flags); p; p = gtagsnext(gtop))
		(void)fprintf(stdout, "%s\n", p);
	gtagsclose(gtop);
}
/*
 * relative_filter: make relative path filter
 *
 *	o)	sb	buffer for the result
 *	i)	root	the root directory of source tree
 *	i)	cwd	current directory
 */
void
relative_filter(sb, root, cwd)
STRBUF	*sb;
char	*root;
char	*cwd;
{
	char	*p, *c, *branch;

	/*
	 * get branch point.
	 */
	branch = cwd;
	for (p = root, c = cwd; *p && *c && *p == *c; p++, c++)
		if (*c == '/')
			branch = c;
	if (*p == 0 && (*c == 0 || *c == '/'))
		branch = c;
	/*
	 * forward to root.
	 */
	strputs(sb, sed_command);
	strputc(sb, ' ');
	strputs(sb, "-e \"s@\\./@");
	for (c = branch; *c; c++)
		if (*c == '/')
			strputs(sb, "../");
	p = root + (branch - cwd);
	/*
	 * backward to leaf.
	 */
	if (*p) {
		p++;
		strputs(sb, p);
		strputc(sb, '/');
	}
	strputs(sb, "@\"");
	/*
	 * remove redundancy.
	 */
	if (*branch) {
		char	unit[256];

		p = unit;
		for (c = branch + 1; ; c++) {
			if (*c == 0 || *c == '/') {
				*p = 0;
				strputs(sb, " -e \"s@\\.\\./");
				strputs(sb, unit);
				strputs(sb, "/@@\"");
				if (*c == 0)
					break;
				p = unit;
			} else
				*p++ = *c;
		}
	}
}
/*
 * printtag: print a tag's line
 *
 *	i)	op	output stream
 *	i)	bp	tag's line
 */
void
printtag(op, bp)
FILE	*op;
char	*bp;
{
	if (tflag) {
		char	buf[MAXBUFLEN+1];
		char	lno[20], *l = lno;
		char	*p = bp;
		char	*q = buf;

		while (*p && !isspace(*p))
			*q++ = *p++;
		*q++ = '\t';
		for (; *p && isspace(*p); p++)
			;
		while (*p && isdigit(*p))
			*l++ = *p++;
		*l = 0;
		for (; *p && isspace(*p); p++)
			;
		while (*p && !isspace(*p))
			*q++ = *p++;
		*q++ = '\t';
		l = lno;
		while (*l)
			*q++ = *l++;
		*q = 0;
		fprintf(op, "%s\n", buf); 
	} else if (!xflag) {
		char	*p = locatestring(bp, "./", MATCH_FIRST);

		if (p == NULL)
			die("illegal tag format (path not found).");
		fputs(strmake(p, " \t"), op);
		(void)putc('\n', op);
	} else
		detab(op, bp);
}
/*
 * grep: grep pattern
 *
 *	i)	pattern	POSIX regular expression
 *	i)	dbpath	GTAGS directory
 */
void
grep(pattern, dbpath)
char	*pattern;
char	*dbpath;
{
	FILE	*op, *fp;
	char	*path;
	char	edit[IDENTLEN+1];
	char	*buffer;
	int	linenum, count, editlen;
	regex_t	preg;

	/*
	 * convert spaces into %FF format.
	 */
	ffformat(edit, pattern);
	editlen = strlen(edit);

	if (regcomp(&preg, pattern, REG_EXTENDED) != 0)
		die("illegal regular expression.");
	if (!(op = openfilter()))
		die("cannot open output filter.");
	count = 0;
	for (gfindopen(dbpath, localprefix); (path = gfindread()) != NULL; ) {
		if (!(fp = fopen(path, "r")))
			die1("cannot open file '%s'.", path);
		linenum = 0;
		while ((buffer = mgets(fp, NULL, 0)) != NULL) {
			linenum++;
			if (regexec(&preg, buffer, 0, 0, 0) == 0) {
				count++;
				if (tflag)
					fprintf(op, "%s\t%s\t%d\n",
						edit, path, linenum);
				else if (!xflag) {
					fprintf(op, "%s\n", path);
					break;
				} else if (editlen >= 16 && linenum >= 1000)
					fprintf(op, "%-16s %4d %-16s %s\n",
						edit, linenum, path, buffer);
				else
					fprintf(op, "%-16s%4d %-16s %s\n",
						edit, linenum, path, buffer);
			}
		}
		fclose(fp);
	}
	gfindclose();
	closefilter(op);
	if (vflag) {
		if (count == 0)
			fprintf(stderr, "object not found");
		if (count == 1)
			fprintf(stderr, "%d object located", count);
		if (count > 1)
			fprintf(stderr, "%d objects located", count);
		fprintf(stderr, " (no index used).\n");
	}
}
/*
 * pathlist: print candidate path list.
 *
 *	i)	dbpath
 */
void
pathlist(dbpath, av)
char	*dbpath;
char	*av;
{
	FILE	*op;
	char	key[10], *path, *p;
	regex_t preg;
	int	count;
	int	i, lim;

	if (av) {
		if (regcomp(&preg, av, REG_EXTENDED) != 0)
			die("illegal regular expression.");
	}
	if (!localprefix)
		localprefix = "./";
	if (!(op = openfilter()))
		die("cannot open output filter.");
	count = 0;
	for (gfindopen(dbpath, localprefix); (path = gfindread()) != NULL; ) {
		/*
		 * skip './' and localprefix because end-user doesn't see it.
		 */
		p = path + strlen(localprefix);
		if (av && regexec(&preg, p, 0, 0, 0) != 0)
			continue;
		if (xflag)
			fprintf(op, "path\t1 %s \n", path);
		else if (tflag)
			fprintf(op, "path\t%s\t1\n", path);
		else
			fprintf(op, "%s\n", path);
		count++;
	}
	gfindclose();
	closefilter(op);
	if (vflag) {
		if (count == 0)
			fprintf(stderr, "path not found");
		if (count == 1)
			fprintf(stderr, "%d path located", count);
		if (count > 1)
			fprintf(stderr, "%d paths located", count);
		fprintf(stderr, " (using '%s').\n", makepath(dbpath, dbname(GPATH), NULL));
	}
}
/*
 * parsefile: parse file to pick up tags.
 *
 *	i)	db
 *	i)	dbpath
 *	i)	root
 *	i)	cwd
 *	i)	argc
 *	i)	argv
 */
void
parsefile(argc, argv, cwd, root, dbpath, db)
int	argc;
char	**argv;
char	*cwd;
char	*root;
char	*dbpath;
int	db;
{
	char	buf[MAXPATHLEN+1], *path;
	char	env[MAXPATHLEN+1];
	char	*p;
	FILE	*ip, *op;
	char	*parser, *av;
	STRBUF  *sb = stropen();
	STRBUF	*com = stropen();

	/*
	 * teach parser where is dbpath.
	 */
	sprintf(env, "GTAGSDBPATH=%s", dbpath);
	putenv(env);

	/*
	 * get parser.
	 */
	if (!getconfs(dbname(db), sb))
		die1("cannot get parser for %s.", dbname(db));
	parser = strvalue(sb);

	if (!(op = openfilter()))
		die("cannot open output filter.");
	if (pathopen(dbpath, 0) < 0)
		die("GPATH not found.");
	for (; argc > 0; argv++, argc--) {
		av = argv[0];

		if (test("d", av)) {
			fprintf(stderr, "'%s' is a directory.\n", av);
			continue;
		}
		if (!test("f", NULL)) {
			fprintf(stderr, "'%s' not found.\n", av);
			continue;
		}
		/*
		 * convert path into relative from root directory of source tree.
		 */
		path = realpath(av, buf);
		if (!isabspath(path))
			die("realpath(3) is not compatible with BSD version.");
		if (strncmp(path, root, strlen(root))) {
			fprintf(stderr, "'%s' is out of source tree.\n", path);
			continue;
		}
		path += strlen(root) - 1;
		*path = '.';
		if (!pathget(path)) {
			fprintf(stderr, "'%s' not found in GPATH.\n", path);
			continue;
		}
		if (chdir(root) < 0)
			die1("cannot move to '%s' directory.", root);
		/*
		 * make command line.
		 */
		strstart(com);
		makecommand(parser, path, com);
		if (!(ip = popen(strvalue(com), "r")))
			die1("cannot execute '%s'.", strvalue(com));
		while ((p = mgets(ip, NULL, 0)) != NULL)
			printtag(op, p);
		pclose(ip);
		if (chdir(cwd) < 0)
			die1("cannot move to '%s' directory.", cwd);
	}
	pathclose();
	closefilter(op);
	strclose(com);
	strclose(sb);
}
/*
 * search: search specified function 
 *
 *	i)	pattern		search pattern
 *	i)	root		root of source tree
 *	i)	dbpath		database directory
 *	i)	db		GTAGS,GRTAGS,GSYMS
 *	r)			count of output lines
 */
int
search(pattern, root, dbpath, db)
char	*pattern;
char	*root;
char	*dbpath;
int	db;
{
	char	*p;
	int	count = 0;
	FILE	*op;
	GTOP	*gtop;
	regex_t	preg;

	/*
	 * open tag file.
	 */
	gtop = gtagsopen(dbpath, root, db, GTAGS_READ, 0);
	if (!(op = openfilter()))
		die("cannot open output filter.");
	/*
	 * search through tag file.
	 */
	for (p = gtagsfirst(gtop, pattern, 0); p; p = gtagsnext(gtop)) {
		if (lflag) {
			char	*q;
			/* locate start point of a path */
			q = locatestring(p, "./", MATCH_FIRST);
			if (!locatestring(q, localprefix, MATCH_AT_FIRST))
				continue;
		}
		printtag(op, p);
		count++;
	}
	closefilter(op);
	gtagsclose(gtop);
	return count;
}
/*
 * includepath: check if the path included in tag line or not.
 *
 *	i)	line	tag line
 *	i)	path	path
 *	r)		0: doesn't included, 1: included
 */
int
includepath(line, path)
char	*line;
char	*path;
{
	char	*p;
	int	length;

	if (!(p = locatestring(line, "./", MATCH_FIRST)))
		die("illegal tag format (path not found).");
	length = strlen(path);
	if (strncmp(p, path, length))
		return 0;
	p += length;
	if (*p == ' ' || *p == '\t')
		return 1;
	return 0;
}
/*
 * ffformat: string copy with converting blank chars into %ff format.
 *
 *	o)	to	result
 *	i)	from	string
 */
void
ffformat(to, from)
char	*to;
char	*from;
{
	char	*p, *e = to;

	for (p = from; *p; p++) {
		if (*p == '%' || *p == ' ' || *p == '\t') {
			sprintf(e, "%%%02x", *p);
			e += 3;
		} else
			*e++ = *p;
	}
	*e = 0;
}
