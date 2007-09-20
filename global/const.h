/* This file is generated automatically by convert.pl from global/manual.in. */
const char *progname = "global";
const char *usage_const = "Usage: global [-ailnqrstTvx] pattern\n\
       global -c[qsv] prefix\n\
       global -f[anqrstvx] files\n\
       global -g[ailnoqtvx] pattern\n\
       global -I[ailnqtvx] pattern\n\
       global -p[qrv]\n\
       global -P[ailnqtvx] pattern\n\
       global -u[qv]\n";
const char *help_const = "Commands:\n\
pattern\n\
       Print object which match to the pattern.\n\
       It can include POSIX 1003.2 regular expression.\n\
-c, --completion [prefix]\n\
       Print candidate function names which start with specified\n\
       prefix. Prefix is not specified,\n\
       print all function names.\n\
-f, --file files\n\
       Print all function definitions in the files.\n\
       This option implies -x option.\n\
-g, --grep pattern\n\
       Print all lines which match to the pattern.\n\
       This function use grep(1) as a search engine.\n\
-I, --idutils pattern\n\
       Print all lines which match to the pattern.\n\
       This function use id-utils(1) as a search engine.\n\
       To use this command, you need to install id-utils(1)\n\
       in your system and you must execute gtags(1)\n\
       with -I option.\n\
-p, --print-dbpath\n\
       Print the location of GTAGS.\n\
-P, --path [pattern]\n\
       Print the path which match to the pattern.\n\
       If no pattern specified, print all.\n\
-u, --update\n\
       Locate tag files and update them incrementally.\n\
--version\n\
       Show version number.\n\
--help\n\
       Show help.\n\
Options:\n\
-a, --absolute\n\
       Print absolute path name. By default, print relative path name.\n\
-i, --ignore-case\n\
       ignore case distinctions in pattern.\n\
-l, --local\n\
       Print just objects which exist under the current directory.\n\
-n, --nofilter\n\
       Suppress sort filter and path conversion filter.\n\
-o, --other\n\
       Search pattern in not only source files but also other files\n\
       like README.\n\
       This options is valid only with -g option.\n\
-q, --quiet\n\
       Quiet mode.\n\
-r, --reference, --rootdir\n\
       Print the locations of object references.\n\
       By default, print object definitions.\n\
       With the -p option, print the root directory of source tree.\n\
-s, --symbol pattern\n\
       Print the locations of specified symbol other than function names.\n\
       You need GSYMS tags file. See gtags(1).\n\
-t, --tags\n\
       Print with standard ctags format.\n\
-T, --through\n\
       Go through all the tag files listed in GTAGSLIBPATH.\n\
       By default, stop searching when tag is found.\n\
       This option is ignored when either -s, -r\n\
       or -l option is specified.\n\
-v, --verbose\n\
       Verbose mode.\n\
-x, --cxref\n\
       In addition to the default output, produce the line number and\n\
       the line contents.\n\
";
