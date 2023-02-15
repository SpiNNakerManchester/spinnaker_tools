#!/usr/bin/perl

# Copyright (c) 2012 The University of Manchester
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

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
