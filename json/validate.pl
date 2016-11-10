#!/usr/bin/env perl 
#===============================================================================
#
#         FILE: validate.pl
#
#        USAGE: ./validate.pl  
#
#  DESCRIPTION: 
#
#      OPTIONS: ---
# REQUIREMENTS: ---
#         BUGS: ---
#        NOTES: ---
#       AUTHOR: Mik Firestone (maf), mikfire@gmail.com
# ORGANIZATION: Never
#      VERSION: 1.0
#      CREATED: 06/15/2016 12:22:40 PM
#     REVISION: ---
#===============================================================================

use strict;
use warnings;
use utf8;

use Mojo::JSON qw/decode_json/;
use JSON::Validator;
use Data::Dumper;

my ( $sname, $jname ) = @ARGV;

die "Usage: $0 <schema file> <json file>\n" if ( ! defined $sname or ! defined $jname );
my $vlad = JSON::Validator->new;
$vlad->schema($sname);

$/=undef;
open my $jfile, '<', $jname or die "Error opening $jname: $!\n";
my $json = decode_json(<$jfile>);
my $aofjson;

if ( ref($json) eq 'ARRAY' ) {
    $aofjson = $json;
}
else {
    push @$aofjson, $json
}

foreach my $thingy ( @{$aofjson} ) {
    my @errors = $vlad->validate($thingy);
    if ( @errors ) {
        print "@errors";
    }
    else {
        print "Okey dokey, hokey pokey\n";
    }
}
