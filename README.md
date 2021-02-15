[![C Actions](https://github.com/SpiNNakerManchester/spinnaker_tools/workflows/C%20Actions/badge.svg?branch=master)](https://github.com/SpiNNakerManchester/spinnaker_tools/actions?query=workflow%3A%22C+Actions%22+branch%3Amaster)

SpiNNaker Low-Level Software Tools
==================================
[<small>Automated Documentation Build</small>](http://spinnakermanchester.github.io/spinnaker_tools/)


Installation and Setup
----------------------
 1. Edit the setup file so that it points to your installations of
    ARM and/or GNU software development tools. You only need one of
    these two tools installed to build standard applications.

    Search for the string "EDIT HERE" to find the two edit points.

    The version of the GNU tools that was used to test this release is

        gcc 9.2.1 20191025 (release) [ARM/arm-9-branch revision 277599]

    This (and more recent versions) can be downloaded from
    [this web site](https://launchpad.net/gcc-arm-embedded).

    The version of the ARM tools that was used to test this release is

        ARM C/C++ Compiler, 4.1 [Build 894]

 2. Source the setup file in the directory in which it lives

        source ./setup

 3. Build SARK and spin1_api for ARM or GNU tools
    (you will probably only want to do one of these)

        make GNU=0	        # If you are using ARM tools
        make        		# If you are using GNU tools

Use of the Tools
----------------
 4. There is a generic make file which works for both C and C++ (make/app.make)
    which is used to build simple applications. The preferred way to use this
    is to create a Makefile which looks something like:

        # A single-file C app called 'example.c' or
        # a single-file C++ app called 'example.cpp'
        APP := example
        include $(SPINN_DIRS)/make/app.make

    The following variables may be set at the top of your Makefile, in
    environment variables or given as arguments to 'make':

    `APP` - to select the default source file to build (required)

    `GNU` - to choose ARM or GNU tools (`GNU=1`)

    `THUMB` - to choose to generate ARM or Thumb code (`THUMB=0`)

    `DEBUG` - to include debugging info in the ELF files (`DEBUG=0`)

    `API` - if you want link with "spin1_api" or not (`API=1`)

    `GP` - if your GNU binaries don't have names like "arm-none-eabi-gcc"
    (`GP=arm-none-eabi`)

 5. Go into the "apps/hello" directory to build your first SpiNNaker app.

        cd apps/hello
        make             # GNU tools
        make GNU=0       # ARM tools

    This should make `hello.aplx`, which is an executable file that
    you can load into SpiNNaker

 6. Now start `tubotron` on your workstation to get output from
    the program as it runs

        tubotron &

    Note that if you see this error message when you start `tubotron`

        failed to create socket: Address already in use

    it is usually because there is already a tubotron (or tubogrid)
    running and it needs exclusive use of a UDP port.

 7. Now start `ybug` to bootstrap your SpiNNaker system and load and run
    the `hello` application. You'll need to know the IP address of (one of)
    your boards to do this (here we'll use `192.168.240.253` as an example).
    If you aren't sure what state your board is in you should reset it by
    pressing the reset button.

        ybug 192.168.240.253

    You should see a start-up message and then a prompt. At the prompt
    type the following.

        boot
        app_load hello.aplx . 1 16
        ps

    This loads the code into core 1 on a single SpiNNaker chip and
    assigns it an application ID of 16. You should see a "Hello world"
    message in the Tubotron window. The `ps` command displays the status of
    every core on the SpiNNaker chip.

    The `hello` application also sends output to an internal I/O buffer
    on the chip. You can see this by using the `iobuf` command and
    telling it which core's buffer to display

        iobuf 1

    Finally, to clear the I/O buffer and stop the application you can
    use the "app_stop" command to remove all applications with a given
    ID and any resources they have used. Exit "ybug" with "quit"

        app_stop 16
        ps
        quit

    Note: If you have a SpiNN-1, SpiNN-2 or SpiNN-3 board and want all the LEDs
    on those boards to function (not just the green LED0), then one of the
    following alternative boot commands may be used:

        boot scamp.boot spin1.conf
        boot scamp.boot spin2.conf
        boot scamp.boot spin3.conf

 8. Now build a more complex application (called "`simple`"!) which runs on
    4 chips

        cd ../simple
        make

    Start `ybug` again to load and run the code. There should be no
    need to run the `boot` command again. The command to load a program
    to multiple chips is `app_load` and it has to be told which chips
    (`all` for all of them), which core (just 1 again) and an application
    ID (17 this time)

    Because this application runs on multiple cores, the startup of the
    cores needs to be synchronised. When a core reaches its
    synchronisation point (or barrier) it enters a wait state known as
    SYNC0. To allow the cores to proceed beyond the barrier a "signal"
    has to be sent to all cores to cause them to proceed. The `app_sig`
    command is used to do this, sending the signal `sync0`. In this
    example, the cores reach their barrier very quickly and so it is
    OK to send the signal immediately after the `app_load`. In other
    cases, a delay may be necessary.

        ybug 192.168.240.253
        app_load simple.aplx all 1 17
        ps
        app_sig all 17 sync0
        ps

    This program runs for around 10 seconds, flashing red LEDs as it goes.
    (Your board may not have red LEDs). Output from this example goes to
    the internal I/O buffer and you can view it with the `iobuf` command.
    You can clean up with `app_stop` again and quit `ybug`.

        iobuf 1
        app_stop 17
        ps
        quit

Additional Resources
--------------------
9.  There are some more examples in the `apps` directory. Each of these
    has a `xxx.ybug` file which contains the `ybug` commands to run the
    application. It also contains, as comments, the commands you need
    to use to build the application and start any visualisation programs
    that the application needs. These examples include

    `hello` - Hello World (as above)

    `simple` - a contrived demo of the SpiNNaker API

    `data_abort` - causes a data abort to demonstrate debugging

    `interrupt` - demonstrates a bare-metal interrupt handler

    `ring` - shows how to set up routing tables for core-to-core comms

    `random` - random number generation and simple graphics

    `pt_demo` - a path tracing program - quite complex - see its own README

    `heat_demo` - a heat diffusion example (precompiled Linux visualiser)

    `gdb_test` - shows how to use the GDB debugger with SpiNNaker. See
    the `gdb-spin` document in the `docs` directory for instructions.

10. There is documentation for SARK, `ybug`, `gdb-spin` and the Spin1
    API in the `docs` directory.

11. SpiNNaker systems mostly communicate using UDP/IP with port numbers
    in the range 17890-17899. Specifically, 17892 is used for Tubotron
    and Tubogrid, 17893 is used by SpiNNaker (e.g., by `ybug`) and 17894
    is often used by visualisers. If you have a firewall blocking any
    of these ports, you may encounter problems and it's worth checking
    the firewall before blaming anything else!

    Similarly, you may only have one visualiser (such as Tubotron) on
    each port at any one time. So you may see a message to the effect
    that the port is in use if you try to start a visualiser when one
    is already running and using the same port.

12. Some visualisers (tubotron/tubogrid) use the Perl-Tk library which
    may not be installed by default on your machine. This can be
    installed as follows

        sudo apt-get install perl-tk		# Ubuntu, etc
        sudo yum     install perl-Tk		# Fedora, etc
