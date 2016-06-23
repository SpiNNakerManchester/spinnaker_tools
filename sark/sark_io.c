
//------------------------------------------------------------------------------
//
// sark_io.c	    Simple character I/O library for Spinnaker
//
// Copyright (C)    The University of Manchester - 2009, 2010, 2011
//
// Author           Steve Temple, APT Group, School of Computer Science
//		    Fixed point formats by Paul Richmond (Univ. of Sheffield)
//                                     and Dave Lester (APT Group)
//
// Email            temples@cs.man.ac.uk
//
//------------------------------------------------------------------------------


#include <sark.h>

#define SPINN_IO_FIX

#include <stdarg.h>

//------------------------------------------------------------------------------


typedef struct iobuf
{
  struct iobuf *next;
  uint unix_time;
  uint time_ms;
  uint ptr;
  uchar buf[];
} iobuf_t;


static uint sp_ptr;		// Buffer pointer for 'sprintf'
static uint buf_ptr;		// Buffer pointer for IO_BUF

static sdp_msg_t *io_msg;	// Points to SDP buffer
static iobuf_t   *io_buf;	// Points to SDRAM buffer

static const char hex[] = "0123456789abcdef";

//------------------------------------------------------------------------------

// Initialisation routines for IO_STD and IO_BUF output streams. In both
// cases a buffer is allocated from the heap and initialised. A run time
// error occurs if the allocation fails.

static sdp_msg_t *io_std_init ()
{
  sdp_msg_t *msg = sark_alloc (1, sizeof (sdp_msg_t));

  if (msg == NULL)
    rt_error (RTE_IOBUF);

  msg->flags = 0x07;
  msg->tag = TAG_HOST;

  msg->dest_port = PORT_ETH;     // Take from dbg_addr?
  msg->dest_addr = sv->dbg_addr;

  msg->srce_port = sark.virt_cpu;
  msg->srce_addr = sv->p2p_addr;

  msg->cmd_rc = CMD_TUBE;
  msg->length = 12;              // !! const (SDP header + command word)

  return msg;
}


static iobuf_t *io_buf_init ()
{
  iobuf_t *iobuf = sark_xalloc (sv->sys_heap,
				sizeof (iobuf_t) + sv->iobuf_size,
				0,
				1);
  if (iobuf == NULL)
    rt_error (RTE_IOBUF);

  iobuf->unix_time = sv->unix_time;
  iobuf->time_ms = sv->time_ms;
  iobuf->next = NULL;
  iobuf->ptr = buf_ptr = 0;

  return iobuf;
}


//------------------------------------------------------------------------------

// Routine to put a character to an output stream. Different behaviour
// for four possible streams - IO_STD, IO_BUF, IO_NULL and "sprintf" strings.
// For IO_STD and IO_BUF the character is placed in a holding buffer which
// is flushed on buffer-full or newline or NULL.

void io_put_char (char *stream, uint c)
{
  if (stream > IO_NULL)
    {
      stream[sp_ptr++] = c;
      stream[sp_ptr] = 0;
    }

  else if (stream == IO_STD)
    {
      if (io_msg == NULL)
	io_msg = io_std_init ();

      uchar *buf = (uchar *) &io_msg->flags;	// Point at start of msg

      buf[io_msg->length++] = c;		// Insert char at end

      if (c == '\n' || c == 0 || io_msg->length == SDP_BUF_SIZE + 12) // !! const
	{
	  sark_msg_send (io_msg, 10); 	// !! const
	  io_msg->length = 12; 		// !! const
	}
    }

  else if (stream == IO_BUF)
    {
      if (io_buf == NULL)
	io_buf = sark.vcpu->iobuf = io_buf_init ();

      io_buf->buf[buf_ptr++] = c;

      if (buf_ptr == sv->iobuf_size)
	{
	  io_buf->ptr = buf_ptr;
	  io_buf->next = io_buf_init ();
	  io_buf = io_buf->next;
	}

      else if (c == '\n' || c == 0)
	io_buf->ptr = buf_ptr;
    }
}


//------------------------------------------------------------------------------

// Routines to deal with the various printf formats. All end up calling
// "io_put_char" above.

// Put a string possibly left-padding with spaces.

static void io_put_str (char *stream, char *s, int d)
{
  char *t = s;
  int n = 0;

  while (*t++)
    n++;

  while (d-- > n)
    io_put_char (stream, ' ');

  while (*s)
    io_put_char (stream, *s++);
}


// Put an integer (signed) in given field width, left-padding with
// spaces. Entire field is assumed to fit in 16 chars!.

static void io_put_int (char *stream, int n, uint d, uint pad) // pad not used!
{
  char s[16];
  int i = 0;
  uint neg = 0;

  if (n < 0)
    {
      n = -n;
      neg = '-';
    }

  while (1)
    {
      divmod_t r = sark_div10 (n);

      s[i++] = r.mod + '0';
      n = r.div;
      if (n == 0) break;
    }

  while (i > 0 && s[--i] == '0')
    continue;

  if (neg)
    s[++i] = neg;

  while (d-- > i + 1)
    io_put_char (stream, ' ');

  while (i >= 0)
    io_put_char (stream, s[i--]);
}


// Put an integer (unsigned) in given field width, left-padding with
// spaces. Entire field is assumed to fit in 16 chars!.

static void io_put_uint (char *stream, uint n, uint d, uint pad)
{
  char s[16];
  int i = 0;

  while (1)
    {
      divmod_t r = sark_div10 (n);

      s[i++] = r.mod + '0';
      n = r.div;
      if (n == 0) break;
    }

  while (i > 0 && s[--i] == '0')
    continue;

  while (d-- > i + 1)
    io_put_char (stream, pad);

  while (i >= 0)
    io_put_char (stream, s[i--]);
}


// Put a hex number in exactly the number of characters specified.
// Truncates high digits if need be.

static void io_put_zhex (char *stream, uint n, uint d)
{
  for (int i = d - 1; i >= 0; i--)
    io_put_char (stream, hex [(n >> (4 * i)) & 15]);
}


// Put an integer in hex in given field width, left-padding with
// spaces. Entire field is assumed to fit in 16 chars!.

static void io_put_hex (char *stream, uint n, uint d, uint pad)
{
  char s[16];
  int i = 0;

  while (1)
    {
      s[i++] = hex[n & 15];
      n = n >> 4;
      if (n == 0) break;
    }

  while (i > 0 && s[--i] == '0')
    continue;

  while (d-- > i + 1)
    io_put_char (stream, pad);

  while (i >= 0)
    io_put_char (stream, s[i--]);
}


//------------------------------------------------------------------------------

// Legacy formats no longer useful...

#ifdef SPINN_IO_NET
static void io_put_mac (char *stream, uchar *s)
{
  for (uint i = 0; i < 6; i++)
    {
      io_put_zhex (stream, s[i], 2);
      if (i != 5)
	io_put_char (stream, ':');
    }
}


static void io_put_ip (char *stream, uchar *s)
{
  for (uint i = 0; i < 4; i++)
    {
      io_put_int (stream, s[i], 0, 0);
      if (i != 3)
	io_put_char (stream, '.');
    }
}
#endif


//------------------------------------------------------------------------------

// Format for 16.16 fixed point by Paul Richmond and Dave Lester

#ifdef SPINN_IO_FIX
static
void io_put_fixed (char *stream, uint n, uint d, uint a, uint pad, int neg)
{
  char s[25];
  int i = 0;
  uint f = 0;
  int c = 0;

  // fractional part

  f = n;
  if (a > 12) // maximum precision of 12 to prevent overflow
    a = 12;

  while (i < a)
    {
      f &= 0x0000ffff;
      f *= 10;
      s[a - ++i] = (f >> 16) + '0';
    }

  //set carry for rounding

  f &= 0x0000ffff;
  f *= 10;
  c = (f >> 16) > 4;

  //carry any rounding

  f = 0;
  while ((c) && (f < a))
    {
      if (s[f] >= '9')
	{
	  s[f++] = '0';
	}
      else
	{
	  s[f++]++;
	  c=0;
	}
    }

  // add decimal

  if (a)
    s[i++] = '.';

  // integer part

  n = (n >> 16) + c; // add the carry

  while (1)
    {
      divmod_t r = sark_div10 (n);

      s[i++] = r.mod + '0';
      n = r.div;
      if (n == 0)
	break;
    }

  if (neg) {  // <drl add>
    s[i++] = '-';
  }  // </drl add>

  while (d-- > (i+1))
    io_put_char (stream, pad);

  while (i > 0)
    io_put_char (stream, s[--i]);
}

 // <drl add>
static void io_put_sfixed (char *stream, int n, uint d, uint a, uint pad)
{ io_put_fixed (stream, (uint)((n < 0)? (-n): n) << 1, d, a, pad, (n < 0)); }
static void io_put_ufixed (char *stream, uint n, uint d, uint a, uint pad)
{ io_put_fixed (stream, n, d, a, pad, (1==0)); }
 // </drl add>

#endif


//------------------------------------------------------------------------------

// Main "printf" routine. The first argument is a 'stream' which is either
// a constant IO_BUF, IO_STD, IO_DBG, IO_NULL or a pointer to a character
// array which will be filled in as in "sprintf". No checks for buffer overflow
// are made for this latter case!

// Formats are
// %c - character
// %s - string - can be eg %8s for right justification
// %d - signed integer (also %8d, %06d, etc)
// %u - signed integer (also %4u, %06u, etc)
// %x - integer in hex (also %4x, %08x, etc)
// %4z - integer in hex, exactly N digits
// %n.mf -  16.16 (Paul Richmond's) fixed point with width n, precision m
// %n.mr -  s0.15 ISO fract fixed point with width n, precision m
// %n.mR -   0.16 ISO unsigned fract fixed point with width n, precision m
// %n.mk - s16.15 ISO accum fixed point with width n, precision m
// %n.mK -  16.16 ISO unsigned accum fixed point with width n, precision m

void io_printf (char *stream, char *f, ...) 
{
  va_list ap;
 
  if (stream == IO_NULL)
    return;

  if (stream > IO_NULL)
    sp_ptr = stream[0] = 0;

  va_start (ap, f);

  while (1)
    {
      char c = *f++;

      if (c == 0)
	return;

      if (c != '%')
	{
	  io_put_char (stream, c);
	  continue;
	}

      c = *f++;

      if (c == 0)
	return;

      char pad = ' ';

      if (c == '0')
	pad = c;

      uint size = 0;

      while (c >= '0' && c <= '9')
	{
	  size = 10 * size + c - '0';
	  c = *f++;
	  if (c == 0)
	    return;
	}

#ifdef SPINN_IO_FIX
      uint precision = 6;

      if (c == '.')
	{
	  precision = 0;
	  c = *f++;

	  while (c >= '0' && c <= '9')
	    {
	      precision = 10 * precision + c - '0';
	      c = *f++;
	    }

	  if (c == 0)
	    return;
	}
#endif

#ifdef SPINN_IO_NET
      uint t;
#endif

      switch (c)
	{
	case 'c':
	  io_put_char (stream, va_arg (ap, uint));
	  break;

	case 's':
	  io_put_str (stream, va_arg (ap, char *), size);
	  break;

	case 'd':
	  io_put_int (stream, va_arg (ap, int), size, pad);
	  break;

	case 'u':
	  io_put_uint (stream, va_arg (ap, uint), size, pad);
	  break;
#ifdef SPINN_IO_FIX
        case 'f': // Paul Richmond's FP format (u1616)
// <drl add>
        case 'K': // ISO unsigned accum (u1616)
	  io_put_ufixed
	    (stream, va_arg (ap, uint), size, precision, pad);
	  break;
        case 'r': // ISO signed fract (s015)
	  io_put_sfixed (stream, va_arg (ap, int), size, precision, pad);
	  break;
        case 'R': // ISO unsigned fract (u016)
	  io_put_ufixed (stream, va_arg (ap, uint), size, precision, pad);
	  break;
        case 'k': // ISO signed accum (s1615)
	  io_put_sfixed (stream, va_arg (ap, int), size, precision, pad);
	  break;
// </drl add>
#endif
	case 'x': // hex, digits as needed
	  io_put_hex (stream, va_arg (ap, uint), size, pad);
	  break;

	case 'z': // zero prefixed hex, exactly "size" digits
	  io_put_zhex (stream, va_arg (ap, uint), size);
	  break;
#ifdef SPINN_IO_NET
	case 'h': // pointer to network short (hex)
	  t = va_arg (ap, uint);
	  t = * (ushort *) t;
	  io_put_zhex (stream, ntohs (t), size);
	  break;

	case 'i': // pointer to network short (decimal)
	  t = va_arg (ap, uint);
	  t = * (ushort *) t;
	  io_put_uint (stream, ntohs (t), size, pad);
	  break;

	case 'p': // pointer to IP address
	  io_put_ip (stream, va_arg (ap, uchar *));
	  break;

	case 'm': // pointer to MAC address
	  io_put_mac (stream, va_arg (ap, uchar *));
	  break;
#endif
	default:
	  io_put_char (stream, c);
	}
    }
  //  va_end (ap);
}
