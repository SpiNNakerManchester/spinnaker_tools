
##------------------------------------------------------------------------------
##
## SpiNN::Struct.pm Manages data in structs
##
## Copyright (C)    The University of Manchester - 2013
##
## Author           Steve Temple, APT Group, School of Computer Science
## Email            temples@cs.man.ac.uk
##
## Status	    Experimental software - liable to change at any time !!
##
##------------------------------------------------------------------------------


package SpiNN::Struct;


use strict;
use warnings;

use SpiNN::Cmd;
use SpiNN::Util qw/find_path/;


sub new
{
    my ($class, %opts) = @_;

    my $debug = $opts{debug} || 0;
    my $file = $opts{file} || "sark.struct";
    my $scp = $opts{scp};

    my $self = {};
    $self->{"=scp="} = $scp;

    eval { read_file ($self, $file, debug => $debug) };

    return undef if $@;

    return bless $self, $class;
}


sub number
{
    my $n = shift;

    return oct $n if $n =~/^0x[0-9a-fA-F]+$/;
    return $n + 0 if $n =~ /^\d+$/;
    return undef;
}


sub read_file
{
    my ($self, $file, %opts) = @_;

    my $debug = $opts{debug} || 0;

    $file = find_path ($file);

    die "read_file: can't find $file\n" unless $file;

    open my $fh, '<', $file or die "read_file: can't open $file\n";

    my $name = "";

    while (<$fh>)
    {
        if (/symlink (.*)/)
        {
            $file = find_path ($1);

            die "read_file: can't find $file\n" unless $file;

            close $fh;
            open $fh, '<', $file or die "read_file: can't open $file\n";
            next;
        }
        chomp;
        s/^\s+|\s+$//g;
        s/\s*#.*//;
        next if /^$/;

        print ">> $_\n" if $debug;

        if (/^name\s*=\s*(\w+)$/)
        {
            if ($name)
            {
                die "read_file: size undefined in $file\n"
		            unless $self->{$name}->{"=size="};
		        die "read_file: base undefined in $file\n"
		        unless defined $self->{$name}->{"=base="};
	        }
	        $name = $1;
	        $self->{$name}->{"=size="} = 0;
	        $self->{$name}->{"=base="} = undef;
	    }
	    elsif (/^size\s*=\s*(\S+)$/)
	    {
	        $self->{$name}->{"=size="} = number ($1);
    	}
	    elsif (/^base\s*=\s*(\S+)$/)
	    {
	        $self->{$name}->{"=base="} = number ($1);
	    }
	    elsif (/^([\w\.]+)(\[\d+\])?\s+(V|v|C|A16)\s+(\S+)\s+(%\d*[dx]|%s)\s+(\S+)$/)
	    {
	        my ($field, $index, $pack, $offset, $format, $value) = 
		    ($1, $2, $3, $4, $5, $6);

	        $offset = number ($offset);
	        $value = number ($value);

	        unless (defined $offset && defined $value) 
	        {
		        close $fh;
		        die "read_file: syntax error - $file:$.\n";
	        }

	        if (defined $index)
	        {
		        $index =~ s/\[|\]//g;
		        $self->{$name}->{$field} = [$value, $pack, $offset,
				    	                    $format, $index];
	        }
	        else
	        {
		        $self->{$name}->{$field} = [$value, $pack, $offset,
					                        $format, 1];
	        }
	    }
	    else
	    {
	        close $fh;
	        die "read_file: syntax error - $file:$.\n";
	    }

    }

    close $fh;

    print ">> EOF\n" if $debug;
    if ($debug)
    {
	    print ">> $_ $self->{$_}\n" for sort keys %$self;
    }
}


sub read_struct
{
    my ($self, $name, %opts) = @_;

    my $addr = $opts{addr};

    my $sv = $self->{$name};
    my $scp = $self->{"=scp="};

    die "read_struct: bad args\n" unless $sv && $scp;

    my $data = $scp->read ($sv->{"=base="}, $sv->{"=size="}, addr => $addr);

    $self->unpack ($name, $data);
}


sub write_struct
{
    my ($self, $name, %opts) = @_;

    my $addr = $opts{addr};

    my $data = $self->pack ($name);

    my $sv = $self->{$name};
    my $scp = $self->{"=scp="};

    die "write_struct: bad args\n" unless $sv && $scp;

    $scp->write ($sv->{"=base="}, $sv->{"=size="}, $data,
		         addr => $addr);
}


sub read_var
{
    my ($self, $var, %opts) = @_;

    my $addr = $opts{addr};

    my ($name, $field) = split /\./, $var;

    my $sv = $self->{$name};
    my $scp = $self->{"=scp="};

    die "read_var: bad args\n" unless $sv && $scp && $sv->{$field};

    my ($value, $pack, $offset, $format, $index) = @{$sv->{$field}};
    my $base = $sv->{"=base="} + $offset;

    my %size = (C => 1, v => 2, V => 4, A16 => 16);

    my $data = $scp->read ($base, $size{$pack}, addr => $addr, unpack => $pack);

    return $sv->{$field}->[0] = $data->[0];
}


sub write_var
{
    my ($self, $var, $new, %opts) = @_;

    my $addr = $opts{addr};

    my ($name, $field) = split /\./, $var;

    my $sv = $self->{$name};
    my $scp = $self->{"=scp="};

    die "write_var: bad args\n" unless $sv && $scp && $sv->{$field};

    my ($value, $pack, $offset, $format, $index) = @{$sv->{$field}};
    my $base = $sv->{"=base="} + $offset;

    my ($v) = pack $pack, $new;

    $scp->write ($base, $v, addr => $addr);

    $sv->{$field}->[0] = $v;

    return 1;
}


sub unpack
{
    my ($self, $name, $data) = @_;

    my %size = (C => 1, v => 2, V => 4, A16 => 16);

    my $sv = $self->{$name};

    die "unpack: bad args\n" unless $sv;

    for my $field (keys %$sv)
    {
	    next if $field =~ /^=/;

	    my ($value, $pack, $offset, $format, $index) = @{$sv->{$field}};
	    $value = substr $data, $offset, $size{$pack};

    	$sv->{$field}->[0] = unpack $pack, $value;
    }

    return 1;
}


sub pack
{
    my ($self, $name) = @_;

    my $sv = $self->{$name};

    die "pack: bad args\n" unless $sv;

    my %size = (C => 1, v => 2, V => 4, A16 => 16);

    my $data = chr (0) x $sv->{"=size="};

    for my $field (keys %$sv)
    {
	    next if $field =~ /^=/;

	    my ($value, $pack, $offset, $format, $index) = @{$sv->{$field}};
	    next if $value eq "-";
	    my $d = pack $pack, $value;
	    substr $data, $offset, $size{$pack}, $d;
    }

    return $data;
}


sub dump
{
    my ($self, $name) = @_;

    my $sv = $self->{$name};

    die "dump: bad args\n" unless $sv;

    my @l = grep !/^=/, keys %$sv;

    @l = sort {$sv->{$a}->[2] <=> $sv->{$b}->[2]} @l;

    for my $field (@l)
    {
	    next if $field =~ /^=/;

	    my $format = $sv->{$field}->[3];
	    my $value = $sv->{$field}->[0];
	    $format = "0x$format" if substr ($format, -1, 1) eq "x";
	    printf "%-16s $format\n", $field, $value;
    }
}


sub size
{
    my ($self, $name) = @_;

    my $sv = $self->{$name};

    die "size: bad args\n" unless $sv;

    return $sv->{"=size="};
}


sub base
{
    my ($self, $name, $new) = @_;

    my $sv = $self->{$name};

    die "base: bad args\n" unless $sv;

    my $old = $sv->{"=base="};

    $sv->{"=base="} = $new if defined $new;

    return $old;
}


sub addr
{
    my ($self, $name, $field) = @_;

    my $sv = $self->{$name};

    die "addr: bad args\n" unless $sv;
    die "addr: unknown variable $field\n" unless exists $sv->{$field};

    return $sv->{$field}->[2] + $sv->{"=base="};
}


sub get_var
{
    my ($self, $var) = @_;

    my ($name, $field) = split /\./, $var;
    my $sv = $self->{$name};

    die "get_var: bad args\n" unless $sv && $sv->{$field};

    return $sv->{$field}->[0];
}


sub set_var
{
    my ($self, $var, $value) = @_;

    my ($name, $field) = split /\./, $var;
    my $sv = $self->{$name};

    die "set_var: bad args\n" unless $sv && $sv->{$field};

    my $old = $sv->{$field}->[0];

    $sv->{$field}->[0] = $value;

    return $old;
}


sub update
{
    my ($self, $name, $file) = @_;

    my $sv = $self->{$name};

    die "update: set_var: bad args\n" unless $sv;
    die "update: can't open config file\n" unless defined $file;
    die "update: can't open \"file\"\n" unless open my $fh, "<", $file;

    while (<$fh>)
    {
	    chomp;
	    s/^\s+|\s+$//g;
	    s/\s*#.*//;
	    next if /^$/;

	    if (/^([\w\.]+)\s+(0x[0-9a-fA-F]+|\d+|time)$/)
	    {
	        my ($name, $value) = ($1, $2);

	        if (exists $sv->{$name})
	        {
		        $value = time if $value eq "time";
		        $value = oct $value if $value =~ /^0x/;
		        $sv->{$name}->[0] = $value;
	        }
	        else
	        {
		        close $fh;
		        die "update: \[$.\] unknown field \"$name\"\n";
	        }
	    }
	    else
	    {
	        close $fh;
	        die "update: \[$.\] bad line \"$_\"\n";
	    }
    }

    close $fh;

    return "";
}


#------------------------------------------------------------------------------

1;
