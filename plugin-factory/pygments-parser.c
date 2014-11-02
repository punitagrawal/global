/*
 * Copyright (c) 2010
 *	Tama Communications Corporation
 * Copyright (c) 2014
 *	Yoshitaro Makise
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
#if !defined(_WIN32) || defined(__CYGWIN__)
#include <sys/wait.h>
#endif
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

#define PYGMENTS_PARSER (DATADIR "/gtags/script/pygments_parser.py")

/*
 * Function layer plugin parser sample
 */

#define TERMINATOR		"###terminator###"
#define LANGMAP_OPTION		"--langmap="
#define INITIAL_BUFSIZE		1024

static char *argv[] = {
	PYGMENTS_PARSER,
	NULL,
	NULL
};
static pid_t pid;
static FILE *ip, *op;
static char *linebuf;
static size_t bufsize;

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

static void
start_process(const struct parser_param *param)
{
	int opipe[2], ipipe[2];

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
		execvp(PYGMENTS_PARSER, argv);
		param->die("execvp failed.");
	}
	/* parent process */
	if (pid < 0)
		param->die("fork failed.");
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

#ifdef __GNUC__
static void terminate_process(void) __attribute__((destructor));
#endif

static void
terminate_process(void)
{
	if (op == NULL)
		return;
	free(linebuf);
	fclose(op);
	fclose(ip);
	while (waitpid(pid, NULL, 0) < 0 && errno == EINTR)
		;
}

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
put_line(char *line, const struct parser_param *param)
{
	int lineno;
	int type = PARSER_DEF;
	char *p, *tagname, *filename;

	/*
	 * Output of ctags:
	 * ctags -x ...
	 * main              326 global/global.c  main(int argc, char **argv)
	 *
	 * ctags -x --gtags ...
	 * D main              326 global/global.c  main(int argc, char **argv)
	 */
	switch (*line) {
	case 'D':
		type = PARSER_DEF;
		break;
	case 'R':
		type = PARSER_REF_SYM;
		break;
	default:
		param->die("unexpected type string: %s", line);
	}
	/* skip type string */
	while (*line && !isspace((unsigned char)*line))
		line++;
	while (*line  && isspace((unsigned char)*line))
		line++;
	filename = strstr(line, param->file);
	if (filename == NULL || filename == line)
		return;
	p = filename - 1;
	if (!isspace((unsigned char)*p))
		return;
	while (p >= line && isspace((unsigned char)*p))
		*p-- = '\0';
	if (p < line)
		return;
	if (!isdigit((unsigned char)*p))
		return;
	while (p >= line && isdigit((unsigned char)*p))
		p--;
	if (p < line)
		return;
	lineno = atoi(p + 1);
	if (!isspace((unsigned char)*p))
		return;
	while (p >= line && isspace((unsigned char)*p))
		*p-- = '\0';
	if (p < line)
		return;
	while (p >= line && !isspace((unsigned char)*p))
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
	char *line;

	assert(param->size >= sizeof(*param));

	if (op == NULL)
		start_process(param);

	/* Write path of input file to pipe. */
	fputs(param->file, op);
	putc('\n', op);
	fflush(op);

	/* Read output of the process. */
	for (;;) {
		line = get_line(param);
		if (line == NULL)
			param->die("unexpected EOF.");
		if (strcmp(line, TERMINATOR) == 0)
			break;
		put_line(line, param);
	}
}
