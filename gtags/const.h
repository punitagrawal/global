/* This file is generated automatically by convert.pl from gtags/manual.in. */
const char *progname = "gtags";
const char *usage_const = "Usage: gtags [-c][-i][-I][-o][-P][-q][-v][-w][dbpath]\n";
const char *help_const = "Options:\n\
-c, --compact\n\
       Make tag files with compact format.\n\
--config name\n\
       Show the value of config variable name.\n\
       If name is not specified then show whole of config entry.\n\
--gtagsconf file\n\
       Load user's configuration from file.\n\
-i, --incremental\n\
       Update tag files incrementally. You had better use\n\
       global(1) with the -u option.\n\
-I, --idutils\n\
       Make index files for id-utils(1).\n\
--info info\n\
       Pass info string to external system.\n\
       Currently you can use it with -P option.\n\
-o, --omit-gsyms\n\
       Suppress making GSYMS file.\n\
       Use this option if you don't use -s option of global(1).\n\
-P, --postgres\n\
       Use postgres database system instead of db(3).\n\
       you can pass info string to PQconnectdb(3)\n\
       using --info option.\n\
-q, --quiet\n\
       Quiet mode.\n\
-v, --verbose\n\
       Verbose mode.\n\
-w, --warning\n\
       Print warning messages.\n\
";
