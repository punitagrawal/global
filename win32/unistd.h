/*
 * This file is placed into the public domain by the author,
 * Dan Fandrich <dan@fch.wimsey.bc.ca>
 */

/* Win32 header file to simulate some of the things Unix provides */

#ifndef _UNISTD_H_
#define _UNISTD_H_

#include <io.h>
#include <fcntl.h>
#include <dir.h>
#include <stddef.h>
#include <stdio.h>  /* to avoid problems with db.h */

#ifdef __VISUALC__
/* Compatibility functions for Microsoft Visual C++ */
#define mkstemp(p) open(_mktemp(p), _O_CREAT | _O_SHORT_LIVED | _O_EXCL)

#ifndef S_ISREG
#define S_ISREG(m) ((m) & _S_IFREG)
#define S_ISDIR(m) ((m) & _S_IFDIR)
#define S_ISCHR(m)  ((m) & S_IFCHR)
#define S_ISBLK(m)  ((m) & S_IFBLK)
#define S_ISFIFO(m) ((m) & S_IFIFO)
#endif

#endif

#ifdef __BORLANDC__
#define mkstemp(p) open(mktemp(p), O_CREAT | O_EXCL)
#define realpath(a,b) _fullpath((b), (a), \
        sizeof(b) == sizeof(char *) ? MAXPATHLEN : sizeof(b)), \
        canonpath(b)
#endif

/* Compatibility for both Borland C++ and MS VC++ */
#define popen _popen
#define pclose _pclose
#define sigsetmask(x) (0)
#define sigblock(x) (0)
#define fsync(f) (0)

#ifndef R_OK
#define R_OK    4
#endif
#ifndef W_OK
#define W_OK    2
#endif
#ifndef X_OK
#define X_OK    1
#endif

#endif /* _UNISTD_H_ */
