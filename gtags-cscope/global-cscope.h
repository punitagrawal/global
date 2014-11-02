/*===========================================================================
 Copyright (c) 1998-2000, The Santa Cruz Operation 
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 *Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.

 *Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 *Neither name of The Santa Cruz Operation nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission. 

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS
 IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 DAMAGE. 
 =========================================================================*/

/* $Id: global-cscope.h,v 1.5 2014/08/18 09:07:45 shigio Exp $ */

/** @file
 *	global type, data, and function definitions
 *
 *	cscope - interactive C symbol cross-reference
 */

#ifndef CSCOPE_GLOBAL_H
#define CSCOPE_GLOBAL_H

#include "config.h"
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>	/* isalpha, isdigit, etc. */
#include <signal.h>	/* SIGINT and SIGQUIT */
#include <stdio.h>	/* standard I/O package */
#include <stdlib.h>     /* standard library functions */

/* A special "magic" header file required by HP/Compaq NSK (Non-Stop
 * Kernel) to present a more Unix-ish environment ... */
#ifdef HAVE_FLOSS_H
# include <floss.h>
#endif

/* Replace most of the #if BSD stuff. Taken straight from the autoconf
 * manual, with an extension for handling memset(). */
#if STDC_HEADERS
# include <string.h>	/* string functions */
#else
# ifndef HAVE_STRCHR
#  define strchr index
#  define strrchr rindex
# endif
char *strchr (), *strrchr ();
# ifndef HAVE_MEMCPY
#  define memcpy(d, s, n) bcopy ((s), (d), (n))
#  define memmove(d, s, n) bcopy ((s), (d), (n))
# endif
# ifndef HAVE_MEMSET
#  ifndef HAVE_MEMORY_H
char	*memset();
#  else 
#   include <memory.h>	/* memset */
#  endif /*V9*/
# endif /* HAVE_MEMSET */
#endif /* STDC_HEADERS */

#include "constants.h"	/* misc. constants */
#include "library.h"    /* library function return values */

/* Fallback, in case 'configure' failed to do its part of the job */
#ifndef RETSIGTYPE
#if SVR2 || BSD && !sun
#define RETSIGTYPE int
#else
#define RETSIGTYPE void
#endif
#endif /* RETSIGTYPE */

#ifndef HAVE_SIGHANDLER_T
typedef RETSIGTYPE (*sighandler_t)(int);
#endif

/* FIXME: this testing for platforms is foolish. Stop it! */
#if BSD
# undef	tolower		/* BSD toupper and tolower don't test the character */
# undef	toupper
# define	tolower(c)	(isupper(c) ? (c) - 'A' + 'a' : (c))	
# define	toupper(c)	(islower(c) ? (c) - 'a' + 'A' : (c))
# if !sun 
#  if !__FreeBSD__
/* in traditional BSD, *printf() doesn't return the number of bytes
 * written */
#   define PRINTF_RETVAL_BROKEN 1
#  endif /* !FreeBSD */
# endif /* !sun */
#endif

/* Un-comment this if you're on a filesystem that doesn't support
 * filenames longer than 14 characters */
/* HBB FIXME 20030302: should have an autoconf test for this: */
/* #define SHORT_NAMES_ONLY */

/* Just in case autoconf didn't correctly flag HAVE_FIXKEYPAD */
#ifndef HAVE_FIXKEYPAD 
# if SVR2 && !BSD && !V9 && !u3b2 && !sun
#  define HAVE_FIXKEYPAD
# endif
#endif

/* HBB 20020728: if <fcntl.h> is there, #include it here, since that's
 * where the system definitions of O_TEXT should be coming from */
#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

/* HBB 20020103: Need to force text or binary mode opens on Cygwins,
 * because of their "binary/text mode mount" silliness :-( */
#ifndef O_TEXT
# ifdef _O_TEXT
#  define O_TEXT _O_TEXT
# else
#  define O_TEXT 0x00
# endif
#endif
/* Same for binary mode --- moved here from vp.h */
#ifndef O_BINARY
# ifdef _O_BINARY
#  define O_BINARY _O_BINARY
# else
#  define O_BINARY 0x00
# endif
#endif 

#undef SETMODE
#if O_BINARY || O_TEXT
/* OK, looks like we are on an MSDOS-ish platform ---> define SETMODE
 * to actually do something */
# ifdef HAVE_SETMODE
#  define SETMODE(fildes, mode) setmode(fildes,mode)
# else 
#  ifdef HAVE__SETMODE
#   define SETMODE(fildes, mode) _setmode(fildes,mode)
#  endif
# endif
#endif

/* access(2) parameters. Only make assumptions about their values if
 * <unistd.h> fails to define them. */
#ifdef R_OK
# define	READ	R_OK
#else
# define	READ	4	
#endif
#ifdef W_OK
# define	WRITE	W_OK
#else
# define	WRITE	2
#endif

/* This can happen on only vaguely Unix-ish platforms... */
#ifndef HAVE_LSTAT
# define	lstat(file,buf)	stat(file,buf)
#endif

typedef	enum	{		/** boolean data type */
	NO,
	YES
} BOOL;

typedef	enum	{		/** findinit return code */
	NOERROR,
	NOTSYMBOL,
	REGCMPERROR
} FINDINIT;

typedef	struct {		/** mouse action */
	int	button;
	int	percent;
	int	x1;
	int	y1;
	int	x2;
	int	y2;
} MOUSE;

struct cmd {			/** command history struct */
	struct	cmd *prev, *next;	/**< list ptrs */
	int	field;			/**< input field number */
	char	*text;			/**< input field text */
};

/** @name main.c global data */
/** @{ */
extern	char	*editor, *home, *shell, *lineflag;	/**< environment variables */
extern	char	*global_command;			/**< @FILE{global} by default */
extern	char	*gtags_command;				/**< @FILE{gtags} by default */
extern	char	*home;		/**< Home directory */
extern 	BOOL	lineflagafterfile;
extern	char	*argv0;		/**< command name */
extern	int	dispcomponents;	/**< file path components to display */
#if CCS
extern	BOOL	displayversion;	/**< display the C Compilation System version */
#endif
extern	BOOL	editallprompt;	/**< prompt between editing files */
extern	BOOL	incurses;	/**< in @NAME{curses} */
extern	BOOL	isuptodate;	/**< consider the crossref up-to-date */
extern	BOOL	linemode;	/**< use line oriented user interface */
extern	BOOL	absolutepath;	/**< print absolute path name */
extern	BOOL	verbosemode;	/**< print extra information on line mode */
extern	BOOL	ogs;		/**< display OGS book and subsystem names */
extern	char	*prependpath;	/**< prepend path to file names */
extern	FILE	*refsfound;	/**< references found file */
extern	char	temp1[];	/**< temporary file name */
extern	char	temp2[];	/**< temporary file name */
extern	char	tempstring[TEMPSTRING_LEN + 1]; /**< global dummy string buffer */
extern	char	*tmpdir;	/**< temporary directory */
/** @} */

/** @name command.c global data */
/** @{ */
extern	BOOL	caseless;	/**< ignore letter case when searching */
extern	BOOL	*change;	/**< change this line */
extern	BOOL	changing;	/**< changing text */
extern	int	selecting;
extern	unsigned int curdispline;
extern	char	newpat[];	/**< new pattern */
extern	char	Pattern[];	/**< symbol or text pattern */
/** @} */

/** @name display.c global data */
/** @{ */
extern 	int	booklen;	/**< OGS book name display field length */
extern	int	*displine;	/**< screen line of displayed reference */
extern	unsigned int disprefs;	/**< displayed references */
extern	int	fcnlen;		/**< function name display field length */
extern	int	field;		/**< input field */
extern	int	filelen;	/**< file name display field length */
extern	unsigned fldcolumn;	/**< input field column */
extern	unsigned int mdisprefs;	/**< maximum displayed references */
extern	unsigned int nextline;	/**< next line to be shown */
extern	int	numlen;		/**< line number display field length */
extern	unsigned int topline;	/**< top line of page */
extern	int	bottomline;	/**< bottom line of page */
extern	long	searchcount;	/**< count of files searched */
extern	int	subsystemlen;	/**< OGS subsystem name display field length */
extern	unsigned int totallines; /**< total reference lines */
extern	const char dispchars[];	/**< display chars for jumping to lines */
/** @} */

/** @name mouse.c global data */
/** @{ */
extern	BOOL	mouse;		/**< mouse interface */
/** @} */

#if UNIXPC
extern	BOOL	unixpcmouse;		/**< UNIX PC mouse interface */
#endif

/* cscope functions called from more than one function or between files */ 

char	*filepath(char *file);
char	*findcalledby(char *pattern);
char	*findcalling(char *pattern);
char	*findallfcns(char *dummy);
char	*finddef(char *pattern);
char	*findfile(char *dummy);
char	*findinclude(char *pattern);
char	*findsymbol(char *pattern);
char	*findregexp(char *egreppat);
char	*findstring(char *pattern);
char	*pathcomponents(char *path, int components);

void	addcmd(int f, char *s);
void	addsrcfile(char *path);
void	askforchar(void);
void	askforreturn(void);
void	atchange(void);
void	atfield(void);
void	cannotwrite(char *file);
void	cannotopen(char *file);
void	clearmsg(void);
void	clearmsg2(void);
void	countrefs(void);
void	crossref(char *srcfile);
void    dispinit(void);
void	display(void);
void	drawscrollbar(int top, int bot);
void	edit(char *file, char *linenum);
void	editall(void);
void	editref(int);
void	entercurses(void);
void	exitcurses(void);
void	findcleanup(void);
void    freesrclist(void);
void    freeinclist(void);
void    freecrossref(void);
void	freefilelist(void);
void	help(void);
void	incfile(char *file, char *type);
void    includedir(char *_dirname);
void    initsymtab(void);
void	makefilelist(void);
void	mousecleanup(void);
void	mousemenu(void);
void	mouseinit(void);
void	mousereinit(void);
void	myexit(int sig);
void	myperror(char *text);
void	ogsnames(char *file, char **subsystem, char **book);
void	progress(char *what, long current, long max);
void	putfilename(char *srcfile);
void	postmsg(char *msg);
void	postmsg2(char *msg);
void	posterr(char *msg,...);
void	postfatal(const char *msg,...);
void	putposting(char *term, int type);
void	fetch_string_from_dbase(char *, size_t);
void	resetcmd(void);
void	seekline(unsigned int line);
void	setfield(void);
void	shellpath(char *out, int limit, char *in);
void    sourcedir(char *dirlist);
void	myungetch(int c);
/* void	warning(char *text); */
void	writestring(char *s);

BOOL	command(int commandc);
BOOL	readrefs(char *filename);
BOOL	search(void);
BOOL	writerefsfound(void);

MOUSE	*getmouseaction(char leading_char);
struct	cmd *currentcmd(void);
struct	cmd *prevcmd(void);
struct	cmd *nextcmd(void);

int	egrep(char *file, FILE *output, char *format);
int	mygetline(char p[], char s[], unsigned size, int firstchar, BOOL iscaseless);
int	mygetch(void);
int	execute(char *a, ...);

#endif /* CSCOPE_GLOBAL_H */
