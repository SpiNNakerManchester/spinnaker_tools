
#include <sark.h>

void c_main (void)
{
  uint *rom = (uint *) ROM_BASE;
  uint data = 0x55aa55aa;

  io_printf (IO_STD, "Reading ROM at %08x = %08x\n", (uint) rom, rom[0]);
  io_printf (IO_STD, "Writing ROM at %08x > %08x\n", (uint) rom, data);

  rom[0] = data;

  io_printf (IO_STD, "Reading ROM at %08x = %08x\n", (uint) rom, rom[0]);  
}
