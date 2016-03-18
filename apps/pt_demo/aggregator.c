// ----------------------------------------------------------------------------
// aggregator.c
// Andrew Webb (webb@cs.man.ac.uk)
// August 2012
// ----------------------------------------------------------------------------
//
// The aggregator runs on chip (0,0), processor 1 and has two jobs.
// First, it constructs an array of SpiNNaker processors that are alive and
// ready, and gives each a subset of the image to trace.
//
// Then it collects MC packets from those processors, each containing a pixel
// location and RGB values.  It bundles these into SDP messages and sends
// them on to the host.
//
// ----------------------------------------------------------------------------

#define API	// Undef to use SARK event library

#include <sark.h>
#include <spin1_api.h>


#define SDP_HEADER_SIZE (24)

const uint firstByte  = 0xff;
const uint secondByte = 0xff00;
const uint thirdByte  = 0xff0000;
const uint fourthByte = 0xff000000;

sdp_msg_t pixelMessage;
sdp_msg_t statusMessage;
sdp_msg_t traceMessage;

int pixel;

int ***nodes;

int camx, camy, camz, lookx, looky, lookz, upx, upy, upz, width, height;
int hField, vField, antialiasing, xChips, yChips, cores;

unsigned int numberOfNodes = 0;
unsigned int nextNode = 0;

// The only MC packets the aggregator expects contain the RGB values of a pixel
// in the payload and the x and y coords of that pixel in the key and payload.

void mc_packet_callback (uint key, uint payload)
{
  sark.vcpu->user0++;

  // Unpack the x,y coords, RGB values
  pixelMessage.data[pixel*7]   = key&firstByte;                // x1
  pixelMessage.data[pixel*7+1] = ((payload&fourthByte)>>24);   // x2
  pixelMessage.data[pixel*7+2] = ((key&fourthByte)>>24);       // y1
  pixelMessage.data[pixel*7+3] = ((key&thirdByte)>>16);        // y2
  pixelMessage.data[pixel*7+4] = ((payload&thirdByte)>>16);    // r
  pixelMessage.data[pixel*7+5] = ((payload&secondByte)>>8);    // g
  pixelMessage.data[pixel*7+6] = payload&firstByte;            // b

  pixel += 1;

  // We have enough pixels to fill an SDP packet payload
  if (pixel == 36)
    {
      sark.vcpu->user2++;
      pixelMessage.length = SDP_HEADER_SIZE + pixel*7;
      pixelMessage.arg1   = pixel; 
      sark_msg_send(&pixelMessage, 10);

      pixel = 0;
    }
}

int status = 0; // Tells us if we're waiting for status acks or trace acks

// The timer callback handles the handshaking that happens before image tracing
// First it tries to establish communication with all processors to determine
// which are alive and ready to trace an image.
// Then it issues a message to all alive processors telling them to start tracing.
// This is necessarily a two-stage process as all tracing processors need to
// know how many processors are tracing the image and what their own assigned
// ID is in order to determine what subset of the image to work on.

void timer_callback(uint time, uint none)
{
  sark.vcpu->user1++;

  int numberWaitingToHearBackFrom = 0;

  unsigned int i,j,k;
  for (i=0; i<xChips; i++)
    {
      for (j=0; j<yChips; j++)
        {
	  for (k=0; k<cores; k++)
            {
	      // In the array 'nodes':
	      // <=-10 is the initial state - means we haven't heard a status check ack from this node
	      // >=0 means we have heard a status check ack and the node has a global ID
	      
	      // If minimum value in array is 0 (excluding -3) then we've heard back
	      // from all of them and we can start sending trace messages.

	      // -2 means we've heard a trace ack
	      // -3 means ignore this node (used for 0,0,0 and for nodes that haven't responded)
            
	      if (status==0) // Asking nodes their status. We're interested in <=-10 nodes
                {
		  if (nodes[i][j][k]<=-10) // If we haven't heard from the node
                    {
		      if (nodes[i][j][k]<-20) // Cull any nodes we're still waiting for
                        {                       // after many attempts
			  nodes[i][j][k] = -3;
                        }
		      else
                        {
			  numberWaitingToHearBackFrom ++;
			  nodes[i][j][k] -= 1; // Track number of attempts
			  
			  statusMessage.dest_port = (1<<5)|(k+1);
			  statusMessage.dest_addr = (i<<8)+j;

			  sark_msg_send(&statusMessage, 10); // Query status

			  if (numberWaitingToHearBackFrom>50)
                            {   // Only try to send 50 status messages at a time
			      i=10000;
			      j=10000;
			      k=10000;
                            }
                        }
                    }
                }
	      else if (status == 1) // waiting for trace ack. We're interested in >=0 nodes
                {
		  if (nodes[i][j][k]>=0) // if it's a node that we're waiting on a trace ack from
                    {
		      if (nodes[i][j][k]>350000) // Cull any nodes we're still waiting for
                        {                          // after many attempts
			  nodes[i][j][k] = -3;
                        }
		      else
                        {
			  nodes[i][j][k] += 10000; // Track number of attempts

			  numberWaitingToHearBackFrom ++;
                    
			  traceMessage.dest_port = (1<<5)|(k+1);
			  traceMessage.dest_addr = (i<<8)+j;

			  // fill in node ID and number of nodes
			  traceMessage.data[14*4+1] = (unsigned char)(((((unsigned int)nodes[i][j][k])%10000)&secondByte)>>8);
			  traceMessage.data[14*4] = (unsigned char)((((unsigned int)nodes[i][j][k])%10000)&firstByte);
			  traceMessage.data[15*4+1] = (unsigned char)((numberOfNodes&secondByte)>>8);
			  traceMessage.data[15*4] = (unsigned char)(numberOfNodes&firstByte);

			  sark_msg_send(&traceMessage, 10);

			  if (numberWaitingToHearBackFrom>50)
                            {   // Only try to send 50 trace messages at a time
			      i=10000;
			      j=10000;
			      k=10000;
                            }
                        }
                    }

                } // end if waiting for trace ack
            }
        }
    } // End of outermost for loop (x)

  // We've received all ACKs we're expecting

  if (numberWaitingToHearBackFrom == 0)
    {
      // If we had been listening for status query ACK messages, it's now
      // time to start sending trace messages.
      if (status == 0)
        {
	  status = 1; // Now we're waiting for trace ACKs
	  io_printf(IO_BUF, "Recruited %d nodes.\n", numberOfNodes);
        }
      // If we had been listening for trace ACKs, we now know that all nodes
      // are tracing the image.
      else if (status == 1)
        {
	  status = 2;
	  // There's no longer any need to listen for SDP packets.

#ifdef API
	  spin1_callback_off(TIMER_TICK);
	  spin1_callback_off(SDP_PACKET_RX);
#else
	  event_enable (EVENT_TIMER, 0);
	  event_enable (EVENT_SDP, 0);
#endif
	  io_printf(IO_BUF, "All nodes are now tracing.\n");
        }
    }
}

// The aggregator receives three types of SDP message
// 1: A message from the host, carrying the position of the camera, that
// triggers the tracing of an image.
// 2: An ACK from SpiNNaker processors in response to a status query message
// 3: An ACK from SpiNNaker processors in response to a trace image message

void sdp_packet_callback(uint msg, uint port)
{
  sdp_msg_t *sdp_msg = (sdp_msg_t *) msg;

  io_printf (IO_BUF, "SDP cmd %d, port %d\n", sdp_msg->cmd_rc, port);

  if (sdp_msg->cmd_rc == 4) // 1. it's a trace message from the host
    {
      io_printf(IO_BUF, "Received trigger message from host PC.\n");
      int i;
      for (i=0; i<(sdp_msg->length-24)/4; i++)
        {
	  int var = 0;
	  var += (sdp_msg->data[i*4]<<0);
	  var += (sdp_msg->data[i*4+1]<<8);
	  var += (sdp_msg->data[i*4+2]<<16);
	  var += (sdp_msg->data[i*4+3]<<24);    

	  traceMessage.length         = sdp_msg->length;
	  traceMessage.data[i*4]      = sdp_msg->data[i*4];
	  traceMessage.data[i*4+1]    = sdp_msg->data[i*4+1];
	  traceMessage.data[i*4+2]    = sdp_msg->data[i*4+2];
	  traceMessage.data[i*4+3]    = sdp_msg->data[i*4+3];

	  // The payload carries...
	  switch(i)
            {
	      // The position and orientation of the camera
	    case 0: camx = var; break;
	    case 1: camy = var; break;
	    case 2: camz = var; break;
	    case 3: lookx = var; break;
	    case 4: looky = var; break;
	    case 5: lookz = var; break;
	    case 6: upx = var; break;
	    case 7: upy = var; break;
	    case 8: upz = var; break;
	      // The dimensions of the image and the field of view
	    case 9: width = var; break;
	    case 10: height = var; break;
	    case 11: hField = var; break;
	    case 12: vField = var; break;
	      // The antialiasing factor
	    case 13: antialiasing = var; break;
	      // The width and height of the grid of chips to try to recruit
	      // Now worked out locally
	    case 14: break; //xChips = var; break;
	    case 15: break; //yChips = var; break;
	      // The number of cores on each chip to try to recruit
	    case 16: cores = var; break;
            }
        }

      xChips = (sv->p2p_dims >> 8) & 0xFF;
      yChips = (sv->p2p_dims >> 0) & 0xFF;

      traceMessage.data[14*4+2] = 0;
      traceMessage.data[14*4+3] = 0;
      traceMessage.data[15*4+2] = 0;
      traceMessage.data[15*4+3] = 0;

      // Construct an array representing processors on the SpiNNaker board

      nodes = (int***) sark_xalloc (sv->sdram_heap, xChips * sizeof(int**), 0, ALLOC_LOCK);

      for (int i = 0; i < xChips; i++)
        {
	  nodes[i] = (int**) sark_xalloc (sv->sdram_heap, yChips * sizeof(int*), 0, ALLOC_LOCK);
	  for (int j = 0; j < yChips; j++)
	    nodes[i][j] = (int*) sark_xalloc (sv->sdram_heap, cores * sizeof(int), 0, ALLOC_LOCK);
        }

      // Initialise the nodes' statuses
      // <-10 means 'not received' anything
      // >=0 indicates the node has an ID
      // -2 means node is tracing an image
      // -3 means ignore this node

      for (int i = 0; i < xChips; i++)
        {
	  for (int j = 0; j < yChips; j++)
            {
	      for (int k = 0; k < cores; k++)
                {
		  if (i + j + k == 0)
		    nodes[i][j][k] = -3;
		  else
		    nodes[i][j][k] = -10;
                }
            }
        }

      // The timer callback deals with handshaking, which is now required
#ifdef API
      spin1_callback_on(TIMER_TICK, timer_callback, 3);
#else
      event_enable (EVENT_TIMER, 1);
#endif
    }

  // 2. a status ACK when we're waiting for them
  else if (sdp_msg->cmd_rc == 7 && status == 0)
    {
      int processor = (sdp_msg->srce_port) - (1<<5) - 1;
      int x = ((sdp_msg->srce_addr&secondByte)>>8);
      int y = (sdp_msg->srce_addr&firstByte);
      // If we hadn't already heard back from this node
      if (nodes[x][y][processor] <= -10)
        {
	  // Give the node a global ID, increment the number of nodes
	  nodes[x][y][processor] = nextNode;
	  nextNode ++;
	  numberOfNodes = nextNode;
        }
    }

    // a trace ack when we're waiting for them
  else if (sdp_msg->cmd_rc == 5 && status == 1) 
    {
      int processor = (sdp_msg->srce_port) - (1<<5) - 1;
      int x = ((sdp_msg->srce_addr&secondByte)>>8);
      int y = (sdp_msg->srce_addr&firstByte);
      // Set the node status to 'tracing'
      nodes[x][y][processor] = -2;
    }

  sark_msg_free(sdp_msg);
}


void c_main()
{   
  io_printf (IO_BUF, "Started aggregator\n");

  sark.vcpu->user0 = sark.vcpu->user1 = sark.vcpu->user2 = sark.vcpu->user3 = 0;

  pixelMessage.flags          = 0x07;
  pixelMessage.tag            = 1;
  pixelMessage.dest_port      = PORT_ETH;
  pixelMessage.srce_port      = (1<<5) | sark_core_id();
  pixelMessage.dest_addr      = sv->eth_addr;
  pixelMessage.srce_addr      = sark_chip_id();
  pixelMessage.cmd_rc         = 3; // Pixel message*/

  statusMessage.flags          = 0x07;
  statusMessage.srce_port      = (1<<5) | sark_core_id();
  statusMessage.srce_addr      = sark_chip_id();
  statusMessage.length         = SDP_HEADER_SIZE;
  statusMessage.cmd_rc         = 6; // Status check message*/

  traceMessage.flags          = 0x07;
  traceMessage.srce_port      = (1<<5) | sark_core_id();
  traceMessage.srce_addr      = sark_chip_id();
  traceMessage.cmd_rc         = 4; // Trace message*/

  pixel = 0;

#ifdef API
  spin1_set_timer_tick (10000);
  spin1_callback_on (SDP_PACKET_RX, sdp_packet_callback, 1);
  spin1_callback_on (MCPL_PACKET_RECEIVED, mc_packet_callback, 2);

  spin1_start (SYNC_NOWAIT);
#else
  event_register_queue (sdp_packet_callback, EVENT_SDP, SLOT_1, PRIO_0);
  event_register_queue (timer_callback, EVENT_TIMER, SLOT_2, PRIO_1);
  event_register_queue (mc_packet_callback, EVENT_RXPKT, SLOT_3, PRIO_2);
  event_enable (EVENT_TIMER, 0);

  event_start (10000, 0, SYNC_NOWAIT);
#endif
}
