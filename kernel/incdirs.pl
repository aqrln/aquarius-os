#!/usr/bin/perl

# incdirs.pl - get the include directories
# Copyright (c) 2012 Alex Orlenko

use strict;
use warnings;

my @all = `for header in \`find kernel -name '*.h'\`; do dirname \$header; done`;
my @dirs = ();

foreach my $dir (@all) {
	chomp $dir;
	push @dirs, $dir unless scalar grep {$_ eq $dir} @dirs;
}

print "-I $_ " foreach @dirs;
