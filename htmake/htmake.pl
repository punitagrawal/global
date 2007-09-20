#!/usr/bin/perl
#
# Copyright (c) 1999, 2000
#		Ron Lee <ron@debian.org> All rights reserved.
#
#  You may distribute this software under the terms of the GNU GPL version 2
#  or later.
#
#	htmake.pl					25-Mar-99
#
#	htmake is a wrapper that calls htags(1) with an --id attribute
#	and runs htconfig on the generated hypertext.
#
# OPERATION:	generate unique id.
#		call htags to make html source
#		call htconfig to update database.
#		must be called from src dir.
#
#	htmake [htags options] --action=cgi-action --url=base-url --tags=gtagsdbpath [destdir]
#
#	TODO: add --rebuild option, to recycle old id. ??
#

$com = $0;
$com =~ s/.*\///;
$usage = "usage: $com [-a][-c|-C][-h][-l][-n][-v][-d tagdir, --tags=tagdir][-t title]\n";
$usage .= "         [--action=cgi-url][--url=base-url][destdir]\n";

#-----------------------------------------------------------------
# DEFAULTS
#-----------------------------------------------------------------

$CONFIG = '/etc/gtags/htmake.conf';
$action = 'http://localhost/cgi-bin/gsearch.cgi';

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

&usable('htags') or &error("htags function not availiable.");
&usable('htconfig') or &error("htconfig function not availiable.");
-f $CONFIG and do $CONFIG or &error("Cannot read $CONFIG, see htconfig --install option first.");
&error("GSPATHDATA must be defined in $CONFIG") unless $GSPATHDATA;
$action = $ACTION if $ACTION;
$destdir = $url = $tags = '';
$htagsflags = 'f';
%longoption = (	action => '/cgi-bin/gsearch.cgi', url => '', tags => '', verbose => 1, );
while ($ARGV[0] =~ /^-/) {
	$opt = shift;
	if ($opt =~ /^--(\w+)=?([-+.:~\/\w]*)/) {
		&usage unless exists $longoption{$1};
		$$1 = $2 || $longoption{$1};
		next;
	}
	&usage if ($opt =~ /[^-acCdFlntv]/);
	if ($opt =~ /a/) { $htagsflags .= 'a'; }
	if ($opt =~ /c/) { $htagsflags .= 'c'; }
	if ($opt =~ /C/) { $htagsflags .= 'C'; }
	if ($opt =~ /F/) { $htagsflags .= 'F'; }
	if ($opt =~ /l/) { $htagsflags .= 'l'; }
	if ($opt =~ /n/) { $htagsflags .= 'n'; }
	if ($opt =~ /v/) { $verbose = 1; }
	if ($opt =~ /d/) {
		$opt = shift;
		last if ($opt eq '');
		$tags = $opt;
	} elsif ($opt =~ /t/) {
		$opt = shift;
		last if ($opt eq '');
		$title = $opt;
	}
}
$destdir = &realpath($ARGV[0]) if $ARGV[0];

# get unique id
#
&getcwd =~ /([^\/]*)$/ and $id = $1;
$id =~ s/[^\w]//g;
$id .= time;

# call htags
#
$htagsflags .= 'v' if $verbose;
@htagsopt = ("-$htagsflags");
push @htagsopt,('-t',"$title") if $title;
push @htagsopt,('-d',"$tags") if $tags;
print "Executing: htags @htagsopt --action=$action --id=$id --nocgi $destdir\n" if $verbose;

# htags is a C program now, so the number of arguments is important.
#
if ($destdir eq '') {
	system('htags',@htagsopt,"--action=$action","--id=$id","--nocgi") == 0 or
		&error("Failed to execute htags.");
} else {
	system('htags',@htagsopt,"--action=$action","--id=$id","--nocgi","$destdir") == 0 or
		&error("Failed to execute htags.");
}

# call htconfig
#
@htconfigopt =();
push @htconfigopt,('-v') if $verbose;
push @htconfigopt,("--url=$url") if $url;
push @htconfigopt,("--tags=$tags") if $tags;
print "Executing: htconfig @htconfigopt $destdir\n" if $verbose;
system('htconfig',@htconfigopt,"$destdir") == 0 or &error("Failed to execute htconfig.");

exit 0;
