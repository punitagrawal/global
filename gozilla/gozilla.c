/*
 * Copyright (c) 1996, 1997, 1998, 1999
 *				Shigio Yamaguchi. All rights reserved.
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
 *	This product includes software developed by Shigio Yamaguchi.
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
 *	gozilla.c				10-Feb-99
 *
 *	- modified by Ron Lee <ron@microtronics.com.au>         24-Feb-99
 *	  to make convertpath recognise html.gz and .ghtml
 *	  suffixes.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"

const char *progname  = "gozilla";		/* command name */

static void	usage __P((void));

int	main __P((int, char **));
int	issource __P((char *));
int	validsuffix __P((STRBUF *));
int	convertpath __P((char *, char *, char *, STRBUF *));
int	sendcommand __P((char *));

int	bflag;
int	pflag;
int	Cflag;

static void
usage()
{
	fprintf(stderr, "usage:\t%s\n\t%s\n",
		"gozilla [+no] [-b browser] [-p] file",
		"gozilla -C command");
	exit(1);
}

int
main(argc, argv)
int	argc;
char	*argv[];
{
	char	c, *p, *q;
	char	*browser = NULL;
	char	*command = NULL;
	char	*arg     = NULL;
	char	URL[MAXPATHLEN+1];
	char	com[MAXFILLEN+1];
	int	linenumber = 0;
	int	status;

	while (--argc > 0 && (c = (++argv)[0][0]) == '-' || c == '+') {
		if (c == '+') {
			linenumber = atoi(argv[0] + 1);
			continue;
		}
		p = argv[0] + 1;
		switch (*p) {
		case 'b':
			browser = argv[1];
			--argc; ++argv;
			break;
		case 'p':
			pflag++;
			break;
		case 'C':
			command = argv[1];
			--argc; ++argv;
			break;
		default:
			usage();
		}
	}
	if (!browser && getenv("BROWSER"))
		browser = getenv("BROWSER");
	if (command) {
		if (browser)
			die("-C option is valid only for mozilla.");
		if (sendcommand(command) == -1)
			die("mozilla not found.");
		exit(0);
	}
	if (argc == 0)
		usage();
	if (locatestring(argv[0], "http:", MATCH_AT_FIRST) ||
		locatestring(argv[0], "ftp:", MATCH_AT_FIRST) ||
		locatestring(argv[0], "news:", MATCH_AT_FIRST) ||
		locatestring(argv[0], "mail:", MATCH_AT_FIRST) ||
		locatestring(argv[0], "file:", MATCH_AT_FIRST))
		strcpy(URL, argv[0]);
	else {
		char	*abspath;
		char	buf[MAXPATHLEN+1];

		if (!test("f", argv[0]) && !test("d", NULL))
			die1("path '%s' not found.", argv[0]);
		if (!(abspath = realpath(argv[0], buf)))
			die1("cannot make absolute path name. realpath(%s) failed.", argv[0]);
		if (*abspath != '/')
			die("realpath(3) is not compatible with BSD version.");
		if (issource(abspath)) {
			char	cwd[MAXPATHLEN+1];
			char	root[MAXPATHLEN+1];
			char	dbpath[MAXPATHLEN+1];
			char	htmldir[MAXPATHLEN+1];
			STRBUF *sb = stropen();
			/*
			 * get current, root and dbpath directory.
			 * if GTAGS not found, getdbpath doesn't return.
			 */
			getdbpath(cwd, root, dbpath);
			if (test("d", makepath(dbpath, "HTML")))
				strcpy(htmldir, makepath(dbpath, "HTML"));
			else if (test("d", makepath(root, "HTML")))
				strcpy(htmldir, makepath(root, "HTML"));
			else
				die("hypertext not found. See htags(1).");
			/*
			 * convert path into hypertext.
			 */
			p = abspath + strlen(root);
			if (convertpath(dbpath, htmldir, p, sb) == -1)
				die("cannot find the hypertext.");
			if (linenumber)
				sprintf(URL, "file:%s#%d", strvalue(sb), linenumber);
			else
				sprintf(URL, "file:%s", strvalue(sb));
		} else {
			sprintf(URL, "file:%s", abspath);
		}
	}
	if (pflag) {
		fprintf(stdout, "%s\n", URL);
		exit(0);
	}
	/*
	 * execute generic browser.
	 */
	if (browser && !locatestring(browser, "netscape", MATCH_AT_LAST)) {
		sprintf(com, "%s '%s'", browser, URL);
		system(com);
		exit (0);
	}
	/*
	 * send a command to mozilla.
	 */
	sprintf(com, "openURL(%s)", URL);
	status = sendcommand(com);
	/*
	 * load mozilla if not found.
	 */
	if (status != 0) {
		int	pid;

		if ((pid = fork()) < 0) {
			die("cannot execute netscape (fork).");
		} else if (pid == 0) {
			execlp("netscape", "netscape", URL, NULL);
			die("loading mozilla failed.");
		}
		exit(0);
	}
	exit(status);
}
int
issource(path)
char	*path;
{
	STRBUF	*sb = stropen();
	char	*p;
	char	suff[MAXPATHLEN+1];
	int	retval = 0;

	if (!getconfs("suffixes", sb)) {
		strclose(sb);
		return 0;
	}
	suff[0] = '.';
	for (p = strvalue(sb); p; ) {
		char    *unit = p;
		if ((p = locatestring(p, ",", MATCH_FIRST)) != NULL)
			*p++ = 0;
		strcpy(&suff[1], unit);
		if (locatestring(path, suff, MATCH_AT_LAST)) {
			retval = 1;
			break;
		}
	}
	strclose(sb);
	return retval;

}
int
validsuffix(sb)
STRBUF	*sb;
{
	static const char *s[] = {".html", ".htm", ".html.gz", ".ghtml", NULL};
	int i=0;

	while(s[i]) {
		strputs(sb,s[i]);
		if (test("f", strvalue(sb)))
			return 1;
		strpushback(sb, strlen(s[i]));
		i++;
	}
	return 0;
}
int
convertpath(dbpath, htmldir, path, sb)
char	*dbpath;
char	*htmldir;
char	*path;
STRBUF	*sb;
{
	char buf[MAXPATHLEN+1], *q = buf;
	char *p;

	/*
	 * new style.
	 */
	strstart(sb);
	strcpy(buf, "./");
	strcat(buf, path + 1);
	if (pathopen(dbpath, 0))
		return -1;
	p = pathget(buf);
	pathclose();
	if (p == NULL)
		return -1;
	strputs(sb, htmldir);
	strputs(sb, "/S/");
	strputs(sb, p);
	if (validsuffix(sb))
		return 0;
	/*
	 * old style.
	 */
	strputs(sb, htmldir);
	strputs(sb, "/S/");
	for (p = path + 1; *p; p++)
		strputc(sb, (*p == '/') ? ' ' : *p);
	if (validsuffix(sb))
		return 0;

	return -1;
}
int
sendcommand(com)
char	*com;
{
	int	argc = 3;
	char	*argv[4];

	argv[0] = "netscape-remote";
	argv[1] = "-remote";
	argv[2] = com;
	argv[3] = NULL;

	return netscape_remote(argc, argv);
}
