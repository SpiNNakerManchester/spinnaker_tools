#!/usr/bin/perl

##------------------------------------------------------------------------------
##
## sdp_ping	    SDP ping program
##
## Copyright (C)    The University of Manchester - 2014
##
## Author           Steve Temple, APT Group, School of Computer Science
## Email            temples@cs.man.ac.uk
##
##------------------------------------------------------------------------------

# Copyright (c) 2014-2019 The University of Manchester
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

use SpiNN::SCP;


my $sleep = 1.0;	# Repeat (secs)
my $debug = 4;		# Debug level (3 or 4)

my $spin;		# SpiNNaker handle
my $port;		# SpiNNaker app. port


# Process the five arguments and open the connection to SpiNNaker
# The arguments are
#   hostname (or IP address) of the SpiNNaker system
#   X coordinate of the SpiNNaker chip
#   Y coordinate of the SpiNNaker chip
#   core number on the SpiNNaker chip
#   SDP port to which the messages should be sent

sub process_args
{
    die "usage: sdp_ping <hostname> <chipX> <chipY> <CPU> <port>\n" unless
	$#ARGV == 4 &&
	$ARGV[1] =~ /^\d+$/ &&
	$ARGV[2] =~ /^\d+$/ &&
	$ARGV[3] =~ /^\d+$/ &&
	$ARGV[4] =~ /^\d+$/;

    $spin = SpiNN::SCP->new (target => $ARGV[0]);
    die "Failed to connect to $ARGV[0]\n" unless $spin;

    $spin->addr ($ARGV[1], $ARGV[2], $ARGV[3]);

    $port = $ARGV[4];
}


# Main loop which sends a ping SDP packet every "$sleep" seconds and looks
# for incoming reply packets. Both sent and received packets are printed
# using the SpiNN::SCP debug facility. We expect a reply only if we send
# to port 1

sub main
{
    process_args ();

    my $data = "0123456789abcdef" . chr(0);
    my $pad = pack "V4", 0, 0, 0, 0;

    while (1)
    {
	$spin->send_sdp ($pad . $data, port => $port, reply => ($port == 1),
			 debug => $debug);

	print "\n";

	my $rc = $spin->recv_sdp (timeout => 0.1, debug => $debug);

	print "# No reply\n" unless $rc;
	print "\n";

	select (undef, undef, undef, $sleep);
    }
}


main ();

#------------------------------------------------------------------------------
