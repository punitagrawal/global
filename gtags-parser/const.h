/* This file is generated automatically by convert.pl from gtags-parser/manual.in. */
const char *progname = "gtags-parser";
const char *usage_const = "Usage: gtags-parser [-bdenrstvw] file ...\n";
const char *help_const = "Options:\n\
-b, --begin-block\n\
       Force level 1 block to begin when reach a left brace at the first column.\n\
       (C only)\n\
-d, --define\n\
       Pick up not only function but also macro without argument\n\
       as a definition.\n\
-e, --end-block\n\
       Force level 1 block to end when reach a right brace at the first column.\n\
       (C only)\n\
-n, --no-tags\n\
       Suppress output of tags. It is useful to use with -w option.\n\
-r, --reference\n\
       Locate function references instead of function definitions.\n\
       GTAGS is needed at the current directory.\n\
       (C, C++ and Java source only)\n\
       By default, locate function definitions.\n\
-s, --symbol\n\
       Collect symbols other than functions.\n\
       By default, locate function definitions.\n\
-t, --typedef\n\
       Pick up not only function but also typedef name and enum member\n\
       as a definition.\n\
-v, --verbose\n\
       Verbose mode.\n\
-w, --warning\n\
       Print warning message.\n\
--langmap=map\n\
       Language mapping. Each comma-separated  map  consists of\n\
       the language name, a colon, and a list of file extensions.\n\
       Default mapping is 'c:.c.h,yacc:.y,asm:.s.S,java:.java,cpp:.c++.cc.cpp.cxx.hxx.hpp.C.H,php:.php.php3.phtml'.\n\
See also:\n\
       GNU GLOBAL web site: http://www.gnu.org/software/global/\n\
";
