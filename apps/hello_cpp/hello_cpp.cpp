/*
  Simple Hello World program which writes output to two different
  places. One is IO_STD, which should appear on a "tubotron" viewer.
  The other is IO_BUF which goes to a memory buffer on the SpiNNaker
  chip and can be viewed with the "iobuf" command in "ybug"
*/
#include <sark_cpp.h>

extern "C" void c_main()
{
  char buf[64];

  io_printf (IO_STD, "Hello world from C++! (via SDP)\n");

  io_printf (IO_BUF, "Hello world from C++! (via SDRAM)\n");

  // io_printf can also do sprintf!

  io_printf (buf, "Hello world from C++! (via printf...)\n");

  io_printf (IO_BUF, buf);
}
