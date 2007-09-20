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
 *	usable.c				26-Aug-99
 *
 */
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "gparam.h"
#include "locatestring.h"
#include "makepath.h"
#include "path.h"
#include "test.h"
#include "usable.h"

#ifdef _WIN32
static const char *suffix[] = {".exe", ".com", ".bat",};
#endif

/*
 * usable: check if command is executable or not.
 *
 *	i)	command
 *	r)		1: executable
 *			0: non executable
 */
char *
usable(command)
char	*command;
{
	char	buf[MAXENVLEN+1], *p, *dir;
	static char path[MAXPATHLEN+1];
#ifdef _WIN32
	int	i, lim = sizeof(suffix)/sizeof(char *);
#endif

	if (isabspath(command) || locatestring(command, "./", MATCH_AT_FIRST)
		|| locatestring(command, "../", MATCH_AT_FIRST)) {
		if (test("fx", command)) {
			strcpy(path, command);
			return path;
		}
		return NULL;
	}
	strcpy(buf, getenv("PATH"));
	p = buf;
	while (p) {
		dir = p;
		if ((p = locatestring(p, PATHSEP, MATCH_FIRST)) != NULL)
			*p++ = 0;
		if (test("fx", makepath(dir, command, NULL))) {
			strcpy(path, makepath(dir, command, NULL));
			return path;
		}
#if defined(_WIN32)
		for (i = 0; i < lim; i++)
			if (test("f", makepath(dir, command, suffix[i]))) {
				strcpy(path, makepath(dir, command, suffix[i]));
				return path;
			}
#endif
	}
	return NULL;
}
