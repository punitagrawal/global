/* This file is generated automatically by convert.pl from gtags/manual.in. */
const char *progname = "gtags";
const char *usage_const = "Usage: gtags [-ciIOqvw][-d tag-file][-f file][dbpath]\n";
const char *help_const = "Options:\n\
--accept-dotfiles\n\
       Accept files and directories whose names begin with a dot.\n\
       By default, gtags ignores them.\n\
-c, --compact\n\
       Make GTAGS in compact format.\n\
       This option does not influence GRTAGS,\n\
       because that is always made in compact format.\n\
--config[=name]\n\
       Print the value of config variable name.\n\
       If name is not specified then print all names and values.\n\
-d, --dump tag-file\n\
       Dump a tag file as text to the standard output. Output format is 'key<tab>data'.\n\
       This is for debugging.\n\
-f, --file file\n\
       Browse through all files whose names are listed in file.\n\
       The argument file can be set to - to accept a list of\n\
       files from the standard input.\n\
       File names must be separated by newline.\n\
--gtagsconf file\n\
       Set environment variable GTAGSCONF to file.\n\
--gtagslabel label\n\
       Set environment variable GTAGSLABEL to label.\n\
-I, --idutils\n\
       In addition to tag files, make ID database for idutils(1).\n\
-i, --incremental\n\
       Update tag files incrementally.\n\
       It's better to use global(1) with the -u command.\n\
-O, --objdir\n\
       Use BSD-style objdir as the location of tag files.\n\
       If $MAKEOBJDIRPREFIX directory exists, gtags creates\n\
       $MAKEOBJDIRPREFIX/<current directory> directory and makes\n\
       tag files in it.\n\
       If dbpath is specified, this option is ignored.\n\
--single-update file\n\
       Update tag files for a single file.\n\
       It is considered that file was added or updated,\n\
       and there is no change in other files.\n\
       This option implies the -i option.\n\
--statistics\n\
       Print statistics information.\n\
-q, --quiet\n\
       Quiet mode.\n\
-v, --verbose\n\
       Verbose mode.\n\
-w, --warning\n\
       Print warning messages.\n\
dbpath\n\
       The directory in which tag files are generated.\n\
       The default is the current directory.\n\
See also:\n\
       GNU GLOBAL web site: http://www.gnu.org/software/global/\n\
";
