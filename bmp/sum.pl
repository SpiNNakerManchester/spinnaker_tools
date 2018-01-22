#!/usr/bin/perl

use strict;
use warnings;

use String::CRC32;

use SpiNN::Util qw/bmp_version/;


die "bad args\n" unless $#ARGV == 1;

my ($SIZE, $in, $out) = (65536, @ARGV);


# Read program binary into buffer

open my $fh, "<", $in or die "Can't open $in\n";
my $len = sysread $fh, my ($buffer), $SIZE;
close $fh;

die "Program read error\n" unless defined $len;
die "Program not 4-byte aligned\n" unless $len % 4 == 0;
die "Program too big\n" if $len == $SIZE;


# Fudge to compute reset vector checksum for LPC1768 

if ($in =~ /BOOT/i)
{
    use integer;

    my @w = unpack "V8", $buffer;
    my $sum = 0;
    $sum += $w[$_] for (0..6);
    $w[7] = -$sum;
    substr $buffer, 0, 32, pack ("V8", @w);
}

# Compute checksum, append to buffer and write out

$out .= "-" . bmp_version ();

$buffer .= pack "V", ~crc32 ($buffer);

open $fh, ">", $out or die "Can't open $out\n";
syswrite $fh, $buffer or die "Write failed";
close $fh;
