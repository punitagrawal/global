/*
 * Copyright (c) 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003, 2004, 2005,
 *      2006, 2007, 2008, 2010, 2011 Tama Communications Corporation
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <stdio.h>
#include <errno.h>
#ifdef STDC_HEADERS
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#else
#include <sys/file.h>
#endif
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <errno.h>

#include "args.h"
#include "checkalloc.h"
#include "getopt.h"
#include "regex.h"
#include "global.h"
#include "anchor.h"
#include "cache.h"
#include "common.h"
#include "htags.h"
#include "incop.h"
#include "path2url.h"
#include "const.h"

/**
 * @file
 * @NAME{htags} - generate hypertext (XHTML or HTML) pages from a set of source files.
 */

void src2html(const char *, const char *, int);
int makedupindex(void);
int makedefineindex(const char *, int, STRBUF *);
int makefileindex(const char *, STRBUF *);
void makeincludeindex(void);
int makecflowindex(const char *, const char *);

#if defined(_WIN32) && !defined(__CYGWIN__)
#define mkdir(path,mode) mkdir(path)
#define link(one,two) (-1)
#endif

/*
 * Global data.
 */
int w32 = W32;				/**< Windows32 environment	*/
const char *www = "http://www.gnu.org/software/global/";
int html_count = 0;
int sep = '/';
int need_bless;
const char *save_config;
const char *save_argv;

char cwdpath[MAXPATHLEN];
char dbpath[MAXPATHLEN];
char distpath[MAXPATHLEN];
char gtagsconf[MAXPATHLEN];
char datadir[MAXPATHLEN];
char localstatedir[MAXPATHLEN];

char gtags_path[MAXFILLEN];
char global_path[MAXFILLEN];
int gtags_exist[GTAGLIM];
const char *null_device = NULL_DEVICE;
const char *tmpdir = "/tmp";
const char *sitekey = "";

/**
 * @details
 * Order of items in the top page (This should be customisable variable in the future).
 *
 * @CODE{'c'}: caution <br>
 * @CODE{'s'}: search form <br>
 * @CODE{'m'}: mains <br>
 * @CODE{'d'}: definitions <br>
 * @CODE{'f'}: files <br>
 * @CODE{'t'}: call tree
 */
char *item_order = "csmdft";
/*
 * options
 */
int aflag;				/**< @OPTION{--alphabet(-a)} option	*/
int cflag;				/**< @OPTION{--compact(-c)} option		*/
int fflag;				/**< @OPTION{--form(-f)} option		*/
int Fflag;				/**< @OPTION{--frame(-F)} option		*/
int gflag;				/**< @OPTION{--gtags(-g)} option		*/
int Iflag;				/**< @OPTION{--icon(-I)} option		*/
int nflag;				/**< @OPTION{--line-number(-n)} option	*/
int Sflag;				/**< @OPTION{--system-cgi} option		*/
int qflag;
int vflag;				/**< @OPTION{--verbose(-v)} option		*/
int wflag;				/**< @OPTION{--warning(-w)} option		*/
int debug;				/**< @OPTION{--debug} option		*/

int show_help;				/**< @OPTION{--help} command		*/
int show_version;			/**< @OPTION{--version} command		*/
int caution;				/**< @OPTION{--caution} option		*/
int dynamic;				/**< @OPTION{--dynamic(-D)} option		*/
int symbol;				/**< @OPTION{--symbol(-s)} option          */
int suggest;				/**< @OPTION{--suggest} option		*/
int suggest2;				/**< @OPTION{--suggest2} option		*/
int auto_completion;			/**< @OPTION{--auto-completion}		*/
int tree_view;				/**< @OPTION{--tree-view}			*/
int fixed_guide;			/**< @OPTION{--fixed-guide}		*/
const char *tree_view_type;		/**< @OPTION{--type-view=[type]}		*/
char *auto_completion_limit = "0";	/**< @OPTION{--auto-completion=limit}	*/
int statistics = STATISTICS_STYLE_NONE;	/**< @OPTION{--statistics} option		*/

int no_order_list;			/**< 1: doesn't use order list	*/
int other_files;			/**< 1: list other files		*/
int enable_grep = 1;			/**< 1: enable grep		*/
int enable_idutils = 1;			/**< 1: enable idutils		*/
int enable_xhtml = 1;			/**< 1: enable XHTML		*/

const char *main_func = "main";
const char *cvsweb_url;
int use_cvs_module;
const char *cvsweb_cvsroot;
const char *gtagslabel;
const char *title;
const char *xhtml_version = "1.0";
const char *insert_header;		/**< @OPTION{--insert-header=\<file\>}	*/
const char *insert_footer;		/**< @OPTION{--insert-footer=\<file\>}	*/
const char *html_header;		/**< @OPTION{--html-header=\<file\>}		*/
const char *jscode;			/**< javascript code		*/
/**
 * @name Constant values.
 */
/** @{ */
const char *title_define_index = "DEFINITIONS";
const char *title_file_index = "FILES";
const char *title_call_tree = "CALL TREE";
const char *title_callee_tree = "CALLEE TREE";
const char *title_included_from = "INCLUDED FROM";
/** @} */
/*
 * Function header items.
 */
const char *anchor_label[] = {
	"&lt;",
	"&gt;",
	"^",
	"v",
	"top",
	"bottom",
	"index",
	"help"
};
const char *anchor_icons[] = {
	"left",
	"right",
	"first",
	"last",
	"top",
	"bottom",
	"index",
	"help"
};
const char *anchor_comment[] = {
	"previous",
	"next",
	"first",
	"last",
	"top",
	"bottom",
	"index",
	"help"
};
const char *anchor_msg[] = {
	"Previous definition.",
	"Next definition.",
	"First definition in this file.",
	"Last definition in this file.",
	"Top of this file.",
	"Bottom of this file.",
	"Return to index page.",
	"You are seeing now."
};
const char *back_icon = "back";
const char *dir_icon  = "dir";
const char *c_icon = "c";
const char *file_icon = "text";

const char *icon_files[] = {
	"first",
	"last",
	"left",
	"right",
	"top",
	"bottom",
	"n_first",
	"n_last",
	"n_left",
	"n_right",
	"n_top",
	"n_bottom",
	"index",
	"help",
	"back",
	"dir",
	"c",
	"text",
	"pglobe"
};
/**
 * @name Configuration parameters.
 */
/** @{ */
int ncol = 4;				/**< columns of line number	*/
int tabs = 8;				/**< tab skip			*/
int flist_fields = 5;			/**< fields number of file list	*/
int full_path = 0;			/**< file index format		*/
int map_file = 0;			/**< 1: create MAP file		*/
int filemap_file = 1;			/**< 1: create FILEMAP file	*/
int overwrite_key = 0;			/**< 1: over write site key	*/
const char *icon_suffix = "png";	/**< icon suffix (jpg, png etc)	*/
const char *icon_spec = "border='0' align='top'"; /**< parameter in IMG tag*/
const char *prolog_script = NULL;	/**< include script at first	*/
const char *epilog_script = NULL;	/**< include script at last	*/
const char *call_file = NULL;		/**< file name of cflow output	*/
const char *callee_file = NULL;		/**< file name of cflow output	*/
int show_position = 0;			/**< show current position	*/
int table_list = 0;			/**< tag list using table tag	*/
int table_flist = 0;			/**< file list using table tag	*/
int colorize_warned_line = 0;		/**< colorize warned line		*/
const char *script_alias = "/cgi-bin";	/**< script alias of WWW server	*/
const char *gzipped_suffix = "ghtml";	/**< suffix of gzipped html file	*/
const char *normal_suffix = "html";	/**< suffix of normal html file	*/
const char *HTML;					/**< HTML */
const char *action = "cgi-bin/global.cgi"; /**< default action		*/
const char *completion_action = "cgi-bin/completion.cgi";	/**< completion_action */
int definition_header=NO_HEADER;	/**< (NO|BEFORE|RIGHT|AFTER)_HEADER */
const char *include_file_suffixes = DEFAULTINCLUDEFILESUFFIXES;	/**< include_file_suffixes */
static const char *langmap = DEFAULTLANGMAP;	/**< langmap */
int grtags_is_empty = 0;						/**< grtags_is_empty */
/** @} */

static struct option const long_options[] = {
	/*
	 * These options have long name and short name.
	 * We throw them to the processing of short options.
	 */
        {"alphabet", no_argument, NULL, 'a'},
        {"compact", no_argument, NULL, 'c'},
        {"dbpath", required_argument, NULL, 'd'},
        {"dynamic", no_argument, NULL, 'D'},
        {"form", no_argument, NULL, 'f'},
        {"frame", no_argument, NULL, 'F'},
        {"func-header", optional_argument, NULL, 'h'},
        {"gtags", no_argument, NULL, 'g'},
        {"icon", no_argument, NULL, 'I'},
        {"line-number", optional_argument, NULL, 'n'},
        {"main-func", required_argument, NULL, 'm'},
        {"other", no_argument, NULL, 'o'},
        {"system-cgi", required_argument, NULL, 'S'},
        {"symbol", no_argument, NULL, 's'},
        {"table-flist", optional_argument, NULL, 'T'},
        {"title", required_argument, NULL, 't'},
        {"verbose", no_argument, NULL, 'v'},
        {"warning", no_argument, NULL, 'w'},
        {"xhtml", optional_argument, NULL, 'x'},

        /*
	 * The following are long name only.
	 */
	/* flag value */
        {"caution", no_argument, &caution, 1},
        {"colorize-warned-line", no_argument, &colorize_warned_line, 1},
        {"debug", no_argument, &debug, 1},
        {"disable-grep", no_argument, &enable_grep, 0},
        {"disable-idutils", no_argument, &enable_idutils, 0},
        {"full-path", no_argument, &full_path, 1},
        {"fixed-guide",  no_argument, &fixed_guide, 1},
        {"map-file", no_argument, &map_file, 1},
        {"no-order-list", no_argument, &no_order_list, 1},
        {"overwrite-key", no_argument, &overwrite_key, 1},
        {"show-position", no_argument, &show_position, 1},
        {"statistics", no_argument, &statistics, STATISTICS_STYLE_TABLE},
        {"suggest", no_argument, &suggest, 1},
        {"suggest2", no_argument, &suggest2, 1},
        {"table-list", no_argument, &table_list, 1},
        {"version", no_argument, &show_version, 1},
        {"help", no_argument, &show_help, 1},

	/* accept value */
#define OPT_CVSWEB		128
#define OPT_CVSWEB_CVSROOT	129
#define OPT_GTAGSCONF		130
#define OPT_GTAGSLABEL		131
#define OPT_NCOL		132
#define OPT_INSERT_FOOTER	133
#define OPT_INSERT_HEADER	134
#define OPT_ITEM_ORDER		135
#define OPT_TABS		136
#define OPT_CFLOW		137
#define OPT_AUTO_COMPLETION	138
#define OPT_TREE_VIEW		139
#define OPT_HTML_HEADER		140
#define OPT_CALL_TREE		141
#define OPT_CALLEE_TREE		142
        {"auto-completion", optional_argument, NULL, OPT_AUTO_COMPLETION},
        {"call-tree", required_argument, NULL, OPT_CALL_TREE},
        {"callee-tree", required_argument, NULL, OPT_CALLEE_TREE},
        {"cflow", required_argument, NULL, OPT_CFLOW},
        {"cvsweb", required_argument, NULL, OPT_CVSWEB},
        {"cvsweb-cvsroot", required_argument, NULL, OPT_CVSWEB_CVSROOT},
        {"gtagsconf", required_argument, NULL, OPT_GTAGSCONF},
        {"gtagslabel", required_argument, NULL, OPT_GTAGSLABEL},
        {"html-header", required_argument,NULL, OPT_HTML_HEADER},
        {"ncol", required_argument, NULL, OPT_NCOL},
        {"insert-footer", required_argument, NULL, OPT_INSERT_FOOTER},
        {"insert-header", required_argument, NULL, OPT_INSERT_HEADER},
        {"item-order", required_argument, NULL, OPT_ITEM_ORDER},
	{"tabs", required_argument, NULL, OPT_TABS},
        {"tree-view",  optional_argument, NULL, OPT_TREE_VIEW},
        { 0 }
};

static void
usage(void)
{
        if (!qflag)
                fputs(usage_const, stderr);
        exit(2);
}
static void
help(void)
{
        fputs(usage_const, stdout);
        fputs(help_const, stdout);
        exit(0);
}
/**
 * Htags catch signal even if the parent ignore it.
 */
void
clean(void)
{
	unload_gpath();
	cache_close();
}
/**
 * Signal handler.
 *
 * This handler is set up in signal_setup().
 */
static void
suddenly(int signo)
{
        signo = 0;      /* to satisfy compiler */

	clean();
	exit(1);
}

/**
 * Setup signal hander.
 *
 * Makes signals @CODE{SIGINT}, @CODE{SIGTERM}, @CODE{SIGHUP} and @CODE{SIGQUIT}
 * call suddenly() if triggered.
 */
static void
signal_setup(void)
{
        signal(SIGINT, suddenly);
        signal(SIGTERM, suddenly);
#ifdef SIGHUP
        signal(SIGHUP, suddenly);
#endif
#ifdef SIGQUIT
        signal(SIGQUIT, suddenly);
#endif
}

/**
 * make directory in the dist (#distpath) directory.
 *
 *     @param[in]      name    name of directory to create.
 *
 * Creates a file called @FILE{index.html} in the new directory.
 *
 * @remark @NAME{mkdir()} creates the directory in mode @CODE{0775}, if doesn't exist.
 */
static void
make_directory_in_distpath(const char *name)
{
	char path[MAXPATHLEN];
	FILE *op;

	strlimcpy(path, makepath(distpath, name, NULL), sizeof(path));
	if (!test("d", path))
		if (mkdir(path, 0775))
			die("cannot make directory '%s'.", path);
	/*
	 * Not to publish the directory list.
	 */
	op = fopen(makepath(path, "index.html", NULL), "w");
	if (op == NULL)
		die("cannot make file '%s'.", makepath(path, "index.html", NULL));
	fputs(html_begin, op);
	fputs(html_end, op);
	fputc('\n', op);
	fclose(op);
}
/**
 * make file in the dist (#distpath) directory.
 */
static void
make_file_in_distpath(const char *name, const char *data)
{
	FILE *op;
	const char *path = makepath(distpath, name, NULL);

	op = fopen(path, "w");
	if (op) {
		if (data && *data) {
			fputs(data, op);
			fputc('\n', op);
		}
		fclose(op);
	} else {
		die("cannot make file '%s'.", path); 
	}
}
void
load_with_replace(const char *file, STRBUF *result, int place)
{
	STRBUF *sb = strbuf_open(0);
	FILE *ip;
	regex_t preg;
	regmatch_t pmatch[2];
	char *_;
	int i;

        struct map {
		const char *name;
		const char *value;
        } tab[] = {
		/* dynamic initialization */
		{"@page_begin@", NULL},
		{"@page_end@", NULL},

		/* static initialization */
		{"@body_begin@", body_begin},
		{"@body_end@", body_end},
		{"@title_begin@", title_begin},
		{"@title_end@", title_end},
		{"@error_begin@", error_begin},
		{"@error_end@", error_end},
		{"@message_begin@", message_begin},
		{"@message_end@", message_end},
		{"@verbatim_begin@", verbatim_begin},
		{"@verbatim_end@", verbatim_end},
		{"@normal_suffix@", normal_suffix},
		{"@hr@", hr},
		{"@br@", br},
		{"@HTML@", HTML},
		{"@DATADIR@", datadir},
		{"@LOCALSTATEDIR@", localstatedir},
		{"@action@", action},
		{"@completion_action@", completion_action},
		{"@limit@", auto_completion_limit},
		{"@sitekey@", sitekey},
		{"@script_alias@", script_alias},
		{"@null_device@", null_device},
		{"@globalpath@", global_path},
		{"@gtagspath@", gtags_path},
        };
	int tabsize = sizeof(tab) / sizeof(struct map);

	tab[0].value = gen_page_begin("Result", place);
	tab[1].value = gen_page_end();
	/*
	 * construct regular expression.
	 */
	strbuf_putc(sb, '(');
	for (i = 0; i < tabsize; i++) {
		strbuf_puts(sb, tab[i].name);
		strbuf_putc(sb, '|');
	}
	strbuf_unputc(sb, '|');
	strbuf_putc(sb, ')');
	if (regcomp(&preg, strbuf_value(sb), REG_EXTENDED) != 0)
		die("cannot compile regular expression.");
	/*
	 * construct skeleton file name in the system datadir directory.
	 */
	strbuf_reset(sb);
	strbuf_sprintf(sb, "%s/gtags/%s.tmpl", datadir, file);
	ip = fopen(strbuf_value(sb), "r");
	if (!ip) {
#ifdef __DJGPP__
		strbuf_reset(sb);
		strbuf_sprintf(sb, "%s/gtags/%s", datadir, file);
		ip = fopen(strbuf_value(sb), "r");
		if (!ip)
#endif
			die("skeleton file '%s' not found.", strbuf_value(sb));
	}
	strbuf_reset(sb);
	/*
	 * Read template file and evaluate macros.
	 */
	while ((_ = strbuf_fgets(sb, ip, STRBUF_NOCRLF)) != NULL) {
		const char *p;

		/* Pick up macro name */
		for (p = _; !regexec(&preg, p, 2, pmatch, 0); p += pmatch[0].rm_eo) {
			const char *start = p + pmatch[0].rm_so;
			int length = pmatch[0].rm_eo - pmatch[0].rm_so;

			/* print before macro */
			for (i = 0; i < pmatch[0].rm_so; i++)
				strbuf_putc(result, p[i]);
			for (i = 0; i < tabsize; i++)
				if (!strncmp(start, tab[i].name, length))
					break;
			if (i >= tabsize)
				die("something wrong.");
			/* print macro value */
			if (i < tabsize) {
				const char *q;
				/*
				 * Double quote should be quoted using '\\'.
				 */
				for (q = tab[i].value; *q; q++) {
					if (*q == '"')
						strbuf_putc(result, '\\');
					else if (*q == '\n')
						strbuf_putc(result, '\\');
					strbuf_putc(result, *q);
				}
			}
		}
		strbuf_puts_nl(result, p);
	}
	fclose(ip);
	strbuf_close(sb);
	regfree(&preg);
}
/**
 * generate_file: generate file with replacing macro.
 *
 *	@param[in]	dist	directory where the file should be created
 *	@param[in]	file	file name
 *	@param[in]	place	#TOPDIR, #SUBDIR, #CGIDIR
 */
static void
generate_file(const char *dist, const char *file, int place)
{
	FILE *op;
	STRBUF *result = strbuf_open(0);

	op = fopen(makepath(dist, file, NULL), "w");
	if (!op)
		die("cannot create file '%s'.", file);
	load_with_replace(file, result, place);
	fputs(strbuf_value(result), op);
	fclose(op);
	html_count++;
	strbuf_close(result);
}
/**
 * makeprogram: make @NAME{CGI} program
 */
static void
makeprogram(const char *cgidir, const char *file)
{
	generate_file(cgidir, file, CGIDIR);
}
/**
 * makebless: make @FILE{bless.sh} file.
 */
static void
makebless(const char *file)
{
	generate_file(distpath, file, SUBDIR);
}
/**
 * makeghtml: make @FILE{ghtml.cgi} file.
 *
 *	@param[in]	cgidir	directory where the file should be created
 *	@param[in]	file	file name
 */
static void
makeghtml(const char *cgidir, const char *file)
{
	generate_file(cgidir, file, SUBDIR);
}
/**
 * makerebuild: make rebuild script
 */
static void
makerebuild(const char *file)
{
	FILE *op;

	op = fopen(makepath(distpath, file, NULL), "w");
	if (!op)
		die("cannot make rebuild script.");
	fputs_nl("#!/bin/sh", op);
	fputs_nl("#", op);
	fputs_nl("# rebuild.sh: rebuild hypertext with the previous context.", op);
	fputs_nl("#", op);
	fputs_nl("# Usage:", op);
	fputs_nl("#\t% sh rebuild.sh", op);
	fputs_nl("#", op);
	fprintf(op, "cd %s && GTAGSCONF='%s' htags%s\n", cwdpath, save_config, save_argv);
        fclose(op);
}
/**
 * makehelp: make help file
 */
static void
makehelp(const char *file)
{
	const char **label = Iflag ? anchor_comment : anchor_label;
	const char **icons = anchor_icons;
	const char **msg   = anchor_msg;
	int n, last = 7;
	FILE *op;

	op = fopen(makepath(distpath, file, NULL), "w");
	if (!op)
		die("cannot make help file.");
	fputs_nl(gen_page_begin("HELP", TOPDIR), op);
	fputs_nl(body_begin, op);
	fputs(header_begin, op);
	fputs("Usage of Links", op);
	fputs_nl(header_end, op);
	if (!Iflag)
		fputs(verbatim_begin, op);
	fputs("/* ", op);
	for (n = 0; n <= last; n++) {
		if (Iflag) {
			fputs(gen_image(CURRENT, icons[n], label[n]), op);
			if (n < last)
				fputc(' ', op);
		} else {
			fprintf(op, "[%s]", label[n]);
		}
	}
	if (show_position)
		fprintf(op, "[+line file]");
	fputs(" */", op);
	if (!Iflag)
		fputs_nl(verbatim_end, op);
	else
		fputc('\n', op);
	fputs_nl(define_list_begin, op);
	for (n = 0; n <= last; n++) {
		fputs(define_term_begin, op);
		if (Iflag) {
			fputs(gen_image(CURRENT, icons[n], label[n]), op);
		} else {
			fprintf(op, "[%s]", label[n]);
		}
		fputs(define_term_end, op);
		fputs(define_desc_begin, op);
		fputs(msg[n], op);
		fputs_nl(define_desc_end, op);
	}
	if (show_position) {
		fputs(define_term_begin, op);
		fputs("[+line file]", op);
		fputs(define_term_end, op);
		fputs(define_desc_begin, op);
		fputs("Current position (line number and file name).", op);
		fputs_nl(define_desc_end, op);
	}
	fputs_nl(define_list_end, op);
	fputs_nl(body_end, op);
	fputs_nl(gen_page_end(), op);
	fclose(op);
	html_count++;
}
/**
 * makesearchpart: make search part
 *
 *	@param[in]	target	\$target
 *	@return		html
 */
static char *
makesearchpart(const char *target)
{
	STATIC_STRBUF(sb);

	strbuf_clear(sb);
	strbuf_puts(sb, header_begin);
	if (Fflag)
		strbuf_puts(sb, gen_href_begin(NULL, "search", normal_suffix, NULL));
	strbuf_puts(sb, "SEARCH");
	if (Fflag)
		strbuf_puts(sb, gen_href_end());
	strbuf_puts_nl(sb, header_end);
	if (!target) {
		strbuf_puts(sb, "Please input object name and select [Search]. POSIX's regular expression is allowed.");
		strbuf_puts_nl(sb, br);
	}
	strbuf_puts_nl(sb, gen_form_begin(target));
	strbuf_puts_nl(sb, gen_input("pattern", NULL, NULL));
	strbuf_puts_nl(sb, gen_input("id", sitekey, "hidden"));
	strbuf_puts_nl(sb, gen_input(NULL, "Search", "submit"));
	strbuf_puts(sb, gen_input(NULL, "Reset", "reset"));
	strbuf_puts_nl(sb, br);
	strbuf_puts(sb, gen_input_radio("type", "definition", 1, "Retrieve the definition place of the specified symbol."));
	strbuf_puts_nl(sb, target ? "Def" : "Definition");
	strbuf_puts(sb, gen_input_radio("type", "reference", 0, "Retrieve the reference place of the specified symbol."));
	strbuf_puts_nl(sb, target ? "Ref" : "Reference");
	strbuf_puts(sb, gen_input_radio("type", "symbol", 0, "Retrieve the place of the specified symbol is used."));
	strbuf_puts_nl(sb, target ? "Sym" : "Other symbol");
	strbuf_puts(sb, gen_input_radio("type", "path", 0, "Look for path name which matches to the specified pattern."));
	strbuf_puts_nl(sb, target ? "Path" : "Path name");
	if (enable_grep) {
		strbuf_puts(sb, gen_input_radio("type", "grep", 0, "Retrieve lines which matches to the specified pattern."));
		strbuf_puts_nl(sb, target ? "Grep" : "Grep pattern");
	}
	if (enable_idutils && test("f", makepath(dbpath, "ID", NULL))) {
		strbuf_puts(sb, gen_input_radio("type", "idutils", 0, "Retrieve lines which matches to the specified pattern using idutils(1)."));
		strbuf_puts_nl(sb, target ? "Id" : "Id pattern");
	}
	strbuf_puts_nl(sb, br);
	strbuf_puts(sb, gen_input_checkbox("icase", NULL, "Ignore case distinctions in the pattern."));
	strbuf_puts_nl(sb, target ? "Icase" : "Ignore case");
	if (other_files) {
		strbuf_puts(sb, gen_input_checkbox("other", NULL, "Files other than the source code are also retrieved."));
		strbuf_puts_nl(sb, target ? "Other" : "Other files");
	}
	if (other_files && !target) {
		strbuf_puts_nl(sb, br);
		strbuf_puts(sb, "('Other files' is effective only to 'Path name'");
		if (enable_grep)
			strbuf_puts(sb, " and 'Grep pattern'");
		strbuf_puts_nl(sb, ".)");
	}
	strbuf_puts_nl(sb, gen_form_end());
	return strbuf_value(sb);
}
/**
 * makeindex: make index file
 *
 *	@param[in]	file	file name
 *	@param[in]	title	title of index file
 *	@param[in]	index	common part
 */
static void
makeindex(const char *file, const char *title, const char *index)
{
	FILE *op;

	op = fopen(makepath(distpath, file, NULL), "w");
	if (!op)
		die("cannot make file '%s'.", file);
	if (Fflag) {
		fputs_nl(gen_page_frameset_begin(title), op);
		fputs_nl(gen_frameset_begin("cols='200,*'"), op);
		if (fflag) {
			fputs_nl(gen_frameset_begin("rows='33%,33%,*'"), op);
			fputs_nl(gen_frame("search", makepath(NULL, "search", normal_suffix)), op);
		} else {
			fputs_nl(gen_frameset_begin("rows='50%,*'"), op);
		}
		/*
		 * id='xxx' for XHTML
		 * name='xxx' for HTML
		 */
		fputs_nl(gen_frame("defines", makepath(NULL, "defines", normal_suffix)), op);
		fputs_nl(gen_frame("files", makepath(NULL, "files", normal_suffix)), op);
		fputs_nl(gen_frameset_end(), op);
		fputs_nl(gen_frame("mains", makepath(NULL, "mains", normal_suffix)), op);
		fputs_nl(noframes_begin, op);
		fputs_nl(body_begin, op);
		fputs(index, op);
		fputs_nl(body_end, op);
		fputs_nl(noframes_end, op);
		fputs_nl(gen_frameset_end(), op);
		fputs_nl(gen_page_end(), op);
	} else {
		fputs_nl(gen_page_index_begin(title, jscode), op);
		fputs_nl(body_begin, op);
		if (insert_header)
			fputs(gen_insert_header(TOPDIR), op);
		fputs(index, op);
		if (insert_footer)
			fputs(gen_insert_footer(TOPDIR), op);
		fputs_nl(body_end, op);
		fputs_nl(gen_page_end(), op);
	}
	fclose(op);
	html_count++;
}
/**
 * makemainindex: make main index
 *
 *	@param[in]	file	file name
 *	@param[in]	index	common part
 */
static void
makemainindex(const char *file, const char *index)
{
	FILE *op;

	op = fopen(makepath(distpath, file, NULL), "w");
	if (!op)
		die("cannot make file '%s'.", file);
	fputs_nl(gen_page_index_begin(title, jscode), op);
	fputs_nl(body_begin, op);
	if (insert_header)
		fputs(gen_insert_header(TOPDIR), op);
	fputs(index, op);
	if (insert_footer)
		fputs(gen_insert_footer(TOPDIR), op);
	fputs_nl(body_end, op);
	fputs_nl(gen_page_end(), op);
	fclose(op);
	html_count++;
}
/**
 * makesearchindex: make search html
 *
 *	@param[in]	file	file name
 */
static void
makesearchindex(const char *file)
{
	FILE *op;

	op = fopen(makepath(distpath, file, NULL), "w");
	if (!op)
		die("cannot create file '%s'.", file);
	fputs_nl(gen_page_index_begin("SEARCH", jscode), op);
	fputs_nl(body_begin, op);
	fputs(makesearchpart("mains"), op);
	fputs_nl(body_end, op);
	fputs_nl(gen_page_end(), op);
	fclose(op);
	html_count++;
}
/**
 * makehtaccess: make @FILE{.htaccess} skeleton file.
 */
static void
makehtaccess(const char *cgidir, const char *file)
{
	FILE *op;

	op = fopen(makepath(distpath, file, NULL), "w");
	if (!op)
		die("cannot make .htaccess skeleton file.");
	fputs_nl("#", op);
	fputs_nl("# Skeleton file for .htaccess -- This file was generated by htags(1).", op);
	fputs_nl("#", op);
	fputs_nl("# To make this file effective, undermentioned description is necessary", op);
	fputs_nl("# in your system's configuration file.", op);
	fputs_nl("#", op);
	fputs_nl("# [/usr/local/apache/conf/http.conf]", op);
	fputs_nl("# +-------------------------------------", op);
	fputs_nl("# |...", op);
	fputs_nl("# |AllowOverride Options FileInfo", op);
	fputs_nl("#", op);
	fputs_nl("# Htags was invoked with the -f, -c or -D option.", op);
	fprintf(op, "# You should start http server so that %s/*.cgi is executed\n", cgidir);
	fputs_nl("# as a CGI script.", op);
	fputs_nl("#", op);
	fputs_nl("Options +ExecCGI", op);
	fputs_nl("AddHandler cgi-script .cgi", op);
	if (cflag) {
		fputs_nl("#", op);
		fputs_nl("# Htags have made gzipped html files because you specified the -c option.", op);
		fputs_nl("# If your browser doesn't decompress gzipped files, you should start", op);
		fputs_nl("# http server so that they are decompressed.", op);
		fputs_nl("#", op);
		fputs_nl("# Please rewrite appropriately the string '/cgi-bin/ghtml.cgi' below, or", op);
		fputs_nl("# copy the file 'cgi-bin/ghtml.cgi' itself to the system's CGI directory.", op);
		fputs_nl("#", op);
		fprintf(op, "AddHandler htags-gzipped-html %s\n", gzipped_suffix);
		fputs_nl("Action htags-gzipped-html /cgi-bin/ghtml.cgi", op);
		fputs_nl("#                         ==================", op);
	}
	fclose(op);
}
/**
 * makehtml: make html files
 *
 *	@param[in]	total	number of files.
 */
static void
makehtml(int total)
{
	GFIND *gp;
	FILE *anchor_stream;
	const char *path;
	int count = 0;

	/*
	 * Create anchor stream for anchor_load().
	 */
	anchor_stream = tmpfile();
#if defined(_WIN32) && !defined(__CYGWIN__)
	/*
	 * tmpfile is created in the root, which user's can't write on Vista+.
	 * Use _tempnam and open it directly.
	 */
	if (anchor_stream == NULL) {
		char *name = _tempnam(tmpdir, "htags");
		anchor_stream = fopen(name, "w+bD");
		free(name);
	}
#endif
	gp = gfind_open(dbpath, NULL, other_files ? GPATH_BOTH : GPATH_SOURCE);
	while ((path = gfind_read(gp)) != NULL) {
		if (gp->type == GPATH_OTHER)
			fputc(' ', anchor_stream);
		fputs(path, anchor_stream);
		fputc('\n', anchor_stream);
	}
	gfind_close(gp);
	/*
	 * Prepare anchor stream for anchor_load().
	 */
	anchor_prepare(anchor_stream);
	/*
	 * For each path in GPATH, convert the path into HTML file.
	 */
	gp = gfind_open(dbpath, NULL, other_files ? GPATH_BOTH : GPATH_SOURCE);
	while ((path = gfind_read(gp)) != NULL) {
		char html[MAXPATHLEN];

		if (gp->type == GPATH_OTHER && !other_files)
			continue;
		/*
		 * load tags belonging to the path.
		 * The path must be start "./".
		 */
		anchor_load(path);
		/*
		 * inform the current path name to lex() function.
		 */
		save_current_path(path);
		count++;
		path += 2;		/* remove './' at the head */
		message(" [%d/%d] converting %s", count, total, path);
		snprintf(html, sizeof(html), "%s/%s/%s.%s", distpath, SRCS, path2fid(path), HTML);
		src2html(path, html, gp->type == GPATH_OTHER);
	}
	gfind_close(gp);
}
/**
 * Load file.
 */
void
loadfile_asis(const char *file, STRBUF *result)
{
	STRBUF *sb = strbuf_open(0);
	FILE *ip = fopen(file, "r");
	if (!ip)
		die("file '%s' not found.", file);
	while (strbuf_fgets(sb, ip, STRBUF_NOCRLF) != NULL)
		strbuf_puts_nl(result, strbuf_value(sb));
	fclose(ip);
	strbuf_close(sb);
}
void
loadfile(const char *file, STRBUF *result)
{
	load_with_replace(file, result, 0);
}
/**
 * makecommonpart: make a common part for @FILE{mains.html} and @FILE{index.html}
 *
 *	@param[in]	title
 *	@param[in]	defines
 *	@param[in]	files
 *	@return	index	common part
 */
static char *
makecommonpart(const char *title, const char *defines, const char *files)
{
	FILE *ip;
	STRBUF *sb = strbuf_open(0);
	STRBUF *ib = strbuf_open(0);
	char buf[MAXFILLEN];
	const char *tips = "Go to the GLOBAL project page.";
	const char *_, *item;

	strbuf_puts(sb, title_begin);
	strbuf_puts(sb, title);
	strbuf_puts_nl(sb, title_end);
	strbuf_puts_nl(sb, poweredby_begin);
	strbuf_sprintf(sb, "Last updated %s%s\n", now(), br);
	if (Iflag) {
		snprintf(buf, sizeof(buf), "Powered by GLOBAL-%s.", get_version());
		strbuf_puts(sb, gen_href_begin_with_title_target(NULL, www, NULL, NULL, tips,"_top"));
		strbuf_puts(sb, gen_image(CURRENT, "pglobe", buf));
		strbuf_puts(sb, gen_href_end());
		strbuf_puts(sb, br);
	} else {
		strbuf_sprintf(sb, "Powered by %sGLOBAL-%s%s.%s\n",
			gen_href_begin_with_title_target(NULL, www, NULL, NULL, tips, "_top"),
			get_version(),
			gen_href_end(),
			br);
	}
	strbuf_puts_nl(sb, poweredby_end);
	strbuf_puts_nl(sb, hr);
	/*
	 * Print items according to the value of variable 'item_order'.
	 */
	for (item = item_order; *item; item++) {
		switch (*item) {
		case 'c':
			if (caution) {
				strbuf_puts_nl(sb, caution_begin);
				strbuf_sprintf(sb, "<font size='+2' color='red'>CAUTION</font>%s\n", br);
				strbuf_sprintf(sb, "This hypertext consists of %d files.\n", html_count);
				strbuf_puts_nl(sb, "Please don't download the whole hypertext using a hypertext copy tool.");
				strbuf_puts_nl(sb, "Our network cannot afford such traffic.");
				strbuf_puts_nl(sb, "Instead, you can generate the same thing in your computer using");
				strbuf_puts(sb, gen_href_begin_with_title_target(NULL, www, NULL, NULL, NULL, "_top"));
				strbuf_puts(sb, "GLOBAL source code tag system");
				strbuf_puts_nl(sb, gen_href_end());
				strbuf_puts_nl(sb, "Thank you.");
				strbuf_puts_nl(sb, caution_end);
				strbuf_sprintf(sb, "\n%s\n", hr);
			}
			break;
		case 's':
			if (fflag) {
				strbuf_puts(sb, makesearchpart(NULL));
				strbuf_puts_nl(sb, hr);
			}
			break;
		case 't':
			if (call_file || callee_file) {
				strbuf_puts(sb, header_begin);
				if (call_file) {
					strbuf_puts(sb, gen_href_begin(NULL, "call", normal_suffix, NULL));
					strbuf_puts(sb, title_call_tree);
					strbuf_puts(sb, gen_href_end());
				}
				if (call_file && callee_file)
					strbuf_puts(sb, " / ");
				if (callee_file) {
					strbuf_puts(sb, gen_href_begin(NULL, "callee", normal_suffix, NULL));
					strbuf_puts(sb, title_callee_tree);
					strbuf_puts(sb, gen_href_end());
				}
				strbuf_puts_nl(sb, header_end);
				strbuf_puts_nl(sb, hr);
			}
			break;
		case 'm':
			strbuf_sprintf(sb, "%sMAINS%s\n", header_begin, header_end);

			snprintf(buf, sizeof(buf), PQUOTE "%s --result=ctags-xid --encode-path=\" \t\" --nofilter=path %s" PQUOTE, quote_shell(global_path), main_func);
			ip = popen(buf, "r");
			if (!ip)
				die("cannot execute '%s'.", buf);
			strbuf_puts_nl(sb, gen_list_begin());
			while ((_ = strbuf_fgets(ib, ip, STRBUF_NOCRLF)) != NULL) {
				char fid[MAXFIDLEN];
				const char *ctags_x = parse_xid(_, fid, NULL);

				strbuf_puts_nl(sb, gen_list_body(SRCS, ctags_x, fid));
			}
			strbuf_puts_nl(sb, gen_list_end());
			if (pclose(ip) != 0)
				die("terminated abnormally '%s' (errno = %d).", buf, errno);
			strbuf_puts_nl(sb, hr);
			break;
		case 'd':
			if (aflag && !Fflag) {
				strbuf_puts(sb, header_begin);
				strbuf_puts(sb, title_define_index);
				strbuf_puts_nl(sb, header_end);
				strbuf_puts(sb, defines);
			} else {
				strbuf_puts(sb, header_begin);
				strbuf_puts(sb, gen_href_begin(NULL, "defines", normal_suffix, NULL));
				strbuf_puts(sb, title_define_index);
				strbuf_puts(sb, gen_href_end());
				strbuf_puts_nl(sb, header_end);
			}
			strbuf_puts_nl(sb, hr);
			break;
		case 'f':
			if (Fflag) {
				strbuf_puts(sb, header_begin);
				strbuf_puts(sb, gen_href_begin(NULL, "files", normal_suffix, NULL));
				strbuf_puts(sb, title_file_index);
				strbuf_puts(sb, gen_href_end());
				strbuf_puts_nl(sb, header_end);
			} else {
				strbuf_puts(sb, header_begin);
				strbuf_puts(sb, title_file_index);
				strbuf_puts_nl(sb, header_end);
				if (tree_view) {
					strbuf_puts_nl(sb, tree_control);
					strbuf_puts_nl(sb, tree_loading);
					if (tree_view_type) {
						strbuf_sprintf(sb, tree_begin_using, tree_view_type);
						strbuf_putc(sb, '\n');
					} else {
						strbuf_puts_nl(sb, tree_begin);
					}
				} else if (table_flist)
					strbuf_puts_nl(sb, flist_begin);
				else if (!no_order_list)
					strbuf_puts_nl(sb, list_begin);
				strbuf_puts(sb, files);
				if (tree_view)
					strbuf_puts_nl(sb, tree_end);
				else if (table_flist)
					strbuf_puts_nl(sb, flist_end);
				else if (!no_order_list)
					strbuf_puts_nl(sb, list_end);
				else
					strbuf_puts_nl(sb, br);
			}
			strbuf_puts_nl(sb, hr);
			break;
		default:
			warning("unknown item '%c'. (Ignored)", *item);
			break;
		}
	}
	strbuf_close(ib);

	return strbuf_value(sb);
	/* doesn't close string buffer */
}
/**
 * basic check.
 */
static void
basic_check(void)
{
	const char *p;

	/*
	 * COMMAND EXISTENCE CHECK
	 */
	if (!(p = usable("gtags")))
		die("gtags command required but not found.");
	strlimcpy(gtags_path, p, sizeof(gtags_path));
	if (!(p = usable("global")))
		die("global command required but not found.");
	strlimcpy(global_path, p, sizeof(global_path));
	/*
	 * Temporary directory.
	 */
	if ((p = getenv("TMPDIR")) == NULL)
		p = getenv("TMP");
	if (p != NULL && test("d", p))
		tmpdir = p;
}
/**
 * load configuration variables.
 */
static void
configuration()
{
	STRBUF *sb = strbuf_open(0);
	char *p, *q;
	int n;

	/*
	 * Config variables.
	 */
	strbuf_reset(sb);
	if (!getconfs("datadir", sb))
		die("cannot get datadir directory name.");
	strlimcpy(datadir, strbuf_value(sb), sizeof(datadir));
	strbuf_reset(sb);
	if (!getconfs("localstatedir", sb))
		die("cannot get localstatedir directory name.");
	strlimcpy(localstatedir, strbuf_value(sb), sizeof(localstatedir));
	if (getconfn("ncol", &n)) {
		if (n < 1 || n > 10)
			warning("parameter 'ncol' ignored because the value (=%d) is too large or too small.", n);
		else
			ncol = n;
	}
	if (getconfn("tabs", &n)) {
		if (n < 1 || n > 32)
			warning("parameter 'tabs' ignored because the value (=%d) is too large or too small.", n);
		else
			tabs = n;
	}
	strbuf_reset(sb);
	if (getconfs("gzipped_suffix", sb))
		gzipped_suffix = check_strdup(strbuf_value(sb));
	strbuf_reset(sb);
	if (getconfs("normal_suffix", sb))
		normal_suffix = check_strdup(strbuf_value(sb));
	if (getconfb("no_order_list"))
		no_order_list = 1;
	strbuf_reset(sb);
	if (getconfs("prolog_script", sb))
		prolog_script = check_strdup(strbuf_value(sb));
	strbuf_reset(sb);
	if (getconfs("epilog_script", sb))
		epilog_script = check_strdup(strbuf_value(sb));
	if (getconfb("colorize_warned_line"))
		colorize_warned_line = 1;
	strbuf_reset(sb);
	if (getconfs("script_alias", sb)) {
		p = check_strdup(strbuf_value(sb));
		/* remove the last '/' */
		q = p + strlen(p) - 1;
		if (*q == '/')
			*q = '\0';
		script_alias = p;
	}
	strbuf_reset(sb);
	if (getconfs("include_file_suffixes", sb))
		include_file_suffixes = check_strdup(strbuf_value(sb));
	strbuf_reset(sb);
	if (getconfs("langmap", sb))
		langmap = check_strdup(strbuf_value(sb));
	strbuf_reset(sb);
	if (getconfs("xhtml_version", sb))
		xhtml_version = check_strdup(strbuf_value(sb));
	strbuf_close(sb);
}
/**
 * save_environment: save configuration data and arguments.
 */
static void
save_environment(int argc, char *const *argv)
{
	char command[MAXFILLEN];
	STRBUF *sb = strbuf_open(0);
	STRBUF *save_c = strbuf_open(0);
	STRBUF *save_a = strbuf_open(0);
	int i;
	const char *p;
	FILE *ip;

	/*
	 * save config values.
	 */
	snprintf(command, sizeof(command), PQUOTE "%s --config" PQUOTE, quote_shell(gtags_path));
	if ((ip = popen(command, "r")) == NULL)
		die("cannot execute '%s'.", command);
	while (strbuf_fgets(sb, ip, STRBUF_NOCRLF) != NULL) {
		for (p = strbuf_value(sb); *p; p++) {
			if (*p == '\'') {
				strbuf_putc(save_c, '\'');
				strbuf_putc(save_c, '"');
				strbuf_putc(save_c, '\'');
				strbuf_putc(save_c, '"');
				strbuf_putc(save_c, '\'');
			} else
				strbuf_putc(save_c, *p);
		}
	}
	if (pclose(ip) != 0)
		die("terminated abnormally '%s' (errno = %d).", command, errno);
	strbuf_close(sb);
	save_config = strbuf_value(save_c);
	/* doesn't close string buffer for save config. */
	/* strbuf_close(save_c); */

	/*
	 * save arguments.
	 */
	{
		char *opt_gtagsconf = "--gtagsconf";

		for (i = 1; i < argc; i++) {
			char *blank;

			/*
			 * skip --gtagsconf because it is already read
			 * as config value.
			 */
			if ((p = locatestring(argv[i], opt_gtagsconf, MATCH_AT_FIRST))) {
				if (*p == '\0')
					i++;
				continue;
			}
			blank = locatestring(argv[i], " ", MATCH_FIRST);
			strbuf_putc(save_a, ' ');
			if (blank)
				strbuf_putc(save_a, '\'');
			strbuf_puts(save_a, argv[i]);
			if (blank)
				strbuf_putc(save_a, '\'');
		}
	}
	save_argv = strbuf_value(save_a);
	/* doesn't close string buffer for save arguments. */
	/* strbuf_close(save_a); */
}

int
main(int argc, char **argv)
{
	const char *av = NULL;
	int func_total, file_total;
        char arg_dbpath[MAXPATHLEN];
	const char *index = NULL;
	int optchar;
        int option_index = 0;
	STATISTICS_TIME *tim;

	arg_dbpath[0] = 0;
	basic_check();
	/*
	 * Setup GTAGSCONF and GTAGSLABEL environment variable
	 * according to the --gtagsconf and --gtagslabel option.
	 */
	preparse_options(argc, argv);
	/*
	 * Load configuration values.
	 */
	if (!vgetcwd(cwdpath, sizeof(cwdpath)))
		die("cannot get current directory.");
	openconf(cwdpath);
	configuration();
	/*
	 * Setup langmap
	 */
	setup_langmap(langmap);
	save_environment(argc, argv);
	/*
	 * insert htags_options at the head of argv.
	 */
	setenv_from_config();
	{
		char *env = getenv("HTAGS_OPTIONS");
		if (env && *env)
			argv = prepend_options(&argc, argv, env);
	}
	while ((optchar = getopt_long(argc, argv, "acd:DfFghIm:nNoqst:Tvwx", long_options, &option_index)) != EOF) {
		switch (optchar) {
		case 0:
			/* already flags set */
			break;
		case OPT_AUTO_COMPLETION:
			auto_completion = 1;
			if (optarg) {
				if (atoi(optarg) > 0)
					auto_completion_limit = optarg;
				else
					die("The option value of --auto-completion must be numeric.");
			}
			break;
		case OPT_CFLOW:
			call_file = optarg;
			break;
		case OPT_CALL_TREE:
			call_file = optarg;
			break;
		case OPT_CALLEE_TREE:
			callee_file = optarg;
			break;
		case OPT_CVSWEB:
			cvsweb_url = optarg;
			break;
		case OPT_CVSWEB_CVSROOT:
			cvsweb_cvsroot = optarg;
			break;
		case OPT_GTAGSCONF:
		case OPT_GTAGSLABEL:
			/* These options are already parsed in preparse_options() */
			break;
		case OPT_INSERT_FOOTER:
			insert_footer = optarg;
			break;
		case OPT_INSERT_HEADER:
			insert_header = optarg;
			break;
		case OPT_HTML_HEADER:
			{
				STATIC_STRBUF(sb);
				if (!test("r", optarg))
					die("file '%s' not found.", optarg);
				strbuf_clear(sb);
				loadfile_asis(optarg, sb);
				html_header = strbuf_value(sb);
			}
			break;
		case OPT_ITEM_ORDER:
			item_order = optarg;
			break;
		case OPT_TABS:
			if (atoi(optarg) > 0)
				tabs = atoi(optarg);
			else
				die("--tabs option requires numeric value.");
                        break;
		case OPT_NCOL:
			if (atoi(optarg) > 0)
				ncol = atoi(optarg);
			else
				die("--ncol option requires numeric value.");
                        break;
		case OPT_TREE_VIEW:
			tree_view = 1;
			if (optarg)
				tree_view_type = optarg;
			break;
                case 'a':
                        aflag++;
                        break;
                case 'c':
                        cflag++;
                        break;
                case 'd':
			strlimcpy(arg_dbpath, optarg, sizeof(arg_dbpath));
                        break;
                case 'D':
			dynamic = 1;
                        break;
                case 'f':
                        fflag++;
                        break;
                case 'F':
                        Fflag++;
                        break;
                case 'g':
                        gflag++;
                        break;
                case 'h':
			definition_header = AFTER_HEADER;
			if (optarg) {
				if (!strcmp(optarg, "before"))
					definition_header = BEFORE_HEADER;
				else if (!strcmp(optarg, "right"))
					definition_header = RIGHT_HEADER;
				else if (!strcmp(optarg, "after"))
					definition_header = AFTER_HEADER;
				else
					die("The option value of --func-header must be one of 'before', 'right' and 'after'.");
			}
                        break;
                case 'I':
                        Iflag++;
                        break;
                case 'm':
			main_func = optarg;
                        break;
                case 'n':
                        nflag++;
			if (optarg) {
				if (atoi(optarg) > 0)
					ncol = atoi(optarg);
				else
					die("The option value of --line-number must be numeric.");
			}
                        break;
                case 'o':
			other_files = 1;
                        break;
                case 's':
			symbol = 1;
                        break;
                case 'S':
			Sflag++;
			sitekey = optarg;
                        break;
                case 'T':
			table_flist = 1;
			if (optarg) {
				if (atoi(optarg) > 0)
					flist_fields = atoi(optarg);
				else
					die("The option value of the --table-flist must be numeric.");
			}
                        break;
                case 't':
			title = optarg;
                        break;
                case 'q':
                        qflag++;
			setquiet();
                        break;
                case 'v':
                        vflag++;
			setverbose();
                        break;
                case 'w':
                        wflag++;
                        break;
		case 'x':
			enable_xhtml = 1;
			if (optarg) {
				if (!strcmp("1.0", optarg) || !strcmp("1.1", optarg))
					xhtml_version = optarg;
				else
					die("The option value of the --xhtml must be '1.0' or '1.1'.");
			}
                        break;
                default:
                        usage();
                        break;
		}
	}
	/*
	 * Leaving everything to htags.
	 * Htags selects popular options for you.
	 */
	if (suggest2)
		suggest = 1;
	if (suggest) {
		int gtags_not_found = 0;
		char dbpath[MAXPATHLEN];

		aflag = Iflag = nflag = vflag = 1;
		setverbose();
		definition_header = AFTER_HEADER;
		other_files = symbol = show_position = table_flist = fixed_guide = 1;
		if (arg_dbpath[0]) {
			if (!test("f", makepath(arg_dbpath, dbname(GTAGS), NULL)))
				gtags_not_found = 1;
		} else if (gtagsexist(".", dbpath, sizeof(dbpath), 0) == 0) {
			gtags_not_found = 1;
		}
		if (gtags_not_found)
			gflag = 1;
	}
	if (suggest2) {
		Fflag = 1;				/* uses frame */
		fflag = dynamic = 1;			/* needs a HTTP server */
		auto_completion = tree_view = 1;	/* needs javascript */
	}
	if (strchr(sitekey, '.') || strchr(sitekey, '/'))
		die("site key must not include '.' and '/'.");
	if (!enable_xhtml && (tree_view || auto_completion || fixed_guide))
		die("The --html option cannot accept the --tree-view, --auto-completion and --fixed-guide option.");
	if (call_file && !test("fr", call_file))
		die("cflow file not found. '%s'", call_file);
	if (callee_file && !test("fr", callee_file))
		die("cflow file not found. '%s'", callee_file);
	if (insert_header && !test("fr", insert_header))
		die("page header file '%s' not found.", insert_header);
	if (insert_footer && !test("fr", insert_footer))
		die("page footer file '%s' not found.", insert_footer);
	if (!fflag)
		auto_completion = 0;
        argc -= optind;
        argv += optind;
        if (!av)
                av = (argc > 0) ? *argv : NULL;

	if (debug)
		setdebug();
	settabs(tabs);					/* setup tab skip */
        if (qflag) {
                setquiet();
		vflag = 0;
	}
	if (!cflag && !fflag && !dynamic)
		Sflag = 0;
	if (enable_xhtml)
		setup_xhtml();
        if (show_version)
                version(av, vflag);
        if (show_help)
                help();
	/*
	 * Invokes gtags beforehand.
	 */
	if (gflag) {
		STRBUF *sb = strbuf_open(0);

		strbuf_puts(sb, gtags_path);
		if (vflag)
			strbuf_puts(sb, " -v");
		if (wflag)
			strbuf_puts(sb, " -w");
		if (suggest2 && enable_idutils && usable("mkid"))
			strbuf_puts(sb, " -I");
		if (arg_dbpath[0]) {
			strbuf_putc(sb, ' ');
			strbuf_puts(sb, arg_dbpath);
		}
		if (system(strbuf_value(sb)))
			die("cannot execute gtags(1) command.");
		strbuf_close(sb);
	}
	/*
	 * get dbpath.
	 */
	if (arg_dbpath[0]) {
		strlimcpy(dbpath, arg_dbpath, sizeof(dbpath));
	} else {
		int status = setupdbpath(0);
		if (status < 0)
			die_with_code(-status, gtags_dbpath_error);
		strlimcpy(dbpath, get_dbpath(), sizeof(dbpath));
	}
	if (cflag && !usable("gzip")) {
		warning("'gzip' command not found. -c option ignored.");
		cflag = 0;
	}
	if (!title) {
		char *p = strrchr(cwdpath, sep);
		title = p ? p + 1 : cwdpath;
	}
	if (cvsweb_url && test("d", "CVS"))
		use_cvs_module = 1;
	/*
	 * decide directory in which we make hypertext.
	 */
	if (av) {
		char realpath[MAXPATHLEN];

		if (!test("dw", av))
			die("'%s' is not writable directory.", av);
		if (chdir(av) < 0)
			die("directory '%s' not found.", av);
		if (!vgetcwd(realpath, sizeof(realpath)))
			die("cannot get current directory");
		if (chdir(cwdpath) < 0)
			die("cannot return to original directory.");
		snprintf(distpath, sizeof(distpath), "%s/HTML", realpath);
	} else {
		snprintf(distpath, sizeof(distpath), "%s/HTML", cwdpath);
	}
	/*
	 * Site key management for center CGI.
	 */
	if (Sflag) {
		STRBUF *sb = strbuf_open(0);
		static char saction[MAXBUFLEN];
		static char completion_saction[MAXBUFLEN];
		char path[MAXBUFLEN];
		int try_writing = 0;

		snprintf(saction, sizeof(saction), "%s/global.cgi", script_alias);
		action = saction;
		snprintf(completion_saction, sizeof(completion_saction), "%s/completion.cgi", script_alias);
		completion_action = completion_saction;
		snprintf(path, sizeof(path), "%s/gtags/%s", localstatedir, SITEKEYDIRNAME);
		if (!test("d", path)) {
			setverbose();
			message("htags: directory '%s' not found.", path);
			message("\n[Information]\n");
			message("It seems that GLOBAL is not install correctly. Please reinstall.");
			exit(0);
		}
		/*
		 * Load the sitekey if it exists.
		 */
		snprintf(path, sizeof(path), "%s/gtags/%s/%s", localstatedir, SITEKEYDIRNAME, sitekey);
		if (test("f", path)) {
			FILE *ip = fopen(path, "r");

			if (ip == NULL)
				die("cannot open file '%s'.", path);
			strbuf_fgets(sb, ip, STRBUF_NOCRLF);
			fclose(ip);
		}
		if (!test("f", path))
			try_writing = 1;
		else if (!strcmp(distpath, strbuf_value(sb)))
			; /* nothing to do */
		else if (overwrite_key == 0)
			die("The site key '%s' is not unique, please change it or use --overwrite-key option.", sitekey);
		else	/* New key value without --overwrite-key option */
			try_writing = 1;
		/*
		 * In almost case, the following procedures are done
		 * using bless.sh script by the root user.
		 */
		if (try_writing) {
			FILE *op = fopen(path, "w");
			
			if (op == NULL)
				need_bless = 1;
			else {
				fprintf(op, "%s\n", distpath);
				fclose(op);
				if (chmod(path, 0644) < 0)
					die("cannot chmod file '%s'(errno = %d).", path, errno);
			}
		}
		strbuf_close(sb);
	}
	/*
	 * Existence check of tag files.
	 */
	{
		int i;
		const char *path;
		GTOP *gtop;

		for (i = GPATH; i < GTAGLIM; i++) {
			path = makepath(dbpath, dbname(i), NULL);
			gtags_exist[i] = test("fr", path);
		}
		/*
		 * Real GRTAGS includes virtual GSYMS.
		 */
		gtags_exist[GSYMS] = symbol ? 1 : 0;
		if (!gtags_exist[GPATH] || !gtags_exist[GTAGS] || !gtags_exist[GRTAGS])
			die("GPATH, GTAGS and/or GRTAGS not found. Please reexecute htags with the -g option.");
		/*
		 * version check.
		 * Do nothing, but the version of tag file will be checked.
		 */
		gtop = gtags_open(dbpath, cwdpath, GTAGS, GTAGS_READ, 0);
		gtags_close(gtop);
		/*
		 * Check whether GRTAGS is empty.
		 */
		gtop = gtags_open(dbpath, cwdpath, GRTAGS, GTAGS_READ, 0);
		if (gtags_first(gtop, NULL, 0) == NULL)
			grtags_is_empty = 1;
		gtags_close(gtop);
	}
	/*
	 * make dbpath absolute.
	 */
	{
		char buf[MAXPATHLEN];
		if (realpath(dbpath, buf) == NULL)
			die("cannot get realpath of dbpath.");
		strlimcpy(dbpath, buf, sizeof(dbpath));
	}
	/*
	 * The older version (4.8.7 or former) of GPATH doesn't have files
         * other than source file. The oflag requires new version of GPATH.
	 */
	if (other_files) {
		GFIND *gp = gfind_open(dbpath, NULL, 0);
		if (gp->version < 2)
			die("GPATH is old format. Please remake it by invoking gtags(1).");
		gfind_close(gp);
	}
	/*
	 * for global(1) and gtags(1).
	 */
	set_env("GTAGSROOT", cwdpath);
	set_env("GTAGSDBPATH", dbpath);
	set_env("GTAGSLIBPATH", "");
	/*------------------------------------------------------------------
	 * MAKE FILES
	 *------------------------------------------------------------------
	 *       HTML/cgi-bin/global.cgi ... CGI program (1)
	 *       HTML/cgi-bin/ghtml.cgi  ... unzip script (1)
	 *       HTML/.htaccess          ... skeleton of .htaccess (1)
	 *       HTML/help.html          ... help file (2)
	 *       HTML/R/                 ... references (3)
	 *       HTML/D/                 ... definitions (3)
	 *       HTML/search.html        ... search index (4)
	 *       HTML/defines.html       ... definitions index (5)
	 *       HTML/defines/           ... definitions index (5)
	 *       HTML/files/             ... file index (6)
	 *       HTML/index.html         ... index file (7)
	 *       HTML/mains.html         ... main index (8)
	 *       HTML/null.html          ... main null html (8)
	 *       HTML/S/                 ... source files (9)
	 *       HTML/I/                 ... include file index (9)
	 *       HTML/rebuild.sh         ... rebuild script (10)
	 *       HTML/style.css          ... style sheet (11)
	 *------------------------------------------------------------------
	 */
	/* for clean up */
	signal_setup();
	sethandler(clean);

        HTML = (cflag) ? gzipped_suffix : normal_suffix;

	message("[%s] Htags started", now());
	init_statistics();
	/*
	 * (#) check if GTAGS, GRTAGS is the latest.
	 */
	if (get_dbpath())
		message(" Using %s/GTAGS.", get_dbpath());
	if (grtags_is_empty)
		message(" GRTAGS is empty.");
	if (gpath_open(dbpath, 0) < 0)
		die("GPATH not found.");
	if (!w32) {
		/* UNDER CONSTRUCTION */
	}
	if (auto_completion || tree_view) {
		STATIC_STRBUF(sb);
		strbuf_clear(sb);
		strbuf_puts_nl(sb, "<script type='text/javascript' src='js/jquery.js'></script>");
		if (auto_completion)
			loadfile("jscode_suggest", sb);
		if (tree_view)
			loadfile("jscode_treeview", sb);
		jscode = strbuf_value(sb);
	}
	/*
	 * (0) make directories
	 */
	message("[%s] (0) making directories ...", now());
	if (!test("d", distpath))
		if (mkdir(distpath, 0777) < 0)
			die("cannot make directory '%s'.", distpath);
	make_directory_in_distpath("files");
	make_directory_in_distpath("defines");
	make_directory_in_distpath(SRCS);
	make_directory_in_distpath(INCS);
	make_directory_in_distpath(INCREFS);
	if (!dynamic) {
		make_directory_in_distpath(DEFS);
		make_directory_in_distpath(REFS);
		if (symbol)
			make_directory_in_distpath(SYMS);
	}
	if (fflag || cflag || dynamic)
		make_directory_in_distpath("cgi-bin");
	if (Iflag)
		make_directory_in_distpath("icons");
	if (auto_completion || tree_view)
		 make_directory_in_distpath("js");
	/*
	 * (1) make CGI program
	 */
	if (fflag || cflag || dynamic) {
		char cgidir[MAXPATHLEN];
		int perm;

		snprintf(cgidir, sizeof(cgidir), "%s/cgi-bin", distpath);
		message("[%s] (1) making CGI program ...", now());
		/*
		 * If the Sflag is specified, CGI script is invalidated.
		 */
		perm = Sflag ? 0644 : 0755;
		if (fflag || dynamic) {
			makeprogram(cgidir, "global.cgi");
			if (chmod(makepath(cgidir, "global.cgi", NULL), perm) < 0)
				die("cannot chmod CGI program.");
		}
		if (auto_completion) {
			makeprogram(cgidir, "completion.cgi");
			if (chmod(makepath(cgidir, "completion.cgi", NULL), perm) < 0)
				die("cannot chmod CGI program.");
		}
		if (cflag) {
			makeghtml(cgidir, "ghtml.cgi");
			if (chmod(makepath(cgidir, "ghtml.cgi", NULL), perm) < 0)
				die("cannot chmod unzip script.");
		}
		makehtaccess(cgidir, ".htaccess");
		if (chmod(makepath(distpath, ".htaccess", NULL), 0644) < 0)
			die("cannot chmod .htaccess skeleton.");
		/*
		 * Don't grant execute permission to bless script.
		 */
		if (Sflag) {
			makebless("bless.sh");
			if (chmod(makepath(distpath, "bless.sh", NULL), 0640) < 0)
				die("cannot chmod bless script.");
		}
	} else {
		message("[%s] (1) making CGI program ...(skipped)", now());
	}
	/*
	 * Save the suffix of compress format for the safe CGI script.
	 */
	{
		const char *path = makepath(distpath, "compress", NULL);
		FILE *op = fopen(path, "w");
		if (op == NULL)
			die("cannot make file '%s'.", path);
		if (cflag) {
			fputs(HTML, op);
			fputc('\n', op);
		}
		fclose(op);
	}
	if (av) {
		const char *path = makepath(distpath, "GTAGSROOT", NULL);
		FILE *op = fopen(path, "w");
		if (op == NULL)
			die("cannot make file '%s'.", path);
		fputs(cwdpath, op);
		fputc('\n', op);
		fclose(op);
	}
	/*
	 * (2) make help file
	 */
	message("[%s] (2) making help.html ...", now());
	makehelp("help.html");
	/*
	 * (#) load GPATH
	 */
	load_gpath(dbpath);

	/*
	 * (3) make function entries (D/ and R/)
	 *     MAKING TAG CACHE
	 */
	message("[%s] (3) making tag lists ...", now());
	cache_open();
	tim = statistics_time_start("Time of making tag lists");
	func_total = makedupindex();
	statistics_time_end(tim);
	message("Total %d functions.", func_total);
	/*
	 * (4) search index. (search.html)
	 */
	if (Fflag && fflag) {
		message("[%s] (4) making search index ...", now());
		makesearchindex("search.html");
	}
	{
		STRBUF *defines = strbuf_open(0);
		STRBUF *files = strbuf_open(0);

		/*
		 * (5) make definition index (defines.html and defines/)
		 *     PRODUCE @defines
		 */
		message("[%s] (5) making definition index ...", now());
		tim = statistics_time_start("Time of making definition index");
		func_total = makedefineindex("defines.html", func_total, defines);
		statistics_time_end(tim);
		message("Total %d functions.", func_total);
		/*
		 * (6) make file index (files.html and files/)
		 *     PRODUCE @files, %includes
		 */
		message("[%s] (6) making file index ...", now());
		init_inc();
		tim = statistics_time_start("Time of making file index");
		file_total = makefileindex("files.html", files);
		statistics_time_end(tim);
		message("Total %d files.", file_total);
		html_count += file_total;
		/*
		 * (7) make call tree using cflow(1)'s output (cflow.html)
		 */
		if (call_file || callee_file) {
			message("[%s] (7) making cflow index ...", now());
			tim = statistics_time_start("Time of making cflow index");
			if (call_file)
				if (makecflowindex("call.html", call_file) < 0)
					call_file = NULL;
			if (callee_file)
				if (makecflowindex("callee.html", callee_file) < 0)
					callee_file = NULL;
			statistics_time_end(tim);
		}
		/*
		 * [#] make include file index.
		 */
		message("[%s] (#) making include file index ...", now());
		tim = statistics_time_start("Time of making include file index");
		makeincludeindex();
		statistics_time_end(tim);
		/*
		 * [#] make a common part for mains.html and index.html
		 *     USING @defines @files
		 */
		message("[%s] (#) making a common part ...", now());
		index = makecommonpart(title, strbuf_value(defines), strbuf_value(files));

		strbuf_close(defines);
		strbuf_close(files);
	}
	/*
	 * (7)make index file (index.html)
	 */
	message("[%s] (7) making index file ...", now());
	makeindex("index.html", title, index);
	/*
	 * (8) make main index (mains.html)
	 */
	message("[%s] (8) making main index ...", now());
	makemainindex("mains.html", index);
	/*
	 * (9) make HTML files (SRCS/)
	 *     USING TAG CACHE, %includes and anchor database.
	 */
	message("[%s] (9) making hypertext from source code ...", now());
	tim = statistics_time_start("Time of making hypertext");
	makehtml(file_total);
	statistics_time_end(tim);
	/*
	 * (10) rebuild script. (rebuild.sh)
	 *
	 * Don't grant execute permission to rebuild script.
	 */
	makerebuild("rebuild.sh");
	if (chmod(makepath(distpath, "rebuild.sh", NULL), 0640) < 0)
		die("cannot chmod rebuild script.");
	/*
	 * (11) style sheet file (style.css)
	 */
	if (enable_xhtml) {
		char src[MAXPATHLEN];
		char dist[MAXPATHLEN];
#ifdef __DJGPP__
		const char *template = "";
		snprintf(src, sizeof(src), "%s/gtags/style.css.tmpl", datadir);
		if (test("f", src))
		       template = ".tmpl";
		snprintf(src, sizeof(src), "%s/gtags/style.css%s", datadir, template);
		snprintf(dist, sizeof(dist), "%s/style.css", distpath);
#else
		snprintf(src, sizeof(src), "%s/gtags/style.css.tmpl", datadir);
		snprintf(dist, sizeof(dist), "%s/style.css", distpath);
#endif
		copyfile(src, dist);
	}
	if (auto_completion || tree_view) {
		char src[MAXPATHLEN];
		char dist[MAXPATHLEN];

		snprintf(src, sizeof(src), "%s/gtags/jquery", datadir);
		snprintf(dist, sizeof(dist), "%s/js", distpath);
		copydirectory(src, dist);
		snprintf(src, sizeof(src), "%s/gtags/jquery/images", datadir);
		snprintf(dist, sizeof(dist), "%s/js/images", distpath);
		copydirectory(src, dist);
	}
	message("[%s] Done.", now());
	if (vflag && (cflag || fflag || dynamic || auto_completion)) {
		message("\n[Information]\n");
		message(" o Htags was invoked with the -f, -c, -D or --auto-completion option. You should");
		message("   start http server so that cgi-bin/*.cgi is executed as a CGI script.");
		if (cflag) {
			message(" o Htags was invoked with the -c option. You should start a http server to");
			message("   decompress *.%s files.", gzipped_suffix);
		}
		if (Sflag && need_bless) {
			char path[MAXBUFLEN];
			snprintf(path, sizeof(path), "%s/gtags/%s/%s", localstatedir, SITEKEYDIRNAME, sitekey);

			message(" o Though htags was invoked with the --system-cgi option with a unique key '%s',", sitekey);
			message("   you don't have the permission to create file '%s'.", path);
			message("   You should ask the root user to bless the hypertext by executing");
			message("   like follows:");
			message("       # cd HTML");
			message("       # sh bless.sh");
		}
 		message("\n If you are using Apache, 'HTML/.htaccess' might be helpful for you.\n");
		message(" Good luck!\n");
	}
	if (Iflag) {
		char src[MAXPATHLEN];
		char dist[MAXPATHLEN];

		snprintf(src, sizeof(src), "%s/gtags/icons", datadir);
		snprintf(dist, sizeof(dist), "%s/icons", distpath);
		copydirectory(src, dist);
	}
	gpath_close();
	/*
	 * Print statistics information.
	 */
	print_statistics(statistics);
	clean();
	return 0;
}
