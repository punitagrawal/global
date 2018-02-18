/*
 * Copyright (c) 2010, 2015, 2016
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
#include <sys/types.h>
#include <assert.h>
#include <ctype.h>
#include <errno.h>
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

#include "parser.h"
#include "checkalloc.h"
#include "conf.h"
#include "strbuf.h"

/*
 * Function layer plugin parser sample
 */

#define TERMINATOR		"###terminator###"
#define LANGMAP_OPTION		"--langmap="
#define INITIAL_BUFSIZE		1024

static FILE *ip, *op;
static char *linebuf;
static size_t bufsize;
/*
 * 'ctagscom' has the path of ctags command.
 * In the configuration phase, the default value is set by the configure script.
 * In execution phase, plug-in parser get the value from the configuration file
 * (gtags.conf, $HOME/.globalrc) if it is defined.
 */
#if defined(USE_EXTRA_FIELDS)
static char *ctagscom = UNIVERSAL_CTAGS;
static const char *ctagsnotfound = "Universal Ctags not found. Please see ./configure --help.";
#else
static char *ctagscom = EXUBERANT_CTAGS;
static const char *ctagsnotfound = "Exuberant Ctags not found. Please see ./configure --help.";
#endif

#ifdef __GNUC__
static void terminate_ctags(void) __attribute__((destructor));
#endif

static void
copy_langmap_converting_cpp(char *dst, const char *src)
{
	const char *p;

	if (strncmp(src, "cpp:", 4) == 0) {
		memcpy(dst, "c++:", 4);
		dst += 4;
		src += 4;
	}
	while ((p = strstr(src, ",cpp:")) != NULL) {
		memcpy(dst, src, p - src);
		dst += p - src;
		memcpy(dst, ",c++:", 5);
		dst += 5;
		src = p + 5;
	}
	strcpy(dst, src);
}

#if defined(_WIN32) && !defined(__CYGWIN__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <fcntl.h>
static HANDLE pid;
static char argv[] = "\" "
#if defined(USE_EXTRA_FIELDS)
	"\"--_xformat=%R %-16N %4n %-16F %C\" "
	"--extras=+r "
	"--fields=+r "
#else
	"--format=1 "
#endif
	"-xu "
	"--filter "
	"\"--filter-terminator=" TERMINATOR "\n\" "
	LANGMAP_OPTION;
static void
start_ctags(const struct parser_param *param)
{
	HANDLE opipe[2], ipipe[2];
	SECURITY_ATTRIBUTES sa;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	char* arg;
	char *path = param->getconf("ctagscom");

	if (path && strlen(path) > 0 && strcmp(path, "no") != 0)
		ctagscom = path;
	if (!ctagscom || !strlen(ctagscom) || !strcmp(ctagscom, "no"))
		param->die(ctagsnotfound);

	arg = malloc(1 + strlen(ctagscom) + sizeof(argv) + strlen(param->langmap));
	if (arg == NULL)
		param->die("short of memory.");
	*arg = '"';
	strcpy(arg+1, ctagscom);
	strcat(arg, argv);
	copy_langmap_converting_cpp(arg + strlen(arg), param->langmap);

	sa.nLength = sizeof(sa);
	sa.bInheritHandle = TRUE;
	sa.lpSecurityDescriptor = NULL;
	if (!CreatePipe(&opipe[0], &opipe[1], &sa, 0) ||
	    !CreatePipe(&ipipe[0], &ipipe[1], &sa, 0))
		param->die("CreatePipe failed.");
	SetHandleInformation(opipe[1], HANDLE_FLAG_INHERIT, 0);
	SetHandleInformation(ipipe[0], HANDLE_FLAG_INHERIT, 0);
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.hStdInput = opipe[0];
	si.hStdOutput = ipipe[1];
	si.hStdError = GetStdHandle(STD_ERROR_HANDLE);
	si.dwFlags = STARTF_USESTDHANDLES;
	if (!CreateProcess(NULL, arg, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
		param->die(ctagsnotfound);
	CloseHandle(opipe[0]);
	CloseHandle(ipipe[1]);
	CloseHandle(pi.hThread);
	pid = pi.hProcess;
	op = fdopen(_open_osfhandle((long)opipe[1], _O_WRONLY), "w");
	ip = fdopen(_open_osfhandle((long)ipipe[0], _O_RDONLY), "r");
	if (ip == NULL || op == NULL)
		param->die("fdopen failed.");

	bufsize = INITIAL_BUFSIZE;
	linebuf = malloc(bufsize);
	if (linebuf == NULL)
		param->die("short of memory.");
}
static void
terminate_ctags(void) {
	if (op == NULL)
		return;
	free(linebuf);
	fclose(op);
	fclose(ip);
	WaitForSingleObject(pid, INFINITE);
	CloseHandle(pid);
}
#else
#include <sys/wait.h>
static pid_t pid;
static char *argv[] = {
	"ctags",
	NULL,
#if defined(USE_EXTRA_FIELDS)
	"--_xformat=%R %-16N %4n %-16F %C",
	"--extras=+r",
	"--fields=+r",
#else
	"--format=1",
#endif
	"-xu",
	"--filter",
	"--filter-terminator=" TERMINATOR "\n",
	NULL
};
static void
start_ctags(const struct parser_param *param)
{
	int opipe[2], ipipe[2];
	char *path = param->getconf("ctagscom");

	if (path && strlen(path) > 0 && strcmp(path, "no") != 0)
		ctagscom = path;
	if (!ctagscom || !strlen(ctagscom) || !strcmp(ctagscom, "no"))
		param->die(ctagsnotfound);
	argv[0] = ctagscom;
	argv[1] = malloc(sizeof(LANGMAP_OPTION) + strlen(param->langmap));
	if (argv[1] == NULL)
		param->die("short of memory.");
	memcpy(argv[1], LANGMAP_OPTION, sizeof(LANGMAP_OPTION) - 1);
	copy_langmap_converting_cpp(argv[1] + sizeof(LANGMAP_OPTION) - 1, param->langmap);

	if (pipe(opipe) < 0 || pipe(ipipe) < 0)
		param->die("cannot create pipe.");
	pid = fork();
	if (pid == 0) {
		/* child process */
		close(opipe[1]);
		close(ipipe[0]);
		if (dup2(opipe[0], STDIN_FILENO) < 0
		 || dup2(ipipe[1], STDOUT_FILENO) < 0)
			param->die("dup2 failed.");
		close(opipe[0]);
		close(ipipe[1]);
		execvp(ctagscom, argv);
		param->die("cannot execute '%s'. (execvp failed)", ctagscom);
	}
	/* parent process */
	if (pid < 0)
		param->die("fork failed.");
	free(path);
	free(argv[1]);
	close(opipe[0]);
	close(ipipe[1]);
	ip = fdopen(ipipe[0], "r");
	op = fdopen(opipe[1], "w");
	if (ip == NULL || op == NULL)
		param->die("fdopen failed.");

	bufsize = INITIAL_BUFSIZE;
	linebuf = malloc(bufsize);
	if (linebuf == NULL)
		param->die("short of memory.");
}

static void
terminate_ctags(void)
{
	if (op == NULL)
		return;
	free(linebuf);
	fclose(op);
	fclose(ip);
	while (waitpid(pid, NULL, 0) < 0 && errno == EINTR)
		;
}
#endif

static char *
get_line(const struct parser_param *param)
{
	size_t linelen = 0;

	for (;;) {
		if (fgets(linebuf + linelen, bufsize - linelen, ip) == NULL) {
			if (linelen == 0)
				return NULL;
			break;
		}
		linelen += strlen(linebuf + linelen);
		if (linelen < bufsize - 1 || linebuf[linelen - 1] == '\n'
		 || feof(ip))
			break;
		bufsize *= 2;
		linebuf = realloc(linebuf, bufsize);
		if (linebuf == NULL)
			param->die("short of memory.");
	}
	while (linelen-- > 0
		&& (linebuf[linelen] == '\n' || linebuf[linelen] == '\r'))
		linebuf[linelen] = '\0';
	return linebuf;
}

static void
put_line(char *ctags_x, const struct parser_param *param)
{
	int lineno;
	int type = PARSER_DEF;
	char *p, *tagname, *filename;

#if defined(USE_EXTRA_FIELDS)
	/*
	 * Output of ctags:
	 * ctags -x ...
	 * main              326 global/global.c  main(int argc, char **argv)
	 *
	 * ctags -x --_xformat="%R %-16N %4n %-16F %C" --extras=+r --fields=+r
	 * D main              326 global/global.c  main(int argc, char **argv)
	 */
	switch (*ctags_x) {
	case 'D':
		type = PARSER_DEF;
		break;
	case 'R':
		type = PARSER_REF_SYM;
		break;
	default:
		param->die("unexpected type string.");
	}
	/* skip type string */
	while (*ctags_x && !isspace((unsigned char)*ctags_x))
		ctags_x++;
	while (*ctags_x  && isspace((unsigned char)*ctags_x))
		ctags_x++;
#endif
	filename = strstr(ctags_x, param->file);
	if (filename == NULL || filename == ctags_x)
		return;
	p = filename - 1;
	if (!isspace((unsigned char)*p))
		return;
	while (p >= ctags_x && isspace((unsigned char)*p))
		*p-- = '\0';
	if (p < ctags_x)
		return;
	if (!isdigit((unsigned char)*p))
		return;
	while (p >= ctags_x && isdigit((unsigned char)*p))
		p--;
	if (p < ctags_x)
		return;
	lineno = atoi(p + 1);
	if (!isspace((unsigned char)*p))
		return;
	while (p >= ctags_x && isspace((unsigned char)*p))
		*p-- = '\0';
	if (p < ctags_x)
		return;
	while (p >= ctags_x && !isspace((unsigned char)*p))
		p--;
	tagname = p + 1;
	p = filename + strlen(param->file);
	if (*p != '\0') {
		if (!isspace((unsigned char)*p))
			return;
		*p++ = '\0';
		while (isspace((unsigned char)*p))
			p++;
	}
	param->put(type, tagname, lineno, filename, p, param->arg);
}

void
parser(const struct parser_param *param)
{
	char *ctags_x;

	assert(param->size >= sizeof(*param));

	if (op == NULL)
		start_ctags(param);

	/* Write path of input file to pipe. */
	fputs(param->file, op);
	putc('\n', op);
	fflush(op);

	/* Read output of ctags command. */
	for (;;) {
		ctags_x = get_line(param);
		if (ctags_x == NULL)
			param->die("unexpected EOF.");
		if (strcmp(ctags_x, TERMINATOR) == 0)
			break;
		put_line(ctags_x, param);
	}
}
