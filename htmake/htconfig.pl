#!/usr/bin/perl
#
# Copyright (c) 1999
#		Ron Lee <ron@microtronics.com.au> All rights reserved.
#
#  Permission to use, copy, modify, distribute, and sell this software and
#  its documentation for any purpose is hereby granted without fee, provided
#  that the above copyright notice appear in all copies and that both that
#  copyright notice and this permission notice appear in supporting documen-
#  tation.  No representations are made about the suitability of this soft-
#  ware for any purpose.  It is provided "as is" without express or implied
#  warranty.  If it or anything else breaks, You get to keep all the pieces.
# 
#	It is adapted from, and intended for use with, Shigio Yamaguchi's
#	GLOBAL source code tagging and html generation suite.
#
#	htconfig.pl					13-Mar-99
#
#	This program is for manipulating the database read by gsearch.cgi,
#	an extended version of the global.cgi script.  It allows for a
#	single central script to search multiple hypertext source trees on
#	your system.
#
#	usage:
#	      htconfig -I [-z] [-a form-action] [-p pathdb] cgidir
#	      htconfig --install [--makeghtml] [--formaction=cgi] [--pathdb=pathsfile] cgidir
#
#	      htconfig -u base-url -d gtagsdbpath -s gtagsroot [dir]
#	      htconfig --url=base-url --tags=gtagsdbpath --src=gtagsroot [dir]
#
#	defaults: ( all are expanded to absolute paths )
#		-u	file:/dir
#		-d, -s	./
#		[dir]	./HTML
#
#	operation:
#		htconfig -I installs the cgi scripts and configuration
#		file.  This file must contain the location of the database
#		used to store path data created by htconfig and read by
#		gsearch.cgi - the -p option can set this at creation time.
#		-z installs ghtml.cgi as well as global.cgi.
#
#		otherwise htconfig appends an entry to the database file
#		based on the paths supplied.
#
#	extra flags:	-v --verbose
#
$com = $0;
$com =~ s/.*\///;
$usage = "usage: $com -I [-v][-z][-a form-action][-p pathdb] cgi-dir\n";
$usage .= "       $com --install [--makeghtml][--formaction=cgi-url][--pathdb=pathdatafile] cgi-dir\n\n";
$usage .= "       $com [-v][-u base-url][-d gtagsdbpath][-s gtagsroot][htmldir]\n";
$usage .= "       $com [--verbose][--url=base-url][--tags=gtagsdbpath][--src=gtagsroot][htmldir]\n";
#-----------------------------------------------------------------
# DEFAULTS
#-----------------------------------------------------------------

$CONFDIR = '/etc/gtags';
$CONFIG = $CONFDIR .'/htmake.conf';
chop($GHTML = `gtags --config gzipped_suffix`) if &usable('gtags');
$GHTML = $GHTML || 'ghtml';
$actiondir = $tags = $src = '.';

#----------------------------------------------------------------
# UTILITIES
#----------------------------------------------------------------

sub getcwd {
	local($dir) = `/bin/pwd`;
	chop($dir);
	$dir;
}
sub realpath {
	local($dir) = @_;
	local($cwd) = &getcwd;
	chdir($dir) || &error("directory '$dir' not found.");
	$dir = &getcwd;
	chdir($cwd) || &error("Cannot return to '$cwd'.");
	$dir;
}
sub usable {
	local($command) = @_;
	foreach (split(/:/, $ENV{'PATH'})) {
		return 1 if (-x "$_/$command");
	}
	return 0;
}
sub error {
        print STDERR "$com: $_[0]\n\n";
        exit 1;
}
sub usage {
	print STDERR $usage;
	exit 1;
}

#-----------------------------------------------------------------
# MAIN
#-----------------------------------------------------------------

$id = $install = $makeghtml = $formaction = $pathdb = $url = $verbose = '';
%longoption = (	install => 1,	makeghtml => 1,	pathdb => '',	formaction => '',
		verbose => 1,	url => '',	tags => '.',	src => '.',
);
while ($ARGV[0] =~ /^-/) {
	$opt = shift;
	if ($opt =~ /^--(\w+)=?([-+.:~\/\w]*)/) {
		&usage unless exists $longoption{$1};
		$$1 = $2 || $longoption{$1};
		next;
	}
	&usage if ($opt =~ /[^-Iadpsuvz]/);
	if ($opt =~ /I/) { $install = 1; }
	if ($opt =~ /v/) { $verbose = 1; }
	if ($opt =~ /z/) { $makeghtml = 1; }
	if ($opt =~ /a/) {
		$opt = shift;
		last if ($opt eq '');
		$formaction = $opt;
	} elsif ($opt =~ /p/) {
		$opt = shift;
		last if ($opt eq '');
		$pathdb = $opt;
	} elsif ($opt =~ /u/) {
		$opt = shift;
		last if ($opt eq '');
		$url = $opt;
	} elsif ($opt =~ /d/) {
		$opt = shift;
		last if ($opt eq '');
		$tags = $opt;
	} elsif ($opt =~ /s/) {
		$opt = shift;
		last if ($opt eq '');
		$src = $opt;
	}
}
$actiondir = &realpath($ARGV[0]) if $ARGV[0];

#-----------------------------------------------------------------
# INSTALL
#-----------------------------------------------------------------

if ($install) {
	&usage if ($actiondir eq '.');
	-w $actiondir or &error("$actiondir is not writable.");
	unless (-e $CONFIG) {
		mkdir($CONFDIR, 0755) || &error("Cannot create dir $CONFDIR") unless (-d $CONFDIR);
		local($gspathdata) = ($pathdb) ? "\n\$GSPATHDATA = '$pathdb';\n" : '';
		local($action) = ($formaction) ? "\n\$ACTION = '$formaction';" : '';
		&makeconfig($CONFIG,$gspathdata,$action) || &error("Cannot create $CONFIG");
		chmod(0644,$CONFIG) || &error("Cannot chmod $CONFIG");
		if ($verbose) {
			print "** $CONFIG created\n";
			print "   GSPATHDATA set to $pathdb\n" if $gspathdata;
			print "   ACTION set to $formaction\n" if $action;
		}
	}
	&makegsearch("$actiondir/gsearch.cgi") || &error("Cannot create $actiondir/gsearch.cgi");
	chmod(0755,"$actiondir/gsearch.cgi") || &error("Cannot chmod $actiondir/gsearch.cgi");
	if ($makeghtml) {
		&makeghtml("$actiondir/ghtml.cgi") || &error("Cannot create $actiondir/ghtml.cgi");
		chmod(0755,"$actiondir/ghtml.cgi") || &error("Cannot chmod $actiondir/ghtml.cgi");
	}
	if ($verbose) {
		print "** gsearch.cgi installed in $actiondir.\n";
		if ($makeghtml) {
			print "** ghtml.cgi installed in $actiondir.\n";
			print "  Your http server will need to be configured to use .ghtml files\n";
			print "  For apache, ensure mod_mime and mod_actions are loaded, and add:\n";
			print "        AddHandler htags-gzipped-html ghtml\n";
			print "        Action htags-gzipped-html /cgi-bin/ghtml.cgi\n";
			print "  to your httpd.conf or srm.conf files.\n";
		}
		do $CONFIG;
		print "\n** You must set \$GSPATHDATA in $CONFIG before using htmake or htconfig.\n\n"
			unless $GSPATHDATA;
	}
	exit 0;
}

#-----------------------------------------------------------------
# UPDATE
#-----------------------------------------------------------------

-f $CONFIG and do $CONFIG or &error("Cannot read $CONFIG, see htconfig --install option first.");
&error("GSPATHDATA must be defined in $CONFIG") unless $GSPATHDATA;
$tags = &realpath($tags);
-f "$tags/GTAGS" or &error("GTAGS file was not located at $tags");
$src = &realpath($src);
$actiondir .= '/HTML' if ($actiondir eq '.');
$url =~ s/\/$//;
$suffix = 'html';
if ($url && &usable('lynx')) {
	$page = `lynx -source $url/mains.html 2>&1`;
	$id = $1 if ($page =~ /NAME=id\sVALUE=(\w+)>/);
	$suffix = 'ghtml' if ($page =~ /ghtml/);
	$suffix = 'html.gz' if ($page =~ /html\.gz/);
} elsif (-e "$actiondir/mains.html") {
	$url = 'file:' . &realpath($actiondir) unless $url;
	open(FH,"<$actiondir/mains.html") or &error("Cannot open $actiondir/mains.html");
	while(<FH>) {
		$id = $1 if /NAME=id\sVALUE=(\w+)>/;
		$suffix = 'ghtml' if /ghtml/;
		$suffix = 'html.gz' if /html\.gz/;
	}
	close FH;
} else {
	&error("Unable to find/parse mains.html for uniqueid");
}
&error("Could not obtain uniqueid value.\n$url is not searchable unless generated with one.") unless $id;
unless (-f $GSPATHDATA) {
	$GSPATHDATA =~ s/[^-+~.\/\w]//g;
	chop($gspathdir = `dirname $GSPATHDATA`);
	system('mkdir','-p','-m','0755',"$gspathdir") == 0 or &error("Cannot create $gspathdir")
		unless (-d $gspathdir);
	system('touch',"$GSPATHDATA") == 0 or &error("Cannot create $GSPATHDATA");
	chmod(0644,"$GSPATHDATA") || &error("Cannot chmod $GSPATHDATA");
}
-w $GSPATHDATA or &error("Unable to update config, you do not have write permission for\n$GSPATHDATA");
$newrecord = <<END_RECORD;
if (\$form{'id'} eq '$id') {
	\$hpath='$url';
	\$srcpath='$src';
	\$dbpath='$tags';
	\$suffix='$suffix';
}
END_RECORD
if (open(RD, "<$GSPATHDATA")) {
	local ($/) = "}\n";
	open(WR, ">$GSPATHDATA~") or &error("Cannot open $GSPATHDATA~");
	while(<RD>) {
		if(/eq\s'$id'/o) {
			print WR $newrecord;
			$done=1;
		} elsif (!/^1;\n/) {
			print WR;
		}
	}
	print WR $newrecord unless $done;
	print WR "1;\n";
	close WR;
	close RD;
	rename ($GSPATHDATA, "$GSPATHDATA.bak") and rename ("$GSPATHDATA~", $GSPATHDATA)
		or &error("Could not replace $GSPATHDATA");
} else {
	&error("Cannot read $GSPATHDATA");
}
if ($verbose) {
	print "Path record " . (($done) ? "updated" : "created") . ".\n";
	print "  id          = $id\n";
	print "  url         = $url\n";	
	print "  source path = $src\n";
	print "  GTAGS path  = $tags\n";
	print "  html suffix = $suffix\n\n";
}

exit 0;

#----------------------------------------------------------------
# GENERATE SCRIPTS
#----------------------------------------------------------------

sub makeconfig {
	local ($file,$gspathdata,$action) = @_;
	open(CFG, ">$file") || &error("Cannot open $file.");
	$cfg = <<END_CONFIG;
# Configuration file for the htmake and htconfig extensions
# to GLOBAL.  Edit this file to suit your preferences.
#
# GSPATHDATA *must* be set or htconfig and gsearch.cgi cannot function.
# There is no default.

# For Debian GNU/Linux systems I would recommend:
# \$GSPATHDATA = '/var/lib/gsearch/pathdata';

# For *BSD systems you might prefer:
# \$GSPATHDATA = '/var/gsearch/pathdata';
$gspathdata
# ACTION sets the url that form data is sent to when searching GLOBAL
# hypertext.  You can override this at any time from the commandline
# or set your preferred default here.
#
# \$ACTION = 'http://localhost/cgi-bin/gsearch.cgi';
$action

# return true
1;
END_CONFIG

	print CFG $cfg;
	close CFG;
}
sub makegsearch {
	local($file) = @_;
	open(PROGRAM, ">$file") || &error("cannot make gsearch.cgi.");
	$program = "#!/usr/bin/perl\n\$CONFILE = '$CONFIG';\n";
	$program .= <<'END_OF_SCRIPT';
print "Content-type: text/html\n\n";
print "<HTML><BODY>";
@pairs = split (/&/, $ENV{'QUERY_STRING'});
foreach $p (@pairs) {
	($name, $value) = split(/=/, $p);
	$value =~ tr/+/ /;
	$value =~ s/%([\dA-Fa-f][\dA-Fa-f])/pack("C", hex($1))/eg;
	$form{$name} = $value;
}
if (!defined($form{'id'})) {
	print "<H3>No form id.  Please regenerate hypertext with htmake.</H3>\n";
	&finish();
}
-f "$CONFILE" and do "$CONFILE" or &alertmissing($CONFILE);
&alertmissing('db') unless (defined($GSPATHDATA));
-f "$GSPATHDATA" and do "$GSPATHDATA" or &alertmissing($GSPATHDATA);
&alertmissing('id') unless (defined($hpath));
# call sanity/security checks here before executing global/setting ENV
$ENV{'GTAGSROOT'} = $srcpath;
$ENV{'GTAGSDBPATH'} = $dbpath;
if ($form{'pattern'} eq '') {
	print "<H3>Pattern not specified. <A HREF=$hpath/mains.html>[return]</A></H3>\n";
	&finish();
}
$pattern = $form{'pattern'};
$flag = ($form{'type'} eq 'definition') ? '' : 'r';
$words = ($form{'type'} eq 'definition') ? 'definitions' : 'references';
print "<H1><FONT COLOR=#cc0000>\"$pattern\"</FONT></H1>\n";
print "Following $words are matched to above pattern.<HR>\n";
$pattern =~ s/'//g;			# to shut security hole
chdir("$srcpath");
unless (open(PIPE, "/usr/bin/global -x$flag '$pattern' |")) {
	print "<H3>Cannot execute global. <A HREF=$hpath/mains.html>[return]</A></H3>\n";
	&finish();
}
$cnt = 0;
print "<PRE>\n";
while (<PIPE>) {
	$cnt++;
	($tag, $lno, $filename) = split;
	chop($fileno = `/usr/bin/btreeop -K "./$filename" GPATH`);
	s/($tag)/<A HREF=$hpath\/S\/$fileno.$suffix#$lno>$1<\/A>/;
	print;
}
close(PIPE);
print "</PRE>\n";
if ($cnt == 0) {
	print "<H3>Pattern not found. <A HREF=$hpath/mains.html>[return]</A></H3>\n";
}
&finish();

sub alertmissing {
	if (@_ eq 'id') {
		print "<H3> No path data configured for this hypertext source</H3>\n";
		print " Please run htconfig on it to enable this search form<br>\n";
	} elsif (@_ eq 'db') {
		print "<H3> Path database location not defined.</H3>\n";
		print " Please set GSPATHDATA in $CONFILE<br>\n";
	} else {
		print "<H2> Unable to read @_</H2>\n";
		print "<p> Please ensure this file exists, and is readable by<br>\n";
		print " the user or group that your cgi scripts run as.\n";
		print "<p> UID: $< or GID: $(\n";
	}
	&finish();
}
sub finish {
	print "</BODY></HTML>\n";
	exit 0;
}
END_OF_SCRIPT

	print PROGRAM $program;
	close(PROGRAM);
}
sub makeghtml {
	local($file) = @_;
	open(PROGRAM, ">$file") || &error("cannot make unzip script.");
	$program = <<END_OF_SCRIPT;
#!/bin/sh
echo "content-type: text/html"
echo
gzip -S $GHTML -d -c "\$PATH_TRANSLATED"
END_OF_SCRIPT

	print PROGRAM $program;
	close(PROGRAM);
}
