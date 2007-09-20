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
 *	getdbpath.c				19-Aug-99
 *
 */
#include <sys/types.h>
#include <sys/stat.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "gparam.h"
#include "die.h"
#include "getdbpath.h"
#include "makepath.h"
#include "path.h"
#include "test.h"

static const char *makeobjdirprefix;	/* obj partition		*/
static const char *makeobjdir;		/* obj directory		*/

/*
 * gtagsexist: test whether GTAGS's existence.
 *
 *	i)	candidate candidate root directory
 *	o)	dbpath	directory which GTAGS exist
 *	r)		0: not found, 1: found
 */
int
gtagsexist(candidate, dbpath)
char	*candidate;
char	*dbpath;
{
	char	path[MAXPATHLEN+1];

	sprintf(path, "%s/GTAGS", candidate);
	if (test("fr", path)) {
		strcpy(dbpath, candidate);
		return 1;
	}
	sprintf(path, "%s/%s/GTAGS", candidate, makeobjdir);
	if (test("fr", path)) {
		sprintf(dbpath, "%s/%s", candidate, makeobjdir);
		return 1;
	}
	sprintf(path, "%s%s/GTAGS", makeobjdirprefix, candidate);
	if (test("fr", path)) {
		sprintf(dbpath, "%s%s", makeobjdirprefix, candidate);
		return 1;
	}
	return 0;
}
/*
 * getdbpath: get dbpath directory
 *
 *	o)	cwd	current directory
 *	o)	root	root of source tree
 *	o)	dbpath	directory which GTAGS exist
 *
 * root and dbpath assumed as
 *	char	cwd[MAXPATHLEN+1];
 *	char	root[MAXPATHLEN+1];
 *	char	dbpath[MAXPATHLEN+1];
 */
void
getdbpath(cwd, root, dbpath)
char	*cwd;
char	*root;
char	*dbpath;
{
	struct stat sb;
	char	*p;

	if (!getcwd(cwd, MAXPATHLEN))
		die("cannot get current directory.");
	canonpath(cwd);
#ifndef _WIN32
	/*
	 * GLOBAL never think '/' is the root of source tree.
	 */
	if (!strcmp(cwd, "/"))
		die("It's root directory! What are you doing?");
#endif
	if ((p = getenv("MAKEOBJDIRPREFIX")) != NULL)
		makeobjdirprefix = p;
	else
		makeobjdirprefix = "/usr/obj";
	if ((p = getenv("MAKEOBJDIR")) != NULL)
		makeobjdir = p;
	else
		makeobjdir = "obj";

	if ((p = getenv("GTAGSROOT")) != NULL) {
		if (!isabspath(p))
			die("GTAGSROOT must be an absolute path.");
		if (stat(p, &sb) || !S_ISDIR(sb.st_mode))
			die1("directory '%s' not found.", p);
		if (realpath(p, root) == NULL)
			die1("cannot get real path of '%s'.", p);
		/*
		 * GTAGSDBPATH is meaningful only when GTAGSROOT exist.
		 */
		if ((p = getenv("GTAGSDBPATH")) != NULL) {
			if (!isabspath(p))
				die("GTAGSDBPATH must be an absolute path.");
			if (stat(p, &sb) || !S_ISDIR(sb.st_mode))
				die1("directory '%s' not found.", p);
			strcpy(dbpath, getenv("GTAGSDBPATH"));
		} else {
			if (!gtagsexist(root, dbpath))
				die("GTAGS not found.");
		}
	} else {
		/*
		 * start from current directory to '/' directory.
		 */
		strcpy(root, cwd);
		p = root + strlen(root);
		while (!gtagsexist(root, dbpath)) {
			while (*--p != '/' && p > root)
				;
			*p = 0;
			if (root == p)	/* reached root directory */
				break;
		}
		if (*root == 0)
			die("GTAGS not found.");
	}
}
