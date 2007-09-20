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
 *	path.c					24-Aug-99
 *
 */
#include <string.h>

#include "path.h"

/*
 * isabspath: wether absolute path or not
 *
 *	i)	path	path
 *	r)		1: absolute, 0: not absolute
 */
int
isabspath(p)
char *p;
{
	if (p[0] == '/')
		return 1;
#ifdef _WIN32
	if (p[0] == '\\')
		return 1;
	if (isdrivechar(p[0]) && p[1] == ':' && (p[2] == '\\' || p[2] == '/'))
		return 1;
#endif
	return 0;
}

/*
 * canonpath: make canonical path name.
 *
 *	io)	path	path
 *	r)		path
 *
 * Note: canonpath rewrite argument buffer.
 */
char   *
canonpath(path)
char *path;
{
	char *p, *s;
#ifdef _WIN32
	p = path;
	/*
	 * Change \ to / in a path (for DOS/Windows paths)
	 */
	while ((p = strchr(p, '\\')) != NULL)
		*p = '/';
#endif
#ifdef __CYGWIN__
	/*
	 * On NT with CYGWIN, getcwd can return something like
	 * "//c/tmp", which isn't usable. We change that to "c:/tmp".
	 */
	p = path;
	if (p[0] == '/' && p[1] == '/' && isdrivechar(p[2]) && p[3] == '/') {
		s = &p[2];		/* point drive char */
		*p++ = *s++;
		*p++ = ':';
		while (*p++ = *s++)
			;
	}
#endif
	return path;
}
