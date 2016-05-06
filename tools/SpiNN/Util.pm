
##------------------------------------------------------------------------------
##
## SpiNN::Util.pm   Utility routines for SpiNNaker/ybug/etc
##
## Copyright (C)    The University of Manchester - 2013
##
## Author           Steve Temple, APT Group, School of Computer Science
## Email            temples@cs.man.ac.uk
##
## Status	    Experimental software - liable to change at any time !!
##
##------------------------------------------------------------------------------

package SpiNN::Util;

use Exporter;
use File::Basename;
use File::Spec;
use Cwd 'abs_path';

@ISA = qw/Exporter/;

@EXPORT = qw/find_path read_path read_file hex_dump parse_cores parse_region
             parse_apps parse_bits sllt_version/;


sub parse_apps
{
    my ($apps) = @_;

    return ($apps, 255) if $apps =~ /^\d+$/ && $apps < 256;

    return (undef, undef) unless $apps =~ /^(\d+)-(\d+)$/;

    my ($app_id, $range) = ($1, $2 - $1 + 1);

    return (undef, undef) if $range < 1;
    return (undef, undef) if $app_id % $range != 0;
    return (undef, undef) if $app_id + $range > 256;

    my $app_mask = 255 & ~($range - 1);

    return ($app_id, $app_mask);
}


sub parse_bits
{
    my ($mask, $min, $max) = @_;

    return 0 unless defined $mask;

    $mask = "$min-$max" if lc $mask eq "all";

    my @range = split /,/, $mask;
    $mask = 0;

    for my $sub (@range)
    {
        if ($sub =~ /^\d+$/)
        {
            return 0 if $sub < $min || $sub > $max;

            $mask |= 1 << $sub;
        }
        elsif ($sub =~ /^(\d+)-(\d+)$/)
        {
            my ($l, $h) = ($1, $2);
            return 0 if $l > $h || $l < $min || $h > $max;

            for (my $i = $l; $i <= $h; $i++)
            {
                $mask |= 1 << $i;
            }
        }
        else
        {
            return 0;
        }
    }

    return $mask;
}


sub parse_region
{
    my ($region, $x, $y) = @_;

    return 0 unless defined $region;

    if ($region eq ".")
    {
        return 0 unless defined $x && defined $y;

        my $m = ($y & 3) * 4 + ($x & 3);

        return (($x & 0xfc) << 24) +
               (($y & 0xfc) << 16) +
               (3 << 16) +
               (1 << $m);
    }

    if ($region =~ /^@(\d+),(\d+)$/)
    {
        ($x, $y) = ($1, $2);

        my $m = ($y & 3) * 4 + ($x & 3);

        return (($x & 0xfc) << 24) +
               (($y & 0xfc) << 16) +
               (3 << 16) +
               (1 << $m);
    }

    $region = "0-15" if lc $region eq "all";

    my @region = split /\./, $region;
    my $level = $#region;

    return 0 if $level < 0 || $level > 3;

    my ($x, $y) = (0, 0);

    for (my $i = 0; $i < $level; $i++)
    {
        my $d = $region[$i];

        return 0 unless $d =~ /^\d+$/ && $d >= 0 && $d <= 15;

        my $shift = 6 - 2 * $i;

        $x += ($d & 3) << $shift;
        $y += ($d >> 2) << $shift;
    }

    my $mask = parse_bits ($region[-1], 0, 15);

    return 0 unless $mask;

    return ($x << 24) + ($y << 16) + ($level << 16) + $mask;
}


sub parse_cores
{
    my ($mask) = @_;

    return parse_bits ($mask, 1, 17);
}


sub sllt_version
{
    my $version = "[unknown]";
    return $version unless defined $ENV{SPINN_DIRS};

    my $vf = "$ENV{SPINN_DIRS}/include/version.h";
    return $version unless -r $vf;

    open my $vh, "<", $vf or die;

    while (<$vh>)
    {
        next unless /^#define\s+SLLT_VER_STR\s+(\S+)/;
        $version = $1;
        $version =~ s/"//g;
        last;
    }

    close $vh;
    return $version;
}


sub find_path
{
    my ($file) = @_;

    my $currentdir = dirname(File::Spec->rel2abs( __FILE__ ));
    my $extradir = "$currentdir/../boot";
    return abs_path("$extradir/$file") if -f "$extradir/$file";

    return undef unless defined $ENV{SPINN_PATH};

    return "$ENV{SPINN_PATH}/$file" if -f "$ENV{SPINN_PATH}/$file";

    my @path = split /:/, $ENV{SPINN_PATH};

    for my $dir (@path)
    {
        $dir =~ s/(.+)\/+$/$1/;
        return "$dir/$file" if -f "$dir/$file";
    }

    print "$file not found!\n";
    return undef;
}


sub read_path
{
    my ($file, $max) = @_;

    $file = find_path ($file);
    return undef unless defined $file;

    my $size = -s $file;
    return undef if $size > $max && $max > 0;

    return undef unless open my ($fh), "<", $file;

    my $s = sysread $fh, my ($buf), $size;
    close $fh;

    return undef if $s != $size;

    return $buf;
}


sub read_file
{
    my ($file, $max) = @_;

    my $size = -s $file;
    return undef if $size > $max;

    open my $fh, "<", $file or return undef;
    my $s = sysread $fh, my ($buf), $size;
    close $fh;

    return undef if $s != $size;
    return $buf;
}

# hex_dump - dump supplied data buffer in hex - returns dumped string
#
# Options
#   format - byte (default), half, word
#   width  - number of items/line (default 32 for byte, 16 otherwise)
#   addr   - address of first byte
#   start  - offset of first byte in buffer
#   length - number of bytes to display (default is size of buffer)
#   prefix - string printed at start of each line (default "")
#   asize  - field width of address in output (default 8)

sub hex_dump
{
    my ($data, %opts) = @_;

    my $format = $opts{format} || "byte";
    my $width = $opts{width} || ($format eq "byte") ? 16 : 32;
    my $addr = $opts{addr} || 0;
    my $ptr = $opts{start} || 0;
    my $count = $opts{length} || length $data;
    my $prefix = $opts{prefix} || "";
    my $asize = $opts{asize} || 8;

    my $text = '';

    while ($count > 0)
    {
        my $w = ($width > $count) ? $count : $width;
        my $chunk = substr $data, $ptr, $w;
        my $len = length $chunk;

        last if $len == 0;

        $text .= sprintf "%s%0${asize}x ", $prefix, $addr + $ptr;

        if ($format eq 'byte')
        {
            my @d = unpack 'C*', $chunk;
            $text .= sprintf " %02x", $_ for @d;
            $text .= '   ' x ($width - 1 - $#d);
            $text .= '  ';
            $text .= ($_ < 32 || $_ > 127) ? '.' : chr $_ for @d;
        }
        elsif ($format eq 'half')
        {
            my @d = unpack 'v*', $chunk;
            $text .= sprintf " %04x", $_ for @d;
        }
        elsif ($format eq 'word')
        {
            my @d = unpack 'V*', $chunk;
            $text .= sprintf " %08x", $_ for @d;
        }

        $text .= "\n";
        $ptr += $len;
        $count -= $len;
    }

    return $text;
}


1;
