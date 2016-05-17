
##------------------------------------------------------------------------------
##
## SpiNN::Boot.pm   Boot a SpiNNaker system
##
## Copyright (C)    The University of Manchester - 2013
##
## Author           Steve Temple, APT Group, School of Computer Science
## Email            temples@cs.man.ac.uk
##
## Status	    Experimental software - liable to change at any time !!
##
##------------------------------------------------------------------------------


package SpiNN::Boot;

use strict;
use warnings;

use SpiNN::Struct;
use SpiNN::Util qw/find_path read_path/;
use SpiNN::Cmd;


#------------------------------------------------------------------------------

# Boot using SpiNNaker BootROM protocol


sub boot_pkt
{
    my ($socket, $op, $a1, $a2, $a3, $data, $delay) = @_;

    my $PROT_VER = 1;

    my $hdr = pack 'n N N N N', $PROT_VER, $op, $a1, $a2, $a3;

    if ($data)
    {
        my @data = unpack 'V*', $data;
        $data = pack 'N*', @data;
    }

    my $rc = send ($socket, $hdr . $data, 0);

    select (undef, undef, undef, $delay);
}


sub rom_boot
{
    my ($host, $buf, $sv, $time, $debug, $port) = @_;

    my $BOOT_WORD_SIZE = 256;	# 256 words
    my $BOOT_BYTE_SIZE = 1024;	# 1024 bytes
    my $MAX_BLOCKS = 32;	# 32k limit in DTCM

    my $delay = 0.01;

    my $socket = new IO::Socket::INET(PeerAddr => "$host:$port",
                                      Proto => 'udp');

    die "can't connect to \"$host:$port\"\n" unless $socket;

    my $size = length $buf;

    my $blocks = int ($size / $BOOT_BYTE_SIZE);
    $blocks++ if $size % $BOOT_BYTE_SIZE != 0;

    print "Boot: $size bytes, $blocks blocks\n" if $debug;

    die "boot file too big\n" if $blocks > $MAX_BLOCKS;

    print "Boot: Start (delay $delay)\n" if $debug;
    boot_pkt ($socket, 1, 0, 0, $blocks - 1, '', $delay);

    for (my $block = 0; $block < $blocks; $block++)
    {
        my $data = substr $buf, $block * $BOOT_BYTE_SIZE, $BOOT_BYTE_SIZE;

        my $a1 = (($BOOT_WORD_SIZE - 1) << 8) | ($block & 255);

        print "Boot: Data $block\r" if $debug;
        boot_pkt ($socket, 3, $a1, 0, 0, $data, $delay);
    }

    print "\nBoot: End\n" if $debug;
    boot_pkt ($socket, 5, 1, 0, 0, '', $delay);

    close $socket;

    select (undef, undef, undef, 2.0); # Wait for boot...

    return "";
}


#------------------------------------------------------------------------------

# Boot using SpiNNaker SC&MP protocol


sub scamp_boot
{
    my ($host, $buf, $sv, $time, $debug, $port) = @_;

    my $spin = SpiNN::Cmd->new (target => $host, debug => $debug);

    die "Failed to open \"$host\"\n" unless $spin;

    eval
    {
        $spin->ver;

        $spin->data =~ /^SC&MP 0.91/ || die "Expected SC&MP 0.91\n";

        $spin->write ($sv->addr ("sv.rom_cpus"), chr (0));

        $spin->flood_boot ($buf);

        my $data = $spin->read (0xf5007f5c, 4, unpack => "V");

        die "boot signature failure\n" unless $time == $data->[0];
    };

    $spin->close;

    return $@;
}


#------------------------------------------------------------------------------

# Main bootstrap routine.

sub boot
{
    my ($class, $host, $file, $conf, %opts) = @_;

    my $debug = $opts{debug} || 0;
    my $port = $opts{port} || 54321;

    my $sv = SpiNN::Struct->new;
    die "failed to process \"sv\" struct file\n" unless $sv;

    if ($conf)
    {
        $sv->update ("sv", find_path ($conf));
    }

    my $buf = read_path ($file, 32768);
    die "failed to load \"$file\"\n" unless defined $buf;

    my $time = time ();
    $sv->set_var ("sv.unix_time", $time);
    $sv->set_var ("sv.boot_sig", $time);

    if ($file =~ /\.boot$/)
    {
        $sv->set_var ("sv.root_chip", 1);

        substr $buf, 384, 128, substr ($sv->pack ("sv"), 0, 128);

        rom_boot ($host, $buf, $sv, $time, $debug, $port);
    }
    elsif ($file =~ /\.aplx$/)
    {
        $sv->set_var ("sv.boot_delay", 0);

        substr $buf, 384, 128, substr ($sv->pack ("sv"), 0, 128);

        scamp_boot ($host, $buf, $sv, $time, $debug, $port);
    }
    else
    {
        die "unknown boot file format\n";
    }
}

#------------------------------------------------------------------------------

1;
