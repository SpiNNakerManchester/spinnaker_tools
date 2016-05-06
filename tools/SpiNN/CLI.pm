
##------------------------------------------------------------------------------
##
## SpiNN::CLI.pm    Simple command line interpreter
##
## Copyright (C)    The University of Manchester - 2013
##
## Author           Steve Temple, APT Group, School of Computer Science
## Email            temples@cs.man.ac.uk
##
## Status	    Experimental software - liable to change at any time !!
##
##------------------------------------------------------------------------------

package SpiNN::CLI;

use strict;
use warnings;

#-----------------------------------------------------------------------------
# new - return a new CLI data structure
#
# arg0 - class
#
# arg1 - filehandle to read input from
# arg2 - prompt string
# arg3 - command list (a hash reference)
# arg4 - (if present) Term::ReadLine handle
#
# returns - CLI data structure
#-----------------------------------------------------------------------------

sub new
{
    my ($class, $fh, $prompt, $cmd, $term, %opts) = @_;

    my $self;

    $self->{fh} = $fh;
    $self->{prompt} = $prompt;
    $self->{cmd} = $cmd;
    $self->{term} = $term;

    $self->{level} = 0;
    $self->{quiet} = 0;
    $self->{tty} = -t $self->{fh};

    $self->{term} = undef unless $self->{tty};

    $self->{arg_c} = 0;
    $self->{arg_v} = [];
    $self->{arg_n} = [];
    $self->{arg_x} = [];

    bless $self, $class;

    return $self;
}


#-----------------------------------------------------------------------------
# run - execute a CLI using supplied data structure
#
# arg1 - CLI data structure (eg made with "new")
#-----------------------------------------------------------------------------

sub run
{
    my $self = shift;

    my $fh = $self->{fh};
    my $tty = $self->{tty};

    $| = ($tty) ? 1 : 0;

    while (1)
    {
        my $prompt = $self->{prompt};
        my $cmds = $self->{cmd};
        my $arg_v = $self->{arg_v} = [];
        my $arg_n = $self->{arg_n} = [];
        my $arg_x = $self->{arg_x} = [];
        my $term = $self->{term};

        print $prompt if $tty && ! defined $term;

        $_ = $term->readline ($prompt) if defined $term;
        $_ = <$fh> unless defined $term;

        unless (defined $_)
        {
            last unless $tty;
            print "\n";
            next;
        }

            print "$prompt$_" if !$tty && !$self->{quiet};

        chomp;
        s/^\s*|\s*$//g;

        next if /^$/ || /^\#/;

        @$arg_v = split;

        my $cmd = shift @$arg_v;
        my $ac = $self->{arg_c} = @$arg_v;

        for (my $i = 0; $i < $ac; $i++)
        {
            my $s = $arg_v->[$i];
            $arg_n->[$i] = undef;
            $arg_x->[$i] = undef;

            $arg_n->[$i] = $1 if $s =~ /^(-?\d+)$/;
            $arg_n->[$i] = hex $1 if $s =~ /^0x([0-9A-Fa-f]+)$/;
            $arg_x->[$i] = hex $1 if $s =~ /^([0-9A-Fa-f]+)$/;
            $arg_x->[$i] = hex $1 if $s =~ /^0x([0-9A-Fa-f]+)$/;
        }

        if (exists $cmds->{$cmd})
        {
            if ($ac == 1 && $arg_v->[0] eq '?')
            {
                print "usage:   $cmd $cmds->{$cmd}->[1]\n";
                print "purpose: $cmds->{$cmd}->[2]\n";
            }
            else
            {
                my $proc = $cmds->{$cmd}->[0];
                my $rc = &$proc ($self);
                last if $rc eq '1';
                chomp $rc;
                print "error: $rc\n" if $rc;
            }
        }
        else
        {
            print "bad command \"$cmd\"\n";
        }
    }
}


#-----------------------------------------------------------------------------
# cmd - update command list
#
# arg1 - CLI data structure
# arg2 - command list (hash reference)
# arg3 - bool - delete old commands
#
#-----------------------------------------------------------------------------

sub cmd
{
    my ($self, $new, $delete) = @_;

    $self->{cmd} = {} if $delete;

    for my $cmd (keys %$new)
    {
        $self->{cmd}->{$cmd} = $new->{$cmd}
    }
}


#-----------------------------------------------------------------------------
# list - return command list
#
# arg1 - CLI data structure
#
#-----------------------------------------------------------------------------

sub list
{
    my ($self) = @_;

    return keys %{$self->{cmd}};
}


#-----------------------------------------------------------------------------
# pause - print a string and wait for Enter key
#
# arg1 - CLI data structure
#
#-----------------------------------------------------------------------------

sub pause
{
    my ($self) = @_;

    my $text = join " ", @{$self->{arg_v}};
    $text =~ s/\\n/\n/g;

    print $text;

    $_ = <STDIN>;

    return 0;
}


#-----------------------------------------------------------------------------
# echo - print a string
#
# arg1 - CLI data structure
#
#-----------------------------------------------------------------------------

sub echo
{
    my ($self) = @_;

    my $text = join " ", @{$self->{arg_v}};
    $text =~ s/\\n/\n/g;

    print $text;

    return 0;
}


#-----------------------------------------------------------------------------
# at - command to read CLI commands from a file
#
# arg1 - CLI data structure
#
# returns 0
#-----------------------------------------------------------------------------

sub at
{
    my $self = shift;
    my $quiet = 0;

    return "filename expected" if $self->{arg_c} < 1;
    return "\@ nested too deep" if $self->{level} > 10;

    my $fn = $self->{arg_v}->[0];

    $quiet = $self->{arg_v}->[1] if $self->{arg_c} > 1;

    open my $fh, '<', $fn or return "can't open \"$fn\"";

    my $old_fh = $self->{fh};
    my $old_tty = $self->{tty};
    my $old_prompt = $self->{prompt};
    my $old_quiet = $self->{quiet};
    my $old_term = $self->{term};

    $self->{fh} = $fh;
    $self->{tty} = -t $fh;
    $self->{prompt} = '@' . $self->{prompt};
    $self->{quiet} = $quiet;
    $self->{term} = undef;
    $self->{level}++;

    $self->run;

    $self->{level}--;
    $self->{fh} = $old_fh;
    $self->{tty} = $old_tty;
    $self->{prompt} =~ s/^@//;
    $self->{quiet} = $old_quiet;
    $self->{term} = $old_term;

    close $fh;

    return 0;
}


#-----------------------------------------------------------------------------
# quit - command to quit current CLI
#
# arg1 - CLI data structure
#
# returns 1
#-----------------------------------------------------------------------------

sub quit
{
    return 1;
}


#-----------------------------------------------------------------------------
# help - command to print help information on CLI commands
#
# arg1 - CLI data structure
#
# returns 0
#-----------------------------------------------------------------------------

sub help
{
    my $self = shift;
    my $cmds = $self->{cmd};
    my $ac = $self->{arg_c};
    my $cmd = $self->{arg_v}->[0];

    if ($ac == 1 && exists $cmds->{$cmd})
    {
        print "usage:   $cmd $cmds->{$cmd}->[1]\n";
        print "purpose: $cmds->{$cmd}->[2]\n";
    }
    else
    {
        printf " %-12s %-30s - %s\n", $_, $cmds->{$_}->[1], $cmds->{$_}->[2]
        for (sort keys %$cmds);
    }

    return 0;
}


#-----------------------------------------------------------------------------
# query - command to print a list of CLI commands
#
# arg1 - CLI data structure
#
# returns 0
#-----------------------------------------------------------------------------

sub query
{
    my $self = shift;
    my $cmds = $self->{cmd};
    my $s = "";

    for (sort keys %$cmds)
    {
        if (length "$s $_" > 78)
        {
            print "$s\n";
            $s = "";
        }
        $s .= " $_";
    }
    print "$s\n" if $s;

    return 0;
}

#-----------------------------------------------------------------------------

1;
