
/*
  Simple test application for sdp callback

  To run the tests, after compiling the code, execute in ybug:
      app_load sdptest.aplx . 1-2 30
      ps
      app_sig all 30 sync0
      ps
      iobuf 1
      app_stop 30

  Modify the TEST_ID parameter below to execute the appropriate test
  and then recompile the code

  Test 0: Set callback for SDP message receive with all priorities, and
          checks that the internal callback array is set accordingly
  Test 1: Set callback for each port-received SDP message with each priority
          and check that the internal handler is set apropriately.
  Test 2: Set two callbacks for port-received SDP_messages, one fith fixed
          port and priority, the second goes through all the remaining ports
          and priorities, checkin gthat the internal handler is set
          appropriately.
  Test 3: Sets one callback routine for the SDP packet receive event, and
          then replaces it with a second callback routine, checking that
          the second callback is stored.
  Test 4: Sets one callback routine for the port-based SDP packet receive
          event, and then replaces it with a second callback routine,
          checking that the second callback is stored.
  Test 5: Checks that setting a SDP callback and then a port-based SDP
          callback generates an RTE error.
          THIS TEST GENERATES A RUN-TIME EXCEPTION ERROR
  Test 6: Checks that setting a port-based SDP callback and then a SDP
          callback generates an RTE error.
          THIS TEST GENERATES A RUN-TIME EXCEPTION ERROR
  Test 7: Transmission of a SDP packet from one core to a SDP callback
          on all the available ports (1-7)
  Test 8: Transmission of a SDP packet from one core to a port-based
          SDP callback on one specific port (1)
  Test 9: Transmission of a SDP packet from one core to a destination
          port for which there is no callback set. The result is that
          the SDP message is dropped (NO STATEMENT IS PRINTED!)
  Test 10: A callback is registered for each SDP port and the sender
           sends a message to each of the ports, checking that the
           appropriate callback is triggered
*/

#include "spin1_api.h"
#include "spin1_api_params.h"

#define TEST_ID 5

#define TEST_MESSAGE "test send sdp message\n"
#define TEST_MESSAGE_LEN 23

typedef void (*func_t) ();

//list of test routines for sender and receiver
void setup_sender0();
void setup_sender1();
void setup_sender2();
void setup_sender3();
void setup_sender4();
void setup_sender5();
void setup_sender6();
void setup_sender7();
void setup_sender8();
void setup_sender9();
void setup_sender10();

void setup_test0();
void setup_test1();
void setup_test2();
void setup_test3();
void setup_test4();
void setup_test5();
void setup_test6();
void setup_test7();
void setup_test8();
void setup_test9();
void setup_test10();

void sender0();
void sender1();
void sender2();
void sender3();
void sender4();
void sender5();
void sender6();
void sender7();
void sender8();
void sender9();
void sender10();

void test0();
void test1();
void test2();
void test3();
void test4();
void test5();
void test6();
void test7();
void test8();
void test9();
void test10();


//callbacks used in the tests
void timer_callback(uint timer_count, uint unused);
void sdp_packet_callback_test0(uint mailbox, uint port);
void sdp_packet_callback_test1(uint mailbox, uint port);
void sdp_packet_callback_port0(uint mailbox, uint port);
void sdp_packet_callback_port1(uint mailbox, uint port);
void sdp_packet_callback(uint mailbox, uint port);
void sdp_port_packet_callback_1(uint mailbox, uint port);
void sdp_port_packet_callback_2(uint mailbox, uint port);
void sdp_port_packet_callback_3(uint mailbox, uint port);
void sdp_port_packet_callback_4(uint mailbox, uint port);
void sdp_port_packet_callback_5(uint mailbox, uint port);
void sdp_port_packet_callback_6(uint mailbox, uint port);
void sdp_port_packet_callback_7(uint mailbox, uint port);


//arrays of function to run during the tests
func_t function_setup_test_list[] =
  {setup_test0, setup_test1, setup_test2, setup_test3, setup_test4,
  setup_test5, setup_test6, setup_test7, setup_test8, setup_test9,
  setup_test10};

func_t function_setup_sender_list[] =
  {setup_sender0, setup_sender1, setup_sender2, setup_sender3, setup_sender4,
  setup_sender5, setup_sender6, setup_sender7, setup_sender8, setup_sender9,
  setup_sender10};

func_t function_test_list[] =
  {test0, test1, test2, test3, test4,
  test5, test6, test7, test8, test9,
  test10};

func_t function_sender_list[] =
  {sender0, sender1, sender2, sender3, sender4,
  sender5, sender6, sender7, sender8, sender9,
  sender10};

sdp_msg_t msg;

void c_main (void)
{
  //wait for 1 msec
  unsigned int timer_period = 100000;

  io_printf (IO_BUF, "Running test %d\n", TEST_ID);

  spin1_set_timer_tick(timer_period);
  spin1_callback_on(TIMER_TICK, timer_callback, 0);

  if (spin1_get_core_id() == 1)
    function_setup_test_list[TEST_ID]();
  else
    function_setup_sender_list[TEST_ID]();

  spin1_start(SYNC_WAIT);
}

int min(int a, int b)
{
  if (a < b)
    return a;
  else
    return b;
}

void timer_callback(uint timer_count, uint unused)
{
  if (spin1_get_core_id() == 1)
    function_test_list[TEST_ID]();
  else
    function_sender_list[TEST_ID]();

  if (timer_count >= 10)
    spin1_exit(0);
}

void sdp_packet_callback_test0(uint mailbox, uint port)
{

}

void sdp_packet_callback_test1(uint mailbox, uint port)
{

}

void sdp_packet_callback_port0(uint mailbox, uint port)
{

}

void sdp_packet_callback_port1(uint mailbox, uint port)
{

}

void sdp_packet_callback(uint mailbox, uint port)
{
  sdp_msg_t *msg_ptr = (sdp_msg_t *) mailbox;

  io_printf (IO_BUF, "Received message on port %d: %s\n", port, (char*)&(msg_ptr -> cmd_rc));

  spin1_msg_free(msg_ptr);
}

void sdp_port_packet_callback_1(uint mailbox, uint port)
{
  sdp_msg_t *msg_ptr = (sdp_msg_t *) mailbox;

  io_printf (IO_BUF, "Callback 1: Received message on port %d: %s\n", port, (char*)&(msg_ptr -> cmd_rc));

  spin1_msg_free(msg_ptr);
}

void sdp_port_packet_callback_2(uint mailbox, uint port)
{
  sdp_msg_t *msg_ptr = (sdp_msg_t *) mailbox;

  io_printf (IO_BUF, "Callback 2: Received message on port %d: %s\n", port, (char*)&(msg_ptr -> cmd_rc));

  spin1_msg_free(msg_ptr);
}

void sdp_port_packet_callback_3(uint mailbox, uint port)
{
  sdp_msg_t *msg_ptr = (sdp_msg_t *) mailbox;

  io_printf (IO_BUF, "Callback 3: Received message on port %d: %s\n", port, (char*)&(msg_ptr -> cmd_rc));

  spin1_msg_free(msg_ptr);
}

void sdp_port_packet_callback_4(uint mailbox, uint port)
{
  sdp_msg_t *msg_ptr = (sdp_msg_t *) mailbox;

  io_printf (IO_BUF, "Callback 4: Received message on port %d: %s\n", port, (char*)&(msg_ptr -> cmd_rc));

  spin1_msg_free(msg_ptr);
}

void sdp_port_packet_callback_5(uint mailbox, uint port)
{
  sdp_msg_t *msg_ptr = (sdp_msg_t *) mailbox;

  io_printf (IO_BUF, "Callback 5: Received message on port %d: %s\n", port, (char*)&(msg_ptr -> cmd_rc));

  spin1_msg_free(msg_ptr);
}

void sdp_port_packet_callback_6(uint mailbox, uint port)
{
  sdp_msg_t *msg_ptr = (sdp_msg_t *) mailbox;

  io_printf (IO_BUF, "Callback 6: Received message on port %d: %s\n", port, (char*)&(msg_ptr -> cmd_rc));

  spin1_msg_free(msg_ptr);
}

void sdp_port_packet_callback_7(uint mailbox, uint port)
{
  sdp_msg_t *msg_ptr = (sdp_msg_t *) mailbox;

  io_printf (IO_BUF, "Callback 7: Received message on port %d: %s\n", port, (char*)&(msg_ptr -> cmd_rc));

  spin1_msg_free(msg_ptr);
}

void setup_sender0() { }

void setup_test0()
{
  int i;
  int callback_priority;
  callback_t callback_cback;

  for (i = -1; i < 4; i++)
  {
    spin1_callback_on(SDP_PACKET_RX, sdp_packet_callback_test0, i);

    callback_priority = callback[SDP_PACKET_RX].priority;
    io_printf (IO_BUF, "callback SDP packet priority: %d\n", callback_priority);
    if (i != callback_priority)
    {
      io_printf(IO_BUF, "Requested callback priority: %d, assigned priority: %d\n", i, callback_priority);
      rt_error (RTE_API);
    }

    spin1_callback_off(SDP_PACKET_RX);

    callback_priority = callback[SDP_PACKET_RX].priority;
    callback_cback = callback[SDP_PACKET_RX].cback;
    if (callback_cback != NULL)
    {
      io_printf(IO_BUF, "Callback not cleared!\n", i, callback_priority);
      rt_error (RTE_API);
    }
  }
}

void sender0() { }

void test0() { }



void setup_sender1() { }

void setup_test1()
{
  int i, port;
  int callback_priority;
  callback_t callback_cback;

  for (port = 1; port <=7; port++)
  {
    for (i = -1; i < 4; i++)
    {
      spin1_sdp_callback_on(port, sdp_packet_callback_test0, i);

      callback_priority = callback[SDP_PACKET_RX].priority;
      io_printf (IO_BUF, "port: %d, callback SDP port packet priority: %d, requested priority: %d\n", port, callback_priority, i);
      if (i != callback_priority)
      {
        io_printf(IO_BUF, "Requested callback priority: %d, assigned priority: %d\n", i, callback_priority);
        rt_error (RTE_API);
      }

      spin1_sdp_callback_off(port);

      callback_priority = callback[SDP_PACKET_RX].priority;
      callback_cback = callback[SDP_PACKET_RX].cback;
      if (callback_cback != NULL)
      {
          io_printf(IO_BUF, "Callback not cleared!\n", i, callback_priority);
          rt_error (RTE_API);
      }
    }
  }
}

void sender1() { }

void test1() { }



void setup_sender2() { }

void setup_test2()
{
  int base_priority = 1;
  int i, port;

  spin1_sdp_callback_on(1, sdp_packet_callback_port0, base_priority);

  for (port = 2; port <=7; port++)
  {
    for (i = -1; i < 4; i++)
    {
      int callback_priority;
      spin1_sdp_callback_on(port, sdp_packet_callback_port1, i);

      callback_priority = callback[SDP_PACKET_RX].priority;
      io_printf (IO_BUF, "port: %d, callback SDP port packet priority: %d, requested priority: %d\n", port, callback_priority, i);
      if (callback_priority != min(base_priority, i))
      {
        io_printf(IO_BUF, "Requested callback priority: %d, assigned priority: %d\n", i, callback_priority);
        rt_error (RTE_API);
      }
    }
    spin1_sdp_callback_off(port);
  }
}

void sender2() { }

void test2() { }



void setup_sender3() { }

void setup_test3()
{
  callback_t callback_routine;

  spin1_callback_on(SDP_PACKET_RX, sdp_packet_callback_test0, 0);
  spin1_callback_on(SDP_PACKET_RX, sdp_packet_callback_test1, 0);

  callback_routine = callback[SDP_PACKET_RX].cback;
  io_printf (IO_BUF, "callback SDP address: 0x%08x\n", (uint)callback_routine);

  if ((callback_t) sdp_packet_callback_test1 != callback_routine)
  {
    io_printf(IO_BUF, "Requested callback: %d, assigned callback: %d\n",
              (uint)(callback_t) sdp_packet_callback_test1, (uint)callback_routine);
    rt_error (RTE_API);
  }

  spin1_callback_off(SDP_PACKET_RX);
}

void sender3() { }

void test3() { }



void setup_sender4() { }

void setup_test4()
{
  callback_t callback_routine;
  int base_port = 0;

  spin1_sdp_callback_on(base_port, sdp_packet_callback_port0, 0);
  spin1_sdp_callback_on(base_port, sdp_packet_callback_port1, 0);

  callback_routine = sdp_callback[base_port].cback;
  io_printf (IO_BUF, "callback SDP address: 0x%08x\n", (uint)callback_routine);

  if ((callback_t) sdp_packet_callback_port1 != callback_routine)
  {
    io_printf(IO_BUF, "Requested callback: %d, assigned callback: %d\n",
              (uint)(callback_t) sdp_packet_callback_port1, (uint)callback_routine);
    rt_error (RTE_API);
  }

  spin1_sdp_callback_off(base_port);
}

void sender4() { }

void test4() { }



void setup_sender5() { }

void setup_test5()
{
  int base_port = 0;

  spin1_callback_on(SDP_PACKET_RX, sdp_packet_callback_test0, 0);
  spin1_sdp_callback_on(base_port, sdp_packet_callback_port0, 0);

  //this should never be printed
  io_printf(IO_BUF, "Test unsuccessful\n");
}

void sender5() { }

void test5() { }



void setup_sender6() { }

void setup_test6()
{

  int base_port = 0;

  spin1_sdp_callback_on(base_port, sdp_packet_callback_port0, 0);
  spin1_callback_on(SDP_PACKET_RX, sdp_packet_callback_test0, 0);

  //this should never be printed
  io_printf(IO_BUF, "Test unsuccessful\n");
}

void sender6() { }

void test6() { }



void setup_sender7()
{
  char *data;

  msg.length = 8 + sizeof(sdp_msg_t);
  msg.flags = 0x7;
  msg.tag = 0;
  msg.dest_port = 0;
  msg.srce_port = (1 << 5) | spin1_get_core_id();
  msg.dest_addr = 0;
  msg.srce_addr = spin1_get_chip_id();
  data = (char*) &(msg.cmd_rc);

  spin1_memcpy(data, TEST_MESSAGE, TEST_MESSAGE_LEN);
}

void setup_test7()
{
  spin1_callback_on(SDP_PACKET_RX, sdp_packet_callback, 0);
}

void sender7()
{
  uint time = spin1_get_simulation_time();

  if (time >= 2 && time < 9)
  {
    msg.dest_port = ((time - 1) << 5) | 1; //send to destination core 1 on port 1
    spin1_send_sdp_msg(&msg, 1);
  }
}

void test7() { }



void setup_sender8()
{
  char *data;

  msg.length = 8 + sizeof(sdp_msg_t);
  msg.flags = 0x7;
  msg.tag = 0;
  msg.dest_port = (1 << 5) | 1; //send to destination core 1 on port 1
  msg.srce_port = (1 << 5) | spin1_get_core_id();
  msg.dest_addr = 0;
  msg.srce_addr = spin1_get_chip_id();
  data = (char*) &(msg.cmd_rc);

  spin1_memcpy(data, TEST_MESSAGE, TEST_MESSAGE_LEN);
}

void setup_test8()
{
  spin1_sdp_callback_on(1, sdp_port_packet_callback_1, 0);
}

void sender8()
{
  uint time = spin1_get_simulation_time();

  if (time == 2)
  {
    msg.dest_port = (1 << 5) | 1; //send to destination core 1 on port 1
    spin1_send_sdp_msg(&msg, 1);
  }
}

void test8() { }



void setup_sender9()
{
  char *data;

  msg.length = 8 + sizeof(sdp_msg_t);
  msg.flags = 0x7;
  msg.tag = 0;
  msg.dest_port = (1 << 5) | 1; //send to destination core 1 on port 1
  msg.srce_port = (1 << 5) | spin1_get_core_id();
  msg.dest_addr = 0;
  msg.srce_addr = spin1_get_chip_id();
  data = (char*) &(msg.cmd_rc);

  spin1_memcpy(data, TEST_MESSAGE, TEST_MESSAGE_LEN);
}

void setup_test9()
{
  spin1_sdp_callback_on(2, sdp_port_packet_callback_2, 0);
}

void sender9()
{
  uint time = spin1_get_simulation_time();

  if (time == 2)
  {
    msg.dest_port = (1 << 5) | 1; //send to destination core 1 on port 1
    spin1_send_sdp_msg(&msg, 1);
  }
}

void test9() { }



void setup_sender10()
{
  char *data;

  msg.length = 8 + sizeof(sdp_msg_t);
  msg.flags = 0x7;
  msg.tag = 0;
  msg.dest_port = (1 << 5) | 1; //send to destination core 1 on port 1
  msg.srce_port = (1 << 5) | spin1_get_core_id();
  msg.dest_addr = 0;
  msg.srce_addr = spin1_get_chip_id();
  data = (char*) &(msg.cmd_rc);

  spin1_memcpy(data, TEST_MESSAGE, TEST_MESSAGE_LEN);
}

void setup_test10()
{
  spin1_sdp_callback_on(1, sdp_port_packet_callback_1, 0);
  spin1_sdp_callback_on(2, sdp_port_packet_callback_2, 0);
  spin1_sdp_callback_on(3, sdp_port_packet_callback_3, 0);
  spin1_sdp_callback_on(4, sdp_port_packet_callback_4, 0);
  spin1_sdp_callback_on(5, sdp_port_packet_callback_5, 0);
  spin1_sdp_callback_on(6, sdp_port_packet_callback_6, 0);
  spin1_sdp_callback_on(7, sdp_port_packet_callback_7, 0);
}

void sender10()
{
  uint time = spin1_get_simulation_time();

  if (time >= 2 && time < 9)
  {
    msg.dest_port = ((time - 1) << 5) | 1; //send to destination core 1 on port 1
    spin1_send_sdp_msg(&msg, 1);
  }
}

void test10() { }
