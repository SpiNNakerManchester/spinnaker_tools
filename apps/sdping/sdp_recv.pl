#!/usr/bin/perl

##------------------------------------------------------------------------------
##
## sdp_recv	    SDP receiver program
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


my $spin;		# SpiNNaker handle
my $timeout;		# Receive timeout


# Process the two arguments and open connection to SpiNNaker
# The first argument is the UDP port to listen on and the
# second is the timeout (in seconds) for the receive operation.

sub process_args
{
    die "usage: sdp_recv <port> <timeout>\n" unless
	$#ARGV == 1 &&
	$ARGV[0] =~ /^\d+$/ &&
	$ARGV[1] =~ /^\d+(\.\d+)?$/;

    $spin = SpiNN::SCP->new (port => $ARGV[0]);

    die "Failed to start receiver\n" unless $spin;

    $timeout = $ARGV[1];
}


# Main loop which waits for incoming SDP messages and prints them
# using the SpiNN::SCP debug facility

sub main
{
    process_args ();

    while (1)
    {
	my $rc = $spin->recv_sdp (timeout => $timeout, debug => 4);

	print "# No reply\n" unless $rc;
	print "\n";
    }
}


main ();

#------------------------------------------------------------------------------
