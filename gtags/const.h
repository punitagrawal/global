/* This file is generated automatically by convert.pl from gtags/manual.in. */
const char *progname = "gtags";
const char *usage_const = "Usage: gtags [-ciIOqvw][-C dir][-d tag-file][-f file][dbpath]\n";
const char *help_const = "Options:\n\
--accept-dotfiles\n\
       Accept files and directories whose names begin with a dot.\n\
       By default, gtags ignores them.\n\
-c, --compact\n\
       Make GTAGS in compact format.\n\
       This option does not influence GRTAGS,\n\
       because that is always made in compact format.\n\
-C, --directory dir\n\
       Change the directory before doing all the work including parameter analysis.\n\
       This option is ignored in GTAGS_OPTIONS.\n\
       Please specify it on the command line directly.\n\
--config[=name]\n\
       Print the value of config variable name.\n\
       If name is not specified then print all names and values.\n\
       In addition to the variables listed in the ENVIRONMENT section,\n\
       you can refer to install directories by read only variables:\n\
       bindir, libdir, datadir, localstatedir and sysconfdir.\n\
-d, --dump tag-file\n\
       Dump a tag file as text to the standard output.\n\
       Output format is 'key<tab>data'. This is for debugging.\n\
--explain\n\
       Explain handling files.\n\
-f, --file file\n\
       Give a list of candidates of target files.\n\
       Files which are not on the list are ignored.\n\
       The argument file can be set to - to accept a list of\n\
       files from the standard input.\n\
       File names must be separated by newline.\n\
       To make the list you may use find(1), which has rich options\n\
       for selecting files.\n\
--gtagsconf file\n\
       Set environment variable GTAGSCONF to file.\n\
--gtagslabel label\n\
       Set environment variable GTAGSLABEL to label.\n\
--help\n\
       Print a usage message.\n\
-I, --idutils\n\
       In addition to tag files, make ID database for idutils(1).\n\
-i, --incremental\n\
       Update tag files incrementally.\n\
       It's better to use global(1) with the -u command.\n\
-O, --objdir\n\
       Use BSD-style obj directory as the location of tag files.\n\
       If GTAGSOBJDIRPREFIX is set and $GTAGSOBJDIRPREFIX directory exists,\n\
       gtags creates $GTAGSOBJDIRPREFIX/<current directory> directory\n\
       and makes tag files in it.\n\
       Though you can use MAKEOBJDIRPREFIX instead of GTAGSOBJDIRPREFIX,\n\
       it is deprecated.\n\
       If dbpath is specified, this option is ignored.\n\
--single-update file\n\
       Update tag files for a single file.\n\
       It is considered that file was added, updated or deleted,\n\
       and there is no change in other files.\n\
       This option implies the -i option.\n\
--skip-unreadable\n\
       Skip unreadable files.\n\
--skip-symlink [=type]\n\
       Skip symbolic links. If type is 'f' then skip only symbolic links for\n\
       file, else if 'd' then skip only symbolic links for directory.\n\
       The default value of type is 'a' (all symbolic links).\n\
--sqlite3\n\
       Use Sqlite 3 API to make tag files. By default, BSD/DB 1.85 API is used.\n\
       To use this option, you need to invoke configure script with\n\
       --with-sqlite3 in the build phase.\n\
--statistics\n\
       Print statistics information.\n\
-q, --quiet\n\
       Quiet mode.\n\
-v, --verbose\n\
       Verbose mode.\n\
--version\n\
       Show version number.\n\
-w, --warning\n\
       Print warning messages.\n\
dbpath\n\
       The directory in which tag files are generated.\n\
       The default is the current directory.\n\
See also:\n\
       GNU GLOBAL web site: http://www.gnu.org/software/global/\n\
";
