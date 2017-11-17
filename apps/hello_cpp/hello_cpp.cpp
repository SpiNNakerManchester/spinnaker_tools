/*
  Simple Hello World program which writes output to two different
  places. One is IO_STD, which should appear on a "tubotron" viewer.
  The other is IO_BUF which goes to a memory buffer on the SpiNNaker
  chip and can be viewed with the "iobuf" command in "ybug"
*/
#include <sark_cpp.h>

class AbstractHello {
public:
    virtual void hello(const char* str) = 0;
};

class HelloSDP : public AbstractHello {
public:
    void hello(const char* str) {
	io_printf(IO_STD, "%s (via SDP)\n", str);
    }
};

class HelloSDRAM : public AbstractHello {
public:
    void hello(const char* str) {
	io_printf(IO_BUF, "%s (via SDRAM)\n", str);
    }
};

class HelloBuf : public AbstractHello {
public:
    void hello(const char* str) {
	char buf[64];
	io_printf(buf, "%s (via printf...)\n", str);
	io_printf(IO_BUF, buf);
    }
};

void cpp_main() {
    AbstractHello *hellos[] = {
	new HelloSDP(), new HelloSDRAM(), new HelloBuf()
    };

    for (AbstractHello *hello : hellos) {
        hello->hello("Hello world from C++!");
    }
}
