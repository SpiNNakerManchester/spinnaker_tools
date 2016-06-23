
##------------------------------------------------------------------------------
##
## SpiNN::SCP.pm    Package implementing SpiNNaker SCP & SDP
##
## Copyright (C)    The University of Manchester - 2013
##
## Author           Steve Temple, APT Group, School of Computer Science
## Email            temples@cs.man.ac.uk
##
## Status	    Experimental software - liable to change at any time !!
##
##------------------------------------------------------------------------------
##
## History
##
## 07aug14 - added delay parameter
##
##------------------------------------------------------------------------------

package SpiNN::SCP;

use strict;
use warnings;

use IO::Socket::INET;
use POSIX;

use SpiNN::Util qw/hex_dump/;


#------------------------------------------------------------------------------


my $SPIN_PORT = 17893;
my $TIMEOUT = 0.5;
my $RETRIES = 2;
my $MAX_CORE = 31;

my $RC_OK = 0x80;

my %rc = (
    0x80 => "RC_OK",
    0x81 => "RC_LEN",
    0x82 => "RC_SUM",
    0x83 => "RC_CMD",
    0x84 => "RC_ARG",
    0x85 => "RC_PORT",
    0x86 => "RC_TIMEOUT",
    0x87 => "RC_ROUTE",
    0x88 => "RC_CPU",
    0x89 => "RC_DEAD",
    0x8a => "RC_BUF",
    0x8b => "RC_P2P_NOREPLY",
    0x8c => "RC_P2P_REJECT",
    0x8d => "RC_P2P_BUSY",
    0x8e => "RC_P2P_TIMEOUT",
    0x8f => "RC_PKT_TX",
);


#------------------------------------------------------------------------------

# Open a new UDP connection suitable for SDP or SCP traffic.
# The following options are allowed
#
# target  - the target host name or IP. If omitted, a listening socket is
#	    created
# port    - the UDP port to use (defaults to 17893 which is OK for sending)
# timeout - the timeout to use when waiting for reply packets
# retries - the number of retries to use when the target doesn't respond
# debug   - a debug value (integers > 0 cause debug output - defaults to 0)
# delay   - delay (seconds) before sending (to throttle packets)

sub new
{
    my ($class, %opts) = @_;

    my $target = $opts{target} || "";
    my $port = $opts{port} || $SPIN_PORT;
    my $timeout = $opts{timeout} || $TIMEOUT;
    my $retries = $opts{retries} || $RETRIES;
    my $debug = $opts{debug} || 0;
    my $delay = $opts{delay} || 0;

    my $socket;

    if ($target)
    {
        $socket = new IO::Socket::INET (PeerAddr => "$target:$port",
                                        Proto => "udp",
                                        Blocking => 0);
    }
    else
    {
        $socket = new IO::Socket::INET (LocalPort => $port,
                                        Proto => "udp",
                                        Blocking => 0);
    }

    return undef unless $socket;

    my $self = {};

    $self->{socket} = $socket;
    $self->{port} = $port;
    $self->{target_name} = $target;
    $self->{target_ip} = ($target) ? $socket->peerhost : "x.x.x.x";
    $self->{host_ip} = $socket->sockhost;

    $self->{timeout} = $timeout;
    $self->{retries} = $retries;
    $self->{debug} = $debug;
    $self->{delay} = $delay;

    $self->{buf_size} = 256;
    $self->{nn_id} = 0;
    $self->{tx_seq} = 0;
    $self->{rx_seq} = 0;
    $self->{cmd_rc} = 0;
    $self->{X} = 0;
    $self->{Y} = 0;
    $self->{C} = 0;

    $self->{tag} = 255;
    $self->{flags} = 0x07;
    $self->{sa} = 0;
    $self->{sp} = 255;

    $self->{sdp_hdr} = "";
    $self->{sdp_data} = "";

    bless $self, $class;
    return $self;
}


#------------------------------------------------------------------------------

# Set the default chip/core address that the connection uses to talk to
# SpiNNaker. Up to three arguments can be given with the following effects
#
# 0 args - chip_x = 255, chip_y = 255, core = 0
# 1 arg  - core = arg1 (chip_x, chip_y unchanged)
# 2 args - chip_x = arg1, chip_y = arg2, core = 0
# 3 args - chip_x = arg1, chip_y = arg2, core = arg3

sub addr
{
    my ($self, $a1, $a2, $a3) = @_;

    if ($#_ == 0) # root
    {
        $self->{X} = $self->{Y} = 255;
        $self->{C} = 0;
    }
    elsif ($#_ == 1)
    {
        die "bad core number\n" if $a1 > $MAX_CORE;
        $self->{C} = $a1;
    }
    elsif ($#_ == 2)
    {
        ($self->{X}, $self->{Y}, $self->{C}) = ($a1, $a2, 0);
    }
    elsif ($#_ == 3)
    {
        die "bad core number\n" if $a3 > $MAX_CORE;
        ($self->{X}, $self->{Y}, $self->{C}) = ($a1, $a2, $a3);
    }
    else
    {
        die "bad address\n";
    }

    return ($self->{X}, $self->{Y}, $self->{C});
}


#------------------------------------------------------------------------------

# Debug dumping routines...

sub sdp_dump
{
    my ($hdr, $data, %opts) = @_;

    my $prefix = $opts{prefix} || "#SDP ";
    my $pdata = $opts{data} || 0;

    my $text = $prefix;

    my ($flags, $tag, $dp, $sp, $dy, $dx, $sy, $sx) = unpack "C8", $hdr;

    $sp = sprintf "%d/%d", $sp >> 5, $sp & 31 if $sp > 31 && $sp != 255;
    $dp = sprintf "%d/%d", $dp >> 5, $dp & 31 if $dp > 31 && $dp != 255;
    $sp = "Ether" if $sp eq "255";
    $dp = "Ether" if $dp eq "255";

    $text .= sprintf "Flag %02x  Tag %3d  ", $flags, $tag;
    $text .= sprintf "DA %d,%d  DP %5s  SA %d,%d  SP %5s",
                     $dx, $dy, $dp, $sx, $sy, $sp;
    $text .= sprintf " \[%d]\n", length $data;

    $text .= hex_dump ($data, asize => 4, prefix => $prefix) if $pdata;

    return $text;
}


sub scp_dump
{
    my ($data, %opts) = @_;

    my $args = $opts{args} || 0;
    my $prefix = $opts{prefix} || "#SCP ";
    my $pdata = $opts{data} || 0;

    my ($cmd_rc, $seq, $rest) = unpack "v2 a*", $data;

    my $text = $prefix . sprintf "Cmd_RC %3d  Seq %5d", $cmd_rc, $seq;

    my @args = unpack "V$args", $rest;
    $text .= sprintf "  Arg%d 0x%08x", $_ + 1, $args[$_] for (0..$#args);

    $text .= sprintf " \[%d]\n", length ($rest) - 4*$args;

    $text .= hex_dump (substr ($rest, 4*$args), asize => 4,
            prefix => $prefix) if $pdata && length ($rest) > 4*$args;

    return $text;
}


#------------------------------------------------------------------------------

# Send a packet containing SDP data to a SpiNNaker machine. The default
# address (chip/core) is used unless it is overridden. An SDP header is
# constructed and prepended to the data which is then sent in a single
# UDP packet. The following options are possible
#
# addr  - specifies an address to override the default. An array reference
#          containing up to three args (see "addr" above for spec.)
# port  - a 3 bit port number to be used at the destination (default 0)
# reply - must be set true if a reply is expected from the destination
# delay - delay in seconds before each send (for throttling)


sub send_sdp
{
    my ($self, $data, %opts) = @_;

    my $addr = $opts{addr};
    my $port = $opts{port} || 0;
    my $reply = $opts{reply} || 0;
    my $debug = $opts{debug} || $self->{debug};
    my $delay = $opts{delay} || $self->{delay};

    die "SDP data overflow\n" if length ($data) > $self->{buf_size} + 16;

    my ($x, $y, $c) = ($self->{X}, $self->{Y}, $self->{C});

    if (defined $addr)
    {
        if ($#$addr == -1)
        {
            ($x, $y, $c) = (255, 255, 0);
        }
        elsif ($#$addr == 0)
        {
            ($c) = @$addr;
        }
        elsif ($#$addr == 1)
        {
            ($x, $y, $c) = (@$addr, 0);
        }
        elsif ($#$addr == 2)
        {
            ($x, $y, $c) = @$addr;
        }
        else
        {
            die "bad address\n";
        }
    }

    die "bad core number\n" if $c > $MAX_CORE;

    my $flags = $self->{flags};
    my $da = ($x << 8) + $y;
    my $dp = ($port << 5) + $c;
    $flags |= 0x80 if $reply;

    my $pad = pack "v", 0;
    my $hdr  = pack "C4 v2", $flags, $self->{tag}, $dp, $self->{sp},
                             $da, $self->{sa};

    select (undef, undef, undef, $self->{delay}) if $self->{delay};

    my $rc = send ($self->{socket}, $pad . $hdr . $data, 0);

    die "target send error\n" unless defined $rc;

    if ($debug >= 3)
    {
        print sdp_dump ($hdr, $data,
                        data => $debug >= 4,
                        prefix => "#>SDP ");
    }
}


#------------------------------------------------------------------------------

# Send a packet containing SCP data to a SpiNNaker machine (uses "send_sdp").
# A command and three arguments must be supplied and these are packed
# along with the data before being sent with "send_sdp". A sequence number
# is inserted which is kept in the class data. The same options as for
# "send_sdp" may be provided (addr, retry, port, delay).


sub send_scp
{
    my ($self, $cmd, $arg1, $arg2, $arg3, $data, %opts) = @_;

    my $debug = $opts{debug} || $self->{debug};

    die "SCP data overflow\n" if length ($data) > $self->{buf_size};

    my $scp_hdr = pack "v2 V3", $cmd, $self->{tx_seq}, $arg1, $arg2, $arg3;

    if ($debug)
    {
        print scp_dump ($scp_hdr . $data,
                        data => $debug >= 2,
                        args => 3,
                        prefix => "#>SCP ")
    }

    $self->send_sdp ($scp_hdr . $data, %opts);
}


#------------------------------------------------------------------------------

# Receive a packet containing SDP data. Waits for a timeout which is
# taken from the class data unless overridden by an option. Returns
# "undef" if the receive times out otherwise 1; The SDP header and data
# are saved in the class data.
#
# timeout - timeout for reception (overrides class data default)

sub recv_sdp
{
    my ($self, %opts) = @_;

    my $timeout = $opts{timeout} || $self->{timeout};
    my $debug = $opts{debug} || $self->{debug};

    $self->{sdp_data} = "";

    my $rd_mask = "";
    vec ($rd_mask, fileno ($self->{socket}), 1) = 1;

    my $n = select ($rd_mask, undef, undef, $timeout);

    return undef if $n == 0;

    my $addr = recv ($self->{socket}, my ($buf), 65536, 0);

    die "target recv error\n" unless defined $addr;
    die "malformed SDP packet\n" if length ($buf) < 10;

#    my ($rem_port, $rem_addr) = sockaddr_in ($addr);
#    $rem_addr = inet_ntoa ($rem_addr);
#    warn ">> $rem_addr:$rem_port\n";

    $self->{sdp_hdr} = substr $buf, 2, 8;
    $self->{sdp_data} = substr $buf, 10;

    if ($debug >= 3)
    {
        print sdp_dump ($self->{sdp_hdr}, $self->{sdp_data},
                        data => $debug >= 4,
                        prefix => "#<SDP ");
    }

    return 1;
}


#------------------------------------------------------------------------------

# Receive a packet containing SCP data. Waits for a timeout which is
# taken from the class data unless overridden by an option. Returns
# "undef" if the receive times out otherwise the "cmd_rc" field from
# the packet

sub recv_scp
{
    my ($self, %opts) = @_;

    my $timeout = $opts{timeout} || $self->{timeout};
    my $debug = $opts{debug} || $self->{debug};

    return undef unless $self->recv_sdp (timeout => $timeout);

    die "malformed SCP packet\n" if length ($self->{sdp_data}) < 4;

    ($self->{cmd_rc}, $self->{rx_seq}) = unpack "v2", $self->{sdp_data};

    if ($debug)
    {
        print scp_dump ($self->{sdp_data},
                        data => $debug >= 2,
                        prefix => "#<SCP ");
    }

    return $self->{cmd_rc}
}


#------------------------------------------------------------------------------

# Send a command to a Spinnaker target using SDP over UDP and receive
# a reply
#
# Arguments - cmd
#
# Options
#   arg1 - argument 1
#   arg2 - argument 2
#   arg3 - argument 3
#   data - data
#   port (integer) SpiNNaker (3-bit) port
#   addr ([]) chip/core address
#   unpack - unpack format for returned data
#   timeout - override default timeout
#   retries - override default retries
#   debug - set debug level
#
# Returns - data (possibly unpacked)

sub scp_cmd
{
    my ($self, $cmd, %opts) = @_;

    my $arg1 = $opts{arg1} || 0;
    my $arg2 = $opts{arg2} || 0;
    my $arg3 = $opts{arg3} || 0;
    my $data = $opts{data} || "";

    my $addr = $opts{addr};
    my $port = $opts{port} || 0;
    my $unpack = $opts{unpack};

    my $timeout = $opts{timeout} || $self->{timeout};
    my $retries = $opts{retries} || $self->{retries};
    my $debug = $opts{debug} || $self->{debug};

    $self->{tx_seq} = ($self->{tx_seq} + 1) & 0xffff;

    my $tries = 0;
    my $rc;

    while (1)
    {
        $self->send_scp ($cmd, $arg1, $arg2, $arg3, $data,
                         addr => $addr, port => $port, reply => 1);

        $rc = $self->recv_scp (timeout => $timeout, debug => $debug);

        last if defined $rc;

        $tries++;

        print "# Timeout (attempt $tries)\n" if $debug;

        die "too many retries\n" if $tries == ($retries + 1);
    }

    if ($rc != $RC_OK)
    {
        $rc = $rc{$rc} || sprintf "0x%02x", $rc;
        die "error $rc\n"
    }

    return substr ($self->{sdp_data}, 4) unless $unpack;

    return [unpack $unpack, substr ($self->{sdp_data}, 4)];
}


#------------------------------------------------------------------------------

sub timeout
{
    my ($self, $timeout) = @_;

    my $v = $self->{timeout};

    $self->{timeout} = $timeout if defined $timeout;

    return $v;
}


sub debug
{
    my ($self, $debug) = @_;

    my $v = $self->{debug};

    $self->{debug} = $debug if defined $debug;

    return $v;
}


sub flags
{
    my ($self, $flags) = @_;

    my $v = $self->{flags};

    $self->{flags} = $flags if defined $flags;

    return $v;
}


sub retries
{
    my ($self, $retries) = @_;

    my $v = $self->{retries};

    $self->{retries} = $retries if defined $retries;

    return $v;
}

#------------------------------------------------------------------------------

sub close
{
    my $self = shift;

    close $self->{socket};

    return "";
}

#------------------------------------------------------------------------------

sub dump_self
{
    my $self = shift;

    for my $key (sort keys %$self)
    {
        printf "%-16s %s\n", $key, $self->{$key};
    }
}

#------------------------------------------------------------------------------

1;
