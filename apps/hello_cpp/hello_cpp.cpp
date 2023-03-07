/*
 * Copyright (c) 2016 The University of Manchester
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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

    io_printf(IO_STD, "Hello world from C++! (via SDP)\n");

    io_printf(IO_BUF, "Hello world from C++! (via SDRAM)\n");

    // io_printf can also do sprintf!

    io_printf(buf, "Hello world from C++! (via printf...)\n");
    io_printf(IO_BUF, buf);
}
