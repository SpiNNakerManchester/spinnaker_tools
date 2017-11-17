// ----------------------------------------------------------------------------
// drawer.c
// Andrew Webb (webb@cs.man.ac.uk)
// August 2012
// ----------------------------------------------------------------------------


#include <stdio.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <GL/freeglut.h>
#ifdef WIN32
#include <windows.h>
#include <ws2tcpip.h>
#endif

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) < (b)) ? (b) : (a))

extern void* input_thread(void *ptr);
extern void init_udp_server_spinnaker(void);

int frameWidth, frameHeight;
int windowWidth, windowHeight;
unsigned char *viewingFrame;
unsigned int *receivedFrame;


typedef struct {
    float x, y, z;
} Vector3;

Vector3 position = {-220.0, 50.0, 0.0};
Vector3 look = {1.0, 0.0, 0.0};
Vector3 up = {0.0, 1.0, 0.0};

int moving = 0;
int strafing = 0;
int turningLeftRight = 0;
int turningUpDown = 0;
int rolling = 0;

float moveAmount = 0.00003;
float turnAmount = 0.0000003;

float verticalFieldOfView = 50.0;
float horizontalFieldOfView = 60.0;


// Called every time OpenGL needs to update the display

void display(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.001);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glDrawPixels(windowWidth, windowHeight, GL_RGB, GL_UNSIGNED_BYTE, viewingFrame);
    glutSwapBuffers();
}


void reshape(
	int width,
	int height)
{
    windowWidth = MIN(width, frameWidth);
    windowHeight = MIN(height, frameHeight);
    glViewport(0, 0, (GLsizei) width, (GLsizei) height);
    glLoadIdentity();
}


void special(
	int key,
	int x,
	int y)
{
    switch (key) {
    case GLUT_KEY_UP:
	turningUpDown = -1;
	break;
    case GLUT_KEY_DOWN:
	turningUpDown = 1;
	break;
    case GLUT_KEY_RIGHT:
	rolling = -1;
	break;
    case GLUT_KEY_LEFT:
	rolling = 1;
	break;
    }
}


void specialUp(
	int key,
	int x,
	int y)
{
    switch (key) {
    case GLUT_KEY_UP:
	turningUpDown = 0;
	break;
    case GLUT_KEY_DOWN:
	turningUpDown = 0;
	break;
    case GLUT_KEY_RIGHT:
	rolling = 0;
	break;
    case GLUT_KEY_LEFT:
	rolling = 0;
	break;
    }
}


void keyPressed(
	unsigned char key,
	int x,
	int y)
{
    switch (key) {
    case 'w':
	moving = 1;
	break;
    case 's':
	moving = -1;
	break;
    case 'a':
	turningLeftRight = -1;
	break;
    case 'd':
	turningLeftRight = 1;
	break;
    case 'q':
	strafing = 1;
	break;
    case 'e':
	strafing = -1;
	break;
    }
}


void keyReleased(
	unsigned char key,
	int x,
	int y)
{
    switch (key) {
    case 'w':
	moving = 0;
	break;
    case 's':
	moving = 0;
	break;
    case 'a':
	turningLeftRight = 0;
	break;
    case 'd':
	turningLeftRight = 0;
	break;
    case 'q':
	strafing = 0;
	break;
    case 'e':
	strafing = 0;
	break;
    }
}


Vector3 fVectorNormalise(
	Vector3 in)
{
    float magnitudeReciprocal = 1.0 / sqrt(in.x*in.x + in.y*in.y + in.z*in.z);
    Vector3 result = {
	    in.x * magnitudeReciprocal,
	    in.y * magnitudeReciprocal,
	    in.z * magnitudeReciprocal};
    return result;
}


Vector3 fVectorCrossProduct(
	Vector3 a,
	Vector3 b)
{
    Vector3 result = {
	    a.y * b.z - a.z * b.y,
	    a.z * b.x - a.x * b.z,
	    a.x * b.y - a.y * b.x};
    return result;
}


// Rotate the first vector around the second

Vector3 fVectorRotate(
	Vector3 rotated,
	Vector3 rotateAbout,
	float amount)
{
    float c = cos(amount);
    float s = sin(amount);
    float t = 1 - cos(amount);

    Vector3 result;

    result.x = rotated.x * (t * rotateAbout.x * rotateAbout.x + c) +
	    rotated.y * (t * rotateAbout.x * rotateAbout.y + s * rotateAbout.z) +
	    rotated.z * (t * rotateAbout.x * rotateAbout.z - s * rotateAbout.y);

    result.y = rotated.x * (t * rotateAbout.x * rotateAbout.y - s * rotateAbout.z) +
	    rotated.y * (t * rotateAbout.y * rotateAbout.y + c) +
	    rotated.z * (t * rotateAbout.y * rotateAbout.z + s * rotateAbout.x);

    result.z = rotated.x * (t * rotateAbout.z * rotateAbout.x + s * rotateAbout.y) +
	    rotated.y * (t * rotateAbout.y * rotateAbout.z - s * rotateAbout.x) +
	    rotated.z * (t * rotateAbout.z * rotateAbout.z + c);

    result = fVectorNormalise(result);
    return result;
}


void calculateMovement(
	int timestep)
{
    // Forward movement

    position.x += look.x * timestep * moveAmount * moving;
    position.y += look.y * timestep * moveAmount * moving;
    position.z += look.z * timestep * moveAmount * moving;

    Vector3 right = fVectorCrossProduct(up, look);

    // Strafing movement

    position.x += right.x * timestep * moveAmount * strafing;
    position.y += right.y * timestep * moveAmount * strafing;
    position.z += right.z * timestep * moveAmount * strafing;

    // To turn left/right, rotate the look vector around the up vector

    look = fVectorRotate(look, up, timestep * turnAmount * turningLeftRight);

    // To turn up/down, rotate the look vector and up vector about the right vector

    look = fVectorRotate(look, right, timestep * turnAmount * turningUpDown);
    up = fVectorRotate(up, right, timestep * turnAmount * turningUpDown);

    // To roll, rotate the up vector around the look vector

    up = fVectorRotate(up, look, timestep * turnAmount * rolling);
}


clock_t prevTime;

void idleFunctionLoop(void)
{
    //reshape(frameWidth, frameHeight);

    clock_t currTime = clock();

    // Calculate movement ten times a second

    if (currTime > prevTime + 10000) {
	int timeSinceLastUpdate = currTime - prevTime;
	calculateMovement(timeSinceLastUpdate);

	prevTime = currTime;
	display();
    }
}


int main(
	int argc,
	char **argv)
{

#ifdef WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) {
	fprintf(stderr, "WSAStartup failed.\n");
	exit(1);
    }
#endif

    pthread_t p1;			// this sets up the thread that can come back to here from type

    init_udp_server_spinnaker();	//initialization of the port for receiving SpiNNaker frames

    frameHeight = (argc > 1 ? atoi(argv[1]) : 256);
    frameWidth = (int) (((int) horizontalFieldOfView * frameHeight) / verticalFieldOfView);

    prevTime = clock();
    viewingFrame = (unsigned char*) malloc(sizeof(unsigned char) * frameWidth * frameHeight * 3);

    receivedFrame = (unsigned int*) malloc(sizeof(unsigned int) * frameWidth * frameHeight);

    int i;
    for (i = 0; i < frameWidth * frameHeight; i++) {
	receivedFrame[i] = 0;
    }

    pthread_create(&p1, NULL, input_thread, NULL); // away it goes

    glutInit(&argc, argv);		// Initialise OpenGL
    glutInitDisplayMode(GLUT_DOUBLE);	// Set the display mode
    glutInitWindowSize(frameWidth, frameHeight); // Set the window size
    glutInitWindowPosition(0, 0);	// Set the window position
    glutCreateWindow("Path Tracer");	// Create the window

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);		// Register the "display" function
    glutReshapeFunc(reshape);		// Register the "reshape" function
    glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
    glutKeyboardFunc(keyPressed);
    glutKeyboardUpFunc(keyReleased);
    glutIdleFunc(idleFunctionLoop);

    glutMainLoop();			// Enter the main OpenGL loop

    return 0;
}
