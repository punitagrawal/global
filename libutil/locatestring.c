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
 *	locatestring.c				3-Aug-99
 *
 */
#include <string.h>
#include <ctype.h>

#include "locatestring.h"

/*
 * strincmp: strncmp with ignoring case.
 *
 *	Interface is same with strncmp.
 */
static int
strincmp(string, pattern, len)
const char *string;
const char *pattern;
size_t len;
{
	unsigned char s, p;

	while (len--) {
		s = tolower(*string++);
		p = tolower(*pattern++);
		if (s != p)
			return s - p;
		if (s == 0)
			break;
	}
	return 0;
}

/*
 * locatestring: locate pattern from string
 *
 *	i)	string	string
 *	i)	pattern	pattern
 *	i)	flag	MATCH_FIRST:	match first
 *			MATCH_AT_FIRST: match only at first column
 *			MATCH_LAST:	match last
 *			MATCH_AT_LAST:	match only at last column
 *			IGNORE_CASE:	Ignore case
 *	r)		pointer or NULL
 *
 * This function is made to avoid compatibility problems.
 */
char	*
locatestring(string, pattern, flag)
const char *string;
const char *pattern;
int	flag;
{
	int	c = *pattern;
	int	plen = strlen(pattern);
	const char *p = NULL;
	int	slen;
	int	(*cmpfunc) ();

	cmpfunc = (flag & IGNORE_CASE) ? strincmp : strncmp;
	flag &= ~IGNORE_CASE;

	if (flag == MATCH_AT_LAST && (slen = strlen(string)) > plen)
		string += (slen - plen);
	for (; *string; string++) {
		if (*string == c)
			if (!(*cmpfunc)(string, pattern, plen)) {
				p = string;
				if (flag == MATCH_FIRST)
					break;
			}
		if (flag == MATCH_AT_FIRST || flag == MATCH_AT_LAST)
			break;
	}
	return (char *)p;
}
