#include <sark_cpp.h>

extern "C" void __cxa_pure_virtual() {
    io_printf(IO_BUF, "Pure Virtual Function called");
    rt_error(RTE_SWERR);
}

extern "C" void c_main(void) {
    try {
	cpp_main();
    } catch(const std::exception &e) {
	io_printf(IO_BUF, "uncaught exception: %s", e.what());
	rt_error(RTE_SWERR);
    } catch(...) {
	io_printf(IO_BUF, "unexpected non-standard exception");
	rt_error(RTE_SWERR);
    }
}
