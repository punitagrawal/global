/* This file is generated automatically by convert.pl from global/manual.in. */
const char *progname = "global";
const char *usage_const = "Usage: global [-adEFGilMnqrstTvx][-S dir][-e] pattern\n\
       global -c[dFiIMoOPrsT] prefix\n\
       global -f[adlnqrstvx][-L file-list][-S dir] files\n\
       global -g[aEGilMnoOqtvVx][-L file-list][-S dir][-e] pattern [files]\n\
       global -I[ailMnqtvx][-S dir][-e] pattern\n\
       global -P[aEGilMnoOqtvVx][-S dir][-e] pattern\n\
       global -p[qrv]\n\
       global -u[qv]\n";
const char *help_const = "Commands:\n\
<no command> pattern\n\
       Print tags which match to pattern.\n\
       By default, print definition tags.\n\
-c, --completion [prefix]\n\
       Print symbols which start with prefix.\n\
       If prefix is not given, print all symbols.\n\
-f, --file files\n\
       Print all tags in the files.\n\
       This command implies the -x option.\n\
-g, --grep pattern [files]\n\
       Print all lines which match to the pattern.\n\
       If files are given, this command searches in those files.\n\
--help\n\
       Print a usage message.\n\
-I, --idutils pattern\n\
       Print all lines which match to pattern.\n\
       This function uses idutils(1) as a search engine.\n\
       To use this command, you need to install idutils(1)\n\
       in your system and execute gtags(1) with the -I option.\n\
-P, --path [pattern]\n\
       Print path names which match to pattern.\n\
       If no pattern is given, print all paths in the project.\n\
-p, --print-dbpath\n\
       Print location of GTAGS.\n\
-u, --update\n\
       Update tag files incrementally.\n\
       This command internally invokes gtags(1).\n\
       You can execute this command anywhere in the project,\n\
       differing from gtags(1).\n\
--version\n\
       Show version number.\n\
Options:\n\
-a, --absolute\n\
       Print absolute path names. By default, print relative path names.\n\
--color when\n\
       Use color to highlight the pattern within the line; when may be one of:\n\
       never, always or auto (default).\n\
       The default color is bold red text on current background; the environment\n\
       variable GREP_COLORS or GREP_COLOR defines it.\n\
       This option is effective to the following commands:\n\
       <no command>, -f, -g, -I, -P.\n\
-d, --definition\n\
       Print locations of definitions.\n\
-e, --regexp pattern\n\
       Use pattern as the pattern; useful to protect patterns starting with -.\n\
-E, --extended-regexp\n\
       Interpret pattern as a extended regular expression.\n\
       This is the default.\n\
--encode-path chars\n\
       Convert path characters in chars into a % symbol, followed by the\n\
       two-digit hexadecimal representation of the character.\n\
       A blank will be converted to %20.\n\
-F, --first-match\n\
       End the search without go through all the tag files listed in GTAGSLIBPATH\n\
       when tags are found in a tag file. This is the default.\n\
--from-here context\n\
       Decide tag type by context, which must be lineno:path.\n\
       If this option is specified then -s and -r are ignored.\n\
       Regular expression is not allowed for pattern.\n\
       This option assumes use in conversational environments such as\n\
       editors and IDEs.\n\
-G, --basic-regexp\n\
       Interpret pattern as a basic regular expression.\n\
       The default is an extended regular expression.\n\
--gtagsconf file\n\
       Set environment variable GTAGSCONF to file.\n\
--gtagslabel label\n\
       Set environment variable GTAGSLABEL to label.\n\
-i, --ignore-case\n\
       Ignore case distinctions in the pattern.\n\
-L, --file-list file\n\
       Obtain files from file in addition to the arguments.\n\
       The argument file can be set to '-' to accept a list of files\n\
       from the standard input. File names must be separated by newline.\n\
-l, --local\n\
       Print only tags which exist under the current directory.\n\
--literal\n\
       Execute literal search instead of regular expression search.\n\
       This option is only valid when the -g command is specified.\n\
-M, --match-case\n\
       Search is case-sensitive. This is the default.\n\
--match-part part\n\
       Specify how path name completion should match, where part is one of:\n\
       first, last or all (default).\n\
       This option is valid only with the -c command in conjunction with -P.\n\
-n, --nofilter\n\
       Suppress sort filter and path conversion filter.\n\
-O, --only-other\n\
       Treat only text files other than source code, like README.\n\
       This option is valid only with the -g or -P command.\n\
       This option overrides the -o option.\n\
-o, --other\n\
       Treat not only source files but also text files other than source code,\n\
       like README.\n\
       This option is valid only with the -g or -P command.\n\
--path-style format\n\
       Print path names using format, which may be one of:\n\
       relative, absolute, shorter, abslib or through.\n\
       The --path-style option is given more priority than the -a option.\n\
--print0\n\
       Print each record followed by a null character instead of a newline.\n\
-q, --quiet\n\
       Quiet mode.\n\
-r, --reference, --rootdir\n\
       Print reference tags.\n\
       Reference means the reference to a symbol which has definitions.\n\
       With the -p option, print the root directory of the project.\n\
--result format\n\
       Print out using format, which may be one of:\n\
       path (default), ctags, ctags-x, grep or cscope.\n\
       The --result=ctags and --result=ctags-x options are\n\
       equivalent to the -t and -x options respectively.\n\
       The --result option is given more priority than the -t and -x options.\n\
--single-update file\n\
       Update tag files using gtags(1) with the --single-update option.\n\
       It is considered that file was added or updated,\n\
       and there is no change in other files.\n\
       This option implies the -u option.\n\
-s, --symbol\n\
       Print other symbol tags.\n\
       Other symbol means the reference to a symbol which has no definition.\n\
-S, --scope dir\n\
       Print only tags which exist under dir directory.\n\
       It is similar to the -l option, but you need not change directory.\n\
-T, --through\n\
       Go through all the tag files listed in GTAGSLIBPATH.\n\
       By default, stop searching when tag is found.\n\
       This option is ignored when either -s, -r\n\
       or -l option is specified.\n\
-t, --tags\n\
       Use standard ctags format.\n\
-V, --invert-match\n\
       Invert the sense of matching, to select non-matching lines.\n\
       This option is valid only with the -g or -P commands.\n\
-v, --verbose\n\
       Verbose mode.\n\
-x, --cxref\n\
       Use standard ctags cxref (with -x) format.\n\
See also:\n\
       GNU GLOBAL web site: http://www.gnu.org/software/global/\n\
";
