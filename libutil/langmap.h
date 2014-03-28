/*
 * Copyright (c) 2004 Tama Communications Corporation
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

#ifndef _LANGMAP_H_
#define _LANGMAP_H_

#include "strbuf.h"

#define DEFAULTLANGMAP  "c:.c.h,yacc:.y,asm:.s.S,java:.java,cpp:.c++.cc.cpp.cxx.hxx.hpp.C.H,php:.php.php3.phtml"
#define DEFAULTSKIP     "GPATH,GTAGS,GRTAGS,GSYMS,HTML/,HTML.pub/,html/,tags,TAGS,ID,y.tab.c,y.tab.h,.notfunction,cscope.out,cscope.po.out,cscope.in.out,.gdbinit,SCCS/,RCS/,CVS/,CVSROOT/,{arch}/,.svn/,.git/,.cvsrc,.cvsignore,.gitignore,.cvspass,.cvswrappers,.deps/,autom4te.cache/,.snprj/"

void setup_langmap(const char *);
const char *decide_lang(const char *);
void make_suffixes(const char *, STRBUF *);

#endif
