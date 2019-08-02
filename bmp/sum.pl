#!/usr/bin/perl

# Copyright (c) 2012-2019 The University of Manchester
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
