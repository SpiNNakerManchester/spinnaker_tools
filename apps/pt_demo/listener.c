// ----------------------------------------------------------------------------
// listener.c
// Andrew Webb (webb@cs.man.ac.uk)
// August 2012
// ----------------------------------------------------------------------------

#ifndef WIN32
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#else
#include <windows.h>
#include <ws2tcpip.h>
#define bzero(b, len)		(memset((b), '\0', (len)), (void) 0)
#define bcopy(b1, b2, len)	(memmove((b2), (b1), (len)), (void) 0)
#define close(sock)

#define SHUT_RD   SD_RECEIVE
#define SHUT_WR   SD_SEND
#define SHUT_RDWR SD_BOTH

typedef unsigned int uint;
typedef unsigned short ushort;
#endif

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <time.h>


// Port to receive SpiNNaker packets

#define INPUT_PORT_SPINNAKER 17894


// Variables from drawer.c

extern int frameWidth, frameHeight;
extern unsigned char *viewingFrame;
extern unsigned int *receivedFrame;


int packet_number = 0;

// Stop alignment in structure: word alignment would be nasty here,
// byte alignment reqd

#pragma pack(1)

// A structure that holds SpiNNaker packet data (inside UDP segment)

struct sdp_msg {
    unsigned short  reserved;
    unsigned char   flags;
    unsigned char   ip_tag;
    unsigned char   destination_port;
    unsigned char   source_port;
    unsigned short  destination_chip;
    unsigned short  source_chip;
    unsigned short  command;
    unsigned short  sequence;
    unsigned int    arg1;
    unsigned int    arg2;
    unsigned int    arg3;
    unsigned char   data;
};


// Global variables for SpiNNaker packet receiver

int sockfd_input;
char portno_input[6];
struct addrinfo hints_input, *servinfo_input, *p_input;
struct sockaddr_storage their_addr_input;
socklen_t addr_len_input;
int rv_input;
int numbytes_input;
struct sdp_msg * scanptr;
unsigned char buffer_input[1515];  //buffer for network packets

// End of variables for SpiNNaker packet receiver - some could be local
// really - but with pthread they may need to be more visible


// prototypes for functions below
void init_udp_server_spinnaker(void);
void* input_thread(void *ptr);
// end of prototypes


// Setup socket for SpiNNaker frame receiving on port 54321 (or per
// define as above)

void init_udp_server_spinnaker(void)
{
    snprintf(portno_input, 6, "%d", INPUT_PORT_SPINNAKER);

    bzero(&hints_input, sizeof(hints_input));
    hints_input.ai_family = AF_INET; // set to AF_INET to force IPv4
    hints_input.ai_socktype = SOCK_DGRAM; // type UDP (socket datagram)
    hints_input.ai_flags = AI_PASSIVE; // use my IP

    rv_input = getaddrinfo(NULL, portno_input, &hints_input, &servinfo_input);
    if (rv_input != 0) {
	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv_input));
	exit(1);
    }

    // loop through all the results and bind to the first we can

    for (p_input = servinfo_input; p_input != NULL; p_input = p_input->ai_next) {
	sockfd_input = socket(p_input->ai_family, p_input->ai_socktype,
		p_input->ai_protocol);
	if (sockfd_input == -1) {
	    perror("SpiNNaker listener: socket");
	    continue;
	}

	if (bind(sockfd_input, p_input->ai_addr, p_input->ai_addrlen) == -1) {
	    close(sockfd_input);
	    perror("SpiNNaker listener: bind");
	    continue;
	}
	break;
    }

    if (p_input == NULL) {
	fprintf(stderr, "SpiNNaker listener: failed to bind socket\n");
	exit(-1);
    }

    freeaddrinfo(servinfo_input);
}


// Helper function for input_thread() below
static inline void process_one_message(
	char *data,
	int numberOfPixels)
{
#define UC(value)	((unsigned char) (value))
#define UI(value)	((unsigned int) (value))
#define ULL(value)	((unsigned long long) (value))

    int i;
    for (i = 0; i < numberOfPixels; i++) {
	unsigned char x1 = UC(data[i*7 + 0]);
	unsigned char x2 = UC(data[i*7 + 1]);
	unsigned char y1 = UC(data[i*7 + 2]);
	unsigned char y2 = UC(data[i*7 + 3]);
	unsigned int r   = UI(data[i*7 + 4]);
	unsigned int g   = UI(data[i*7 + 5]);
	unsigned int b   = UI(data[i*7 + 6]);
	int x = (((int)x1)<<8) + ((int)x2);
	int y = (((int)y1)<<8) + ((int)y2);

	if (((frameHeight-y-1)*frameWidth + x)*3 + 2 < frameWidth*frameHeight*3 - 1) {
	    int index = (frameHeight-y-1)*frameWidth + x;
	    unsigned long long receivedFrames = ULL(receivedFrame[index]);

#define EXTRACT(factor, base, index) \
  UC((ULL(UC(base)) + (factor)*ULL(viewingFrame[index])) / ((factor) + 1))

	    viewingFrame[index*3]   = EXTRACT(receivedFrames, r, index*3);
	    viewingFrame[index*3+1] = EXTRACT(receivedFrames, g, index*3+1);
	    viewingFrame[index*3+2] = EXTRACT(receivedFrames, b, index*3+2);

	    //viewingFrame[index*3]   = (unsigned char) r;
	    //viewingFrame[index*3+1] = (unsigned char) g;
	    //viewingFrame[index*3+2] = (unsigned char) b;

	    receivedFrame[index] += 1;
	}
    }
}


void* input_thread(
	void *ptr)
{
    printf("Drawer running (port %d)...\n", INPUT_PORT_SPINNAKER);

    while (1) {
	int numAdditionalBytes = 0;

	addr_len_input = sizeof(their_addr_input);

	numbytes_input = recvfrom(sockfd_input, buffer_input, 1514, 0,
		(struct sockaddr *) &their_addr_input, &addr_len_input);
	if (numbytes_input == -1) {
	    // Error getting the input frame off the Ethernet
	    perror((char*) "error recvfrom");
	    exit(-1);
	}

	scanptr = (struct sdp_msg *) buffer_input;
	if ((int) scanptr->command == 3) {
	    process_one_message((char*) &scanptr->data, (int) scanptr->arg1);
        }

	packet_number++;

	fflush(stdout);
    }
}
