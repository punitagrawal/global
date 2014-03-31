/*
 * Copyright (c) 1997, 1998, 1999, 2000, 2002, 2005, 2006, 2010
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
#include <ctype.h>
#ifdef STDC_HEADERS
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif

#include "checkalloc.h"
#include "die.h"
#include "strbuf.h"

#ifndef isblank
#define isblank(c)	((c) == ' ' || (c) == '\t')
#endif
/**
 @file

String buffer: usage and memory status

					[xxx]: string buffer <br>
					'v': current pointer

@code
Function call                           Memory status
----------------------------------------------------------
                                        (not exist)
                                         v
sb = strbuf_open(0);                    []
                                          v
strbuf_putc(sb, 'a');                   [a]
                                          v
char *s = strbuf_value(sb);             [a\0]           s == "a"
                                            v
strbuf_puts(sb, "bc");                  [abc]
                                            v
char *s = strbuf_value(sb);             [abc\0]         s == "abc"
                                            v
int len = strbuf_getlen(sb);            [abc\0]         len == 3
                                         v
strbuf_reset(sb);                       [abc\0]
                                         v
int len = strbuf_getlen(sb);            [abc\0]         len == 0
                                           v
strbuf_puts(sb, "XY");                  [XYc\0]
                                           v
char *s = strbuf_value(sb);             [XY\0]          s == "XY"

fp = fopen("/etc/passwd", "r");                                             v
char *s = strbuf_fgets(sb, fp, 0)       [root:*:0:0:Charlie &:/root:/bin/csh\0]
fclose(fp)				s == "root:*:0:0:Charlie &:/root:/bin/csh"

strbuf_close(sb);                       (not exist)
@endcode
*/

/**
 * __strbuf_expandbuf: expand buffer so that afford to the length data at least.
 *
 *	@param[in]	sb	#STRBUF structure
 *	@param[in]	length	required room
 */
void
__strbuf_expandbuf(STRBUF *sb, int length)
{
	int count = sb->curp - sb->sbuf;
	int newsize = sb->sbufsize + (length > EXPANDSIZE ? length : EXPANDSIZE);
	char *newbuf;

	newbuf = (char *)check_realloc(sb->sbuf, newsize + 1);
	sb->sbufsize = newsize;
	sb->sbuf = newbuf;

	sb->curp = sb->sbuf + count;
	sb->endp = sb->sbuf + sb->sbufsize;
}
/**
 * strbuf_open: open string buffer.
 *
 *	@param[in]	init	initial buffer size <br>
 *			if 0 (zero) is specified then use default value (#INITIALSIZE).
 *	@return	sb	#STRBUF structure
 */
STRBUF *
strbuf_open(int init)
{
	STRBUF *sb = (STRBUF *)check_calloc(sizeof(STRBUF), 1);

	sb->sbufsize = (init > 0) ? init : INITIALSIZE;
	sb->sbuf = (char *)check_malloc(sb->sbufsize + 1);
	sb->curp = sb->sbuf;
	sb->endp = sb->sbuf + sb->sbufsize;

	return sb;
}
/**
 * strbuf_reset: reset string buffer.
 *
 *	@param[in]	sb	string buffer
 */
void
strbuf_reset(STRBUF *sb)
{
	sb->curp = sb->sbuf;
}
/**
 * strbuf_clear: clear static string buffer.
 *
 *	@param[in]	sb	statically defined string buffer
 *
 * This function is used for the initializing of static string buffer.
 * For the detail, see STATIC_STRBUF(sb) macro in @FILE{strbuf.h}.
 */
void
strbuf_clear(STRBUF *sb)
{
	if (sb == NULL)
		die("NULL string buffer. (strbuf_clear)");
	if (strbuf_empty(sb)) {
		sb->sbufsize = INITIALSIZE;
		sb->sbuf = (char *)check_malloc(sb->sbufsize + 1);
		sb->curp = sb->sbuf;
		sb->endp = sb->sbuf + sb->sbufsize;
	} else {
		strbuf_reset(sb);
	}
}
/**
 * strbuf_nputs: Put string with length
 *
 *	@param[in]	sb	string buffer
 *	@param[in]	s	string
 *	@param[in]	len	length of string
 */
void
strbuf_nputs(STRBUF *sb, const char *s, int len)
{
	if (len > 0) {
		if (sb->curp + len > sb->endp)
			__strbuf_expandbuf(sb, len);
		while (len-- > 0)
			*sb->curp++ = *s++;
	}
}
/**
 * strbuf_nputc: Put a character, @a len (number) times
 *
 *	@param[in]	sb	string buffer
 *	@param[in]	c	character
 *	@param[in]	len	number of times to put @a c
 *
 *	@see strbuf_putc()
 */
void
strbuf_nputc(STRBUF *sb, int c, int len)
{
	if (len > 0) {
		if (sb->curp + len > sb->endp)
			__strbuf_expandbuf(sb, len);
		while (len-- > 0)
			*sb->curp++ = c;
	}
}
/**
 * strbuf_puts: Put string
 *
 *	@param[in]	sb	string buffer
 *	@param[in]	s	string
 */
void
strbuf_puts(STRBUF *sb, const char *s)
{
	while (*s) {
		if (sb->curp >= sb->endp)
			__strbuf_expandbuf(sb, 0);
		*sb->curp++ = *s++;
	}
}
/**
 * strbuf_puts_withterm: Put string until the terminator
 *
 *	@param[in]	sb	string buffer
 *	@param[in]	s	string
 *	@param[in]	c	terminator
 *	@return		pointer to the terminator
 */
void
strbuf_puts_withterm(STRBUF *sb, const char *s, int c)
{
	while (*s && *s != c) {
		if (sb->curp >= sb->endp)
			__strbuf_expandbuf(sb, 0);
		*sb->curp++ = *s++;
	}
}
/**
 * strbuf_puts_nl: Put string with a new line
 *
 *	@param[in]	sb	string buffer
 *	@param[in]	s	string
 */
void
strbuf_puts_nl(STRBUF *sb, const char *s)
{
	while (*s) {
		if (sb->curp >= sb->endp)
			__strbuf_expandbuf(sb, 0);
		*sb->curp++ = *s++;
	}
	if (sb->curp >= sb->endp)
		__strbuf_expandbuf(sb, 0);
	*sb->curp++ = '\n';
}
/**
 * strbuf_putn: put digit string at the last of buffer.
 *
 *	@param[in]	sb	#STRBUF structure
 *	@param[in]	n	number
 */
void
strbuf_putn(STRBUF *sb, int n)
{
	if (n == 0) {
		strbuf_putc(sb, '0');
	} else {
		char num[128];
		int i = 0;

		while (n) {
			if (i >= sizeof(num))
				die("Too big integer value.");
			num[i++] = n % 10 + '0';
			n = n / 10;
		}
		while (--i >= 0)
			strbuf_putc(sb, num[i]);
	}
}
/**
 * strbuf_unputc: remove specified char from the last of buffer
 *
 *	@param[in]	sb	#STRBUF structure
 *	@param[in]	c	character
 *	@return		0: do nothing, 1: removed
 */
int
strbuf_unputc(STRBUF *sb, int c)
{
	if (sb->curp > sb->sbuf && *(sb->curp - 1) == c) {
		sb->curp--;
		return 1;
	}
	return 0;
}
/**
 * strbuf_value: return the content of string buffer.
 *
 *	@param[in]	sb	#STRBUF structure
 *	@return		string
 */
char *
strbuf_value(STRBUF *sb)
{
	*sb->curp = 0;
	return sb->sbuf;
}
/**
 * strbuf_trim: trim following blanks.
 *
 *	@param[in]	sb	#STRBUF structure
 */
void
strbuf_trim(STRBUF *sb)
{
	char *p = sb->curp;

	while (p > sb->sbuf && isblank(*(p - 1)))
		*--p = 0;
	sb->curp = p;
}
/**
 * strbuf_fgets: read whole record into string buffer
 *
 *	@param[out]	sb	string buffer
 *	@param[in]	ip	input stream
 *	@param[in]	flags	flags <br>
 *			#STRBUF_NOCRLF:	remove last @CODE{'\\n'} and/or @CODE{'\\r'} if exist. <br>
 *			#STRBUF_APPEND:	append next record to existing data <br>
 *			#STRBUF_SHARPSKIP: skip lines which start with @CODE{'\#'}
 *	@return		record buffer (@VAR{NULL} at end of file)
 *
 * Returned buffer has whole record. <br>
 * The buffer end with @CODE{'\\0'}. If #STRBUF_NOCRLF is set then buffer doesn't
 * include @CODE{'\\r'} and @CODE{'\\n'}.
 */
char *
strbuf_fgets(STRBUF *sb, FILE *ip, int flags)
{
	if (!(flags & STRBUF_APPEND))
		strbuf_reset(sb);

	if (sb->curp >= sb->endp)
		__strbuf_expandbuf(sb, EXPANDSIZE);	/* expand buffer */

	for (;;) {
		if (!fgets(sb->curp, sb->endp - sb->curp, ip)) {
			if (sb->curp == sb->sbuf)
				return NULL;
			break;
		}
		if (flags & STRBUF_SHARPSKIP && *(sb->curp) == '#')
			continue;
		sb->curp += strlen(sb->curp);
		if (sb->curp > sb->sbuf && *(sb->curp - 1) == '\n')
			break;
		else if (feof(ip)) {
			return sb->sbuf;
		}
		__strbuf_expandbuf(sb, EXPANDSIZE);	/* expand buffer */
	}
	if (flags & STRBUF_NOCRLF) {
		if (*(sb->curp - 1) == '\n')
			*(--sb->curp) = 0;
		if (sb->curp > sb->sbuf && *(sb->curp - 1) == '\r')
			*(--sb->curp) = 0;
	}
	return sb->sbuf;
}
/**
 * strbuf_sprintf: do @NAME{sprintf} into string buffer.
 *
 *	@param[in]	sb	#STRBUF structure
 *	@param[in]	s	similar to @NAME{sprintf()} <br>
 *			Currently the following format is supported. <br>
 *			@CODE{\%s, \%d, \%\<number\>d, \%\<number\>s, \%-\<number\>d, \%-\<number\>s}
 */
void
strbuf_sprintf(STRBUF *sb, const char *s, ...)
{
	va_list ap;

	va_start(ap, s);
	strbuf_vsprintf(sb, s, ap);
	va_end(ap);
}
/**
 * strbuf_vsprintf: do @NAME{vsprintf} into string buffer.
 *
 *	@param[in]	sb	#STRBUF structure
 *	@param[in]	s	similar to @NAME{vsprintf()} <br>
 *			Currently the following format is supported. <br>
 *			@CODE{\%s, \%d, \%\<number\>d, \%\<number\>s, \%-\<number\>d, \%-\<number\>s}
 *	@param[in]	ap	 <br>
 */
void
strbuf_vsprintf(STRBUF *sb, const char *s, va_list ap)
{
	for (; *s; s++) {
		/*
		 * Put the before part of '%'.
		 */
		{
			const char *p;
			for (p = s; *p && *p != '%'; p++)
				;
			if (p > s) {
				strbuf_nputs(sb, s, p - s);
				s = p;
			}
		}
		if (*s == '\0')
			break;
		if (*s == '%') {
			int c = (unsigned char)*++s;
			/*
			 * '%%' means '%'.
			 */
			if (c == '%') {
				strbuf_putc(sb, c);
			}
			/*
			 * If the optional number is specified then
			 * we forward the job to snprintf(3).
			 * o %<number>d
			 * o %<number>s
			 * o %-<number>d
			 * o %-<number>s
			 */
			else if (isdigit(c) || (c == '-' && isdigit((unsigned char)*(s + 1)))) {
				char format[32], buf[1024];
				int i = 0;

				format[i++] = '%';
				if (c == '-')
					format[i++] = *s++;
				while (isdigit((unsigned char)*s))
					format[i++] = *s++;
				format[i++] = c = *s;
				format[i] = '\0';
				if (c == 'd' || c == 'x')
					snprintf(buf, sizeof(buf), format, va_arg(ap, int));
				else if (c == 's')
					snprintf(buf, sizeof(buf), format, va_arg(ap, char *));
				else
					die("Unsupported control character '%c'.", c);
				strbuf_puts(sb, buf);
			} else if (c == 's') {
				strbuf_puts(sb, va_arg(ap, char *));
			} else if (c == 'd') {
				strbuf_putn(sb, va_arg(ap, int));
			} else {
				die("Unsupported control character '%c'.", c);
			}
		}
	}
}
/**
 * strbuf_close: close string buffer.
 *
 *	@param[in]	sb	#STRBUF structure
 */
void
strbuf_close(STRBUF *sb)
{
	if (sb->name)
		(void)free(sb->name);
	(void)free(sb->sbuf);
	(void)free(sb);
}
/**
 * @fn STRBUF *strbuf_open_tempbuf(void)
 *
 * Temporary string buffer for general purpose.
 *
 * @par Usage:
 *
 * @code
 *	STRBUF *sbt = strbuf_open_tempbuf();
 *	....
 *	strbuf_puts(sbtemp, "xxx");
 *	...
 *	strbuf_release_tempbuf(sbt);
 * @endcode
 *
 */
static int used = 0;

STRBUF *
strbuf_open_tempbuf(void)
{
	STATIC_STRBUF(sb);
	if (used)
		die("Internal error: temporary string buffer is already used.");
	used = 1;
	strbuf_clear(sb);
	return sb;
}
void
strbuf_release_tempbuf(STRBUF *sb)
{
	used = 0;
}
