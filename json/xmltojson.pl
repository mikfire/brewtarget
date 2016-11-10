#!/usr/bin/env perl 
#===============================================================================
#
#         FILE: xmltojson.pl
#
#        USAGE: ./xmltojson.pl  
#
#  DESCRIPTION: Your mama told me to write this code.
#
#      OPTIONS: ---
# REQUIREMENTS: ---
#         BUGS: ---
#        NOTES: ---
#       AUTHOR: Mik Firestone (maf), mikfire@gmail.com
# ORGANIZATION: Never
#      VERSION: 1.0
#      CREATED: 06/12/2016 02:19:39 PM
#     REVISION: ---
#===============================================================================
use strict;
use warnings;
use utf8;

use XML::Simple qw(:strict);
use JSON;
use Data::Dumper;

sub clean_entries {
    my $ref = shift;

    for my $key ( sort keys %$ref ) {
        if ( $key eq 'entryinstructions' ) {
            delete $ref->{$key};
        }
        if ( ! ref($ref->{$key})) {
            if ( defined $ref->{$key} ) {
                $ref->{$key} =~ s/^\n//;
            }
        }
        elsif ( ref($ref->{$key}) eq 'HASH' ) {
            clean_entries( $ref->{$key} );
        }
    }
}

sub make_tags {
    my $ref = shift;
    my $tags = $ref->{tags};

    if ( defined $tags ) {
        my @tmp = split /,\s*/, $tags;
        $ref->{tags} = \@tmp;
    }
}

sub fix_stats {
    my ($ref, $debug) = @_;

    $debug = defined($debug) ? $debug : 0;

    # do the gravities first
    if ( defined ( $ref->{og} ) ) {
        $ref->{gravity}{original}{min} = $ref->{og}{low};
        $ref->{gravity}{original}{max} = $ref->{og}{high};
        delete $ref->{og};
    }

    if ( defined ( $ref->{fg} ) ) {
        $ref->{gravity}{final}{min} = $ref->{fg}{low};
        $ref->{gravity}{final}{max} = $ref->{fg}{high};
        delete $ref->{fg};
    }

    for my $key ( qw/srm abv ibu/ ) {
        print STDERR "key: $key\n" if ( $debug );
        if ( defined( $ref->{$key} ) ) {
            print STDERR "min: $ref->{$key}{low}\n" if $debug;
            print STDERR "max $ref->{$key}{high}\n" if $debug;
            $ref->{$key}{min} = $ref->{$key}{low};
            $ref->{$key}{max} = $ref->{$key}{high};
            delete $ref->{$key}{low};
            delete $ref->{$key}{high};
            delete $ref->{$key}{flexible};
        }
    }
}

sub add_information {
    my ($ref, $mtype, $catid, $catname, $subcatid, $revision, $counter) = @_;

    $ref->{source} = "bjcp";
    $ref->{class} = $mtype;
    $ref->{category} = $catname;
    $ref->{categoryid} = $catid;
    $ref->{subcategory} = $subcatid;
    $ref->{revision} = $revision;
    $ref->{id} = $counter;
}

sub weird_sort {
    my %order = ( id           => 0,
                  source       => 100,
                  class        => 200,
                  name         => 300,
                  revision     => 400,
                  categoryid   => 500,
                  subcategory  => 600,
                  category     => 601,
                  appearance   => 700,
                  aroma        => 800,
                  comments     => 900,
                  examples     => 1000,
                  comparison   => 1100,
                  flavor       => 1200,
                  history      => 1300,
                  impression   => 1400,
                  ingredients  => 1500,
                  varieties    => 1500,
                  mouthfeel    => 1700,
                  notes        => 1800,
                  tags         => 1900,
                  stats        => 2000,
                  abv          => 2100,
                  gravity      => 2200,
                  ibu          => 2300,
                  srm          => 2400,
                  min          => 2500,
                  max          => 2600,
                  original     => 2700,
                  final        => 2800,
                  exceptions   => 2900,
                  content      => 10000,
    );
    if ( not defined($order{$JSON::PP::a}) ) { 
        print STDERR $JSON::PP::a,"\n";
    }
    elsif ( not defined($order{$JSON::PP::b}) ) {
        print STDERR  $JSON::PP::b,"\n";
    }

    return $order{$JSON::PP::a} <=> $order{$JSON::PP::b};
}

sub process_entry {
    my $ref = shift;
    my $json = JSON->new;

    # The XML file has an excess of newlines. This cleans them up
    for my $sref ( @{$ref} ) {
        clean_entries($sref);
        # I need to re-arrange the stats a little.
        fix_stats($sref->{stats});
        # I want an array for the tags
        make_tags($sref);
    }

    print STDERR "DEBUG: ", scalar(@{$ref}), "\n";
    # print Dumper( $ref );
    $json->sort_by( \&weird_sort );
    print $json->pretty->utf8->encode($ref);
}

my $doc = XMLin('styleguide.xml', KeyAttr => { id => 'category id' }, ForceArray => ['id']); 

my $counter=200;

my @styles;
for my $class ( @{$doc->{class}} ) {
    my $type = $class->{type};

    for my $cat ( @{$class->{category}} ) {
        my $catid = $cat->{id};
        my $catname = $cat->{name};
        my $revision = $cat->{revision}{content};
        if ( not defined $revision ) {
            warn "$catid did not define a revision\n";
        }
        else {
            $revision =~ s/^\n//;
        }

        if ( ref($cat->{subcategory}) eq 'ARRAY' ) { 
            for my $subcat ( @{$cat->{subcategory}} ) {
                next unless defined $subcat->{id};
                print STDERR "DEBUG: $subcat->{id}\n";
                add_information( $subcat, $type, $catid, $catname, $subcat->{id}, $revision, $counter++);
                push @styles, $subcat;
            }
        }
        elsif ( ref($cat->{subcategory}) eq 'HASH' ) {
            next unless defined $cat->{id};
            print STDERR "DEBUG: $cat->{id}\n";
            add_information( $cat, $type, $catid, $catname, $cat->{id}, $revision, $counter++);
            push @styles, $cat;
        }
        else {
            print "Unknown reference type: ", ref($cat->{subcategory}), "\n";
            exit 2;
        }
    }
}
print STDERR "DEBUG: ", scalar(@styles), " :: $counter\n";
process_entry( \@styles );
