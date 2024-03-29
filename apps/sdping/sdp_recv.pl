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

# Copyright (c) 2014 The University of Manchester
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.


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
