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
 *	token.h					12-Jul-99
 */

#ifndef _TOKEN_H_
#define _TOKEN_H_

#include "gparam.h"
#include "mgets.h"

#define SYMBOL		0

extern unsigned char     *sp, *cp, *lp;
extern int      lineno;
extern int	crflag;
extern int	cmode;
extern int	ymode;
extern int	cppmode;
extern unsigned char	token[MAXTOKEN];
extern unsigned char	curfile[MAXPATHLEN];

#define nextchar() \
	(cp == NULL ? \
		((sp = cp = mgets(ip, NULL, 0)) == NULL ? \
			EOF : \
			(lineno++, *cp == 0 ? \
				lp = cp, cp = NULL, '\n' : \
				*cp++)) : \
		(*cp == 0 ? (lp = cp, cp = NULL, '\n') : *cp++))
#define atfirst (sp && sp == (cp ? cp - 1 : lp))

int	opentoken(char *);
void	rewindtoken(void);
void	closetoken(void);
int	nexttoken(const char *, int (*)(char *));
void	pushbacktoken(void);
int	peekc(int);
int     atfirst_exceptspace(void);

#endif /* ! _TOKEN_H_ */
