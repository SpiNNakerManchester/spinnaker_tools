#include <sark.h>

void __cxa_pure_virtual() {
    io_printf(IO_BUF, "Pure Virtual Function called");
    rt_error(RTE_SWERR);
}
