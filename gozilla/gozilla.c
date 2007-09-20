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
 *	gozilla.c				29-Aug-99
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#ifdef _WIN32
#include <windows.h>
#endif

#include "global.h"

const char *progname  = "gozilla";		/* command name */

static void	usage(void);

int	main(int, char **);
int	isprotocol(char *);
int	issource(char *);
int	convertpath(char *, char *, char *, STRBUF *);
void	loadmozilla(char *);
int	sendmozilla(char *, int);
#ifndef _WIN32
int	sendcommand(char *);
#endif

int	bflag;
int	pflag;
int	Cflag;
int	linenumber = 0;

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
	char	URL[MAXPATHLEN+1];
	char	com[MAXFILLEN+1];
	int	isfile = 1;
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
#ifndef _WIN32
		if (sendcommand(command) == -1)
			die("mozilla not found.");
#else
		die("-C option isn't supported in WIN32 environment.");
#endif
		exit(0);
	}
	if (argc == 0)
		usage();

	if (isprotocol(argv[0])) {
		isfile = 0;
		strcpy(URL, argv[0]);
	} else {
		char	*abspath;
		char	buf[MAXPATHLEN+1];

		if (!test("f", argv[0]) && !test("d", NULL))
			die1("path '%s' not found.", argv[0]);
		if (!(abspath = realpath(argv[0], buf)))
			die1("cannot make absolute path name. realpath(%s) failed.", argv[0]);
		if (!isabspath(abspath))
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
			if (test("d", makepath(dbpath, "HTML", NULL)))
				strcpy(htmldir, makepath(dbpath, "HTML", NULL));
			else if (test("d", makepath(root, "HTML", NULL)))
				strcpy(htmldir, makepath(root, "HTML", NULL));
			else
				die("hypertext not found. See htags(1).");
			/*
			 * convert path into hypertext.
			 */
			p = abspath + strlen(root);
			if (convertpath(dbpath, htmldir, p, sb) == -1)
				die("cannot find the hypertext.");
			if (linenumber)
				sprintf(URL, "%s#%d", strvalue(sb), linenumber);
			else
				strcpy(URL, strvalue(sb));
		} else {
			strcpy(URL, abspath);
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
	status = sendmozilla(URL, isfile);
	/*
	 * load mozilla if not found.
	 */
	if (status != 0) {
		loadmozilla(URL);
		exit(0);
	}
	exit(status);
}
/*
 * isprotocol: return 1 if url has a procotol.
 *
 *	i)	url	URL
 *	r)		1: protocol, 0: file
 */
int
isprotocol(url)
char	*url;
{
	char	*p;
	/*
	 * protocol's style is like http://xxx.
	 */
	for (p = url; *p && *p != ':'; p++)
		if (!isalnum(*p))
			return 0;
	if (!*p)
		return 0;
	if (*p++ == ':' && *p++ == '/' && *p == '/')
		return 1;
	return 0;
}
/*
 * issource: return 1 if path is a source file.
 *
 *	i)	path	path
 *	r)		1: source file, 0: not source file
 */
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
/*
 * convertpath: convert source file into hypertext path.
 *
 *	i)	dbpath	dbpath
 *	i)	htmldir	HTML directory made by htags(1)
 *	i)	path	source file path
 *	o)	sb	string buffer
 *	r)		0: normal, -1: error
 */
int
convertpath(dbpath, htmldir, path, sb)
char	*dbpath;
char	*htmldir;
char	*path;
STRBUF	*sb;
{
	static const char *suffix[] = {".html", ".htm"};
	static const char *gz = ".gz";
	int i, lim = sizeof(suffix)/sizeof(char *);
	char *p;

	strstart(sb);
	strputs(sb, htmldir);
	strputs(sb, "/S/");
	/*
	 * new style.
	 */
	if (pathopen(dbpath, 0) == 0) {
		char key[MAXPATHLEN+1];

		strcpy(key, "./");
		strcat(key, path + 1);
		p = pathget(key);
		if (p == NULL) {
			pathclose();
			return -1;
		}
		strcpy(key, p);
		pathclose();
		strputs(sb, key);
		for (i = 0; i < lim; i++) {
			strputs(sb, suffix[i]);
			if (test("f", strvalue(sb)))
				return 0;
			strputs(sb, gz);
			if (test("f", strvalue(sb)))
				return 0;
			strpushback(sb, strlen(gz));
			strpushback(sb, strlen(suffix[i]));
		}
		strpushback(sb, strlen(key));
	}
	/*
	 * old style.
	 */
	for (p = path + 1; *p; p++)
		strputc(sb, (*p == '/') ? ' ' : *p);
	for (i = 0; i < lim; i++) {
		strputs(sb, suffix[i]);
		if (test("f", strvalue(sb)))
			return 0;
		strputs(sb, gz);
		if (test("f", strvalue(sb)))
			return 0;
		strpushback(sb, strlen(gz));
		strpushback(sb, strlen(suffix[i]));
	}
	return -1;
}
/*
 * sendmozilla: send URL to mozilla.
 *
 *	i)	url	url or path
 *	i)	isfile	1: file, 0: not file (protocol)
 *	r)		-1: error, 0: normal
 *
 * Imprementation is based on
 * [UNIX: X-property]
 * See http://home.netscape.com/newsref/std/x-remote.html
 * [WIN32: DDE]
 * See http://developer.netscape.com/docs/manuals/communicator/DDE/abtdde.htm
 */
#ifdef _WIN32
HDDEDATA CALLBACK DdemlCallback(hwnd, umsg, wparam, lparam)
        HWND    hwnd;
        UINT    umsg;
        WPARAM  wparam;
        LPARAM  lparam;
{
	return(NULL);
}

int
sendmozilla(url, isfile)
	char	*url;
	int	isfile;
{
	char	com[MAXFILLEN+1];
	int	status;
	DWORD dde     = 0;
	HSZ   topic   = NULL;
	HSZ   service = NULL;
	HSZ   command = NULL;
	HCONV hconv   = NULL;

	/* load and initialize DDEML */
	if (DdeInitialize(&dde, (PFNCALLBACK)MakeProcInstance((FARPROC)DdemlCallback, dde),
		APPCLASS_STANDARD | APPCMD_CLIENTONLY, 0) != 0)
		die("Could not initialize DDEML.");

	/* setup strings */
	service = DdeCreateStringHandle(dde, "Netscape", CP_WINANSI);
	topic   = DdeCreateStringHandle(dde, "WWW_OpenURL", CP_WINANSI);

	/* connect to server */
	hconv = DdeConnect(dde, service, topic, NULL);
	if (!hconv)
		/* Could not connect to mozilla. We need to load mozilla. */
		return -1;

	if (isfile)
		sprintf(com, "%s,,0xFFFFFFFF", url);
	else
		sprintf(com, "%s,,0xFFFFFFFF,0x4", url);
	command = DdeCreateStringHandle(dde, com, CP_WINANSI);
	/* send message to server */
	if (!DdeClientTransaction((LPVOID)NULL, 0L, hconv, command,
					CF_TEXT, XTYP_REQUEST, 1000L, NULL))
		die1("Could not execute transaction(errorno = 0x%04x)\n", DdeGetLastError(dde));

	DdeFreeStringHandle(dde, service);
	DdeFreeStringHandle(dde, topic);
	DdeFreeStringHandle(dde, command);

	/* close connection */
	DdeDisconnect(hconv);
	DdeUninitialize(dde);

	return 0;
}
#else
int
sendmozilla(url, isfile)
char	*url;
int	isfile;
{
	char	com[MAXFILLEN+1];

	if (isfile)
		sprintf(com, "openFile(%s)", url);
	else
		sprintf(com, "openURL(%s)", url);

	return sendcommand(com);
}
int
sendcommand(com)
	char	*com;
{
	char	*argv[5];
	int	argc = 3;

	argv[0] = "netscape-remote";
	argv[1] = "-raise";
	argv[2] = "-remote";
	argv[3] = com;
	argv[4] = NULL;

	return netscape_remote(argc, argv);
}
#endif
/*
 * loadmozilla: load mozilla
 *
 *	i)	url	URL
 *
 */
#ifdef _WIN32
void
loadmozilla(url)
char	*url;
{
	char	com[1024], *path;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	if (!(path = usable("netscape")))
		die("netscape not found in your path.");
	
	ZeroMemory(&si, sizeof(STARTUPINFO));
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOWNORMAL;
	sprintf(com, "netscape %s", url);
	if (!CreateProcess(path, com, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
		die1("Cannot load mozilla.(error = 0x%04x)\n", GetLastError());
	exit(0);
}
#else
void
loadmozilla(url)
char	*url;
{
	int	pid;

	if ((pid = fork()) < 0) {
		die("cannot load mozilla (fork).");
	} else if (pid == 0) {
		execlp("netscape", "netscape", url, NULL);
		die("cannot load mozilla (execlp).");
	}
}
#endif
