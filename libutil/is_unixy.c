/*
 * Copyright (c) 2001
 *             Tama Communications Corporation. All rights reserved.
 *
 * Contributed by Jason Hood <jadoxa@yahoo.com.au>, 2001.
 *
 * This file is part of GNU GLOBAL.
 *
 * GNU GLOBAL is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * GNU GLOBAL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#ifdef __DJGPP__
#include <stdlib.h>
#include <sys/system.h>
#endif

#include "is_unixy.h"

/*
 * is_unixy: whether running in a unix-like shell or not
 *
 *	r)		1: unixy shell, 0: DOS shell (COMMAND.COM)
 */
int
is_unixy(void)
{
#ifdef __DJGPP__
	static int unix_shell = -1;

	if (unix_shell == -1) {
		char *s = getenv("SHELL");
		/* Assume if SHELL isn't defined, COMSPEC is DOS. */
		unix_shell = (s == NULL) ? 0 : _is_unixy_shell(s);
	}
	return unix_shell;
#else
	return 1;
#endif
}