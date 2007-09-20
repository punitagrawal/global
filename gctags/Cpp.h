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
 *	Cpp.h						26-Jun-99
 */
#define CPP___P		1001
#define CPP_ASM		1002
#define CPP_AUTO	1003
#define CPP_BREAK	1004
#define CPP_CASE	1005
#define CPP_CATCH	1006
#define CPP_CHAR	1007
#define CPP_CLASS	1008
#define CPP_CONST	1009
#define CPP_CONTINUE	1010
#define CPP_DEFAULT	1011
#define CPP_DELETE	1012
#define CPP_DO		1013
#define CPP_DOUBLE	1014
#define CPP_ELSE	1015
#define CPP_ENUM	1016
#define CPP_EXTERN	1017
#define CPP_FLOAT	1018
#define CPP_FOR		1019
#define CPP_FRIEND	1020
#define CPP_GOTO	1021
#define CPP_IF		1022
#define CPP_INLINE	1023
#define CPP_INT		1024
#define CPP_LONG	1025
#define CPP_NEW		1026
#define CPP_OPERATOR	1027
#define CPP_PRIVATE	1028
#define CPP_PROTECTED	1029
#define CPP_PUBLIC	1030
#define CPP_REGISTER	1031
#define CPP_RETURN	1032
#define CPP_SHORT	1033
#define CPP_SIGNED	1034
#define CPP_SIZEOF	1035
#define CPP_STATIC	1036
#define CPP_STRUCT	1037
#define CPP_SWITCH	1038
#define CPP_TEMPLATE	1039
#define CPP_THIS	1040
#define CPP_THROW	1041
#define CPP_TRY		1042
#define CPP_TYPEDEF	1043
#define CPP_UNION	1044
#define CPP_UNSIGNED	1045
#define CPP_VIRTUAL	1046
#define CPP_VOID	1047
#define CPP_VOLATILE	1048
#define CPP_WHILE	1049
#define CPP_SEP		1050

#define CP_ELIF		2001
#define CP_ELSE		2002
#define CP_DEFINE	2003
#define CP_IF		2004
#define CP_IFDEF	2005
#define CP_IFNDEF	2006
#define CP_INCLUDE	2007
#define CP_PRAGMA	2008
#define CP_SHARP	2009
#define CP_ERROR	2010
#define CP_UNDEF	2011
#define CP_ENDIF	2012
#define CP_LINE		2013
#define CP_WARNING	2014
#define CP_IDENT	2015

#define IS_CPPTOKEN(c)	(c > 1000 && c < 2001)
#define IS_CPTOKEN(c)	(c > 2000 && c < 3001)
#define MAXCOMPLETENAME 1024            /* max size of complete name of class */
#define MAXCLASSSTACK   10              /* max size of class stack */
