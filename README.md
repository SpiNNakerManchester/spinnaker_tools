Native SpiNNaker Application Libraries
======================================

This repository contains the standard low-level SpiNNaker application
development libraries (`spin1_api` and `sark`) and a set of tools and Makefiles
for building SpiNNaker applications.

Setup
-----

These makefiles require the CodeSourcery ARM cross compiler to be installed.
Installation instructions can be found on the [SpiNNaker
Wiki](https://github.com/SpiNNakerManchester/SpiNNakerManchester.github.io/wiki/1.3-C-Development-for-SpiNNaker#-install-development-dependencies).

Once the compiler has been installed, you must build the standard SpiNNaker
libraries:

	$ cd spinnaker_tools  # You must be in the spinnaker_tools directory!
	$ source setup
	$ make


Building Applications
---------------------

When using the makefiles supplied in this repository, you must set up a number
of environment variables using:

	$ cd spinnaker_tools  # You must be in the spinnaker_tools directory!
	$ source setup

You should also ensure you have compiled the SpiNNaker libraries as described
above otherwise application compilation will fail.

### Basic Application Compilation

To quickly compile a simple single-file application for SpiNNaker, you can use
the following command:

	make -f $SPINN_DIRS/Makefile.app APP=example

This will compile the application in `example.c` and produce a SpiNNaker binary
called `example.aplx` in the current directory.

### Example Makefile

Though the above is suitable while prototyping applications, real-world
applications may contain many source files and should be compiled using their
own makefile.

[Makefile.example](./Makefile.example) provides an annotated Makefile template
showing how a simple C program can be compiled for SpiNNaker using these tools.
Once your makefile is set up, your application can then be compiled by calling:

	$ make
