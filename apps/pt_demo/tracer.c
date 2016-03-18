// ----------------------------------------------------------------------------
// tracer.c
// Andrew Webb (webb@cs.man.ac.uk)
// August 2012
// ----------------------------------------------------------------------------

#define API	// Undef to use SARK event library

#include <sark.h>
#include <spin1_api.h>


#define SDP_HEADER_SIZE (24)

#define FP_PI       205824

// arithmetic.c
int lower16 = (1<<16)-1;
int upper16 = ~((int)((1<<16)-1));
int lower8  = (1<<8)-1;
int upper8  = ~((int)((1<<8)-1));

// Only look at bits 15:8 for control
int multicastControlMask = ((1<<16)-1)^((1<<8)-1);

// key for packets to be routed to chip (0,0), processor 1 (pixel packets)
int proc001Key = (1<<10)+(1<<9);

const uint firstByte  = 0xff;
const uint secondByte = 0xff00;
const uint thirdByte  = 0xff0000;
const uint fourthByte = 0xff000000;



int abs(int a)
{
    return (a<0 ? -a : a);
}


// All integers represent fixed point numbers with 16 bits for the integer
// part and 16 bits for the fractional

// Note: There are multiple multiplication functions to take advantage of
// the users knowledge about the numbers being multiplied. E.g. If an operand
// always has an empty upper 16 bits, we can save some operations.

// Multiplies two numbers between -1.0 and 1.0

int fp_unit_mul(int a, int b)
{
    int sign = (a<0?-1:1)*(b<0?-1:1);
    unsigned int a1 = (unsigned int)(a<0?-a:a);
    unsigned int b1 = (unsigned int)(b<0?-b:b);

    if((a1>>16)>0)
        return sign*b1;
    else if((b1>>16)>0)
        return sign*a1;

    unsigned int  lowera = (a1 & lower16);
    unsigned int  lowerb = (b1 & lower16);

    return sign * (int)(((lowera*lowerb) & upper16)>>16);
}   

// Multiplies two numbers where b is between -1.0 and 1.0

int fp_unit_mul2(int a, int b)
{
    int sign = (a<0?-1:1)*(b<0?-1:1);
    unsigned int a1 = (unsigned int) (a<0?-a:a);
    unsigned int b1 = (unsigned int) (b<0?-b:b);

    if ((b1>>16)>0)
        return sign*a1;

    int lowera = (a1 & lower16);
    int uppera = ((a1 & upper16)>>16);

    return sign * (((lowera*b1)>>16) + uppera*b1);
}

// Multiplies two numbers where b is between 0.0 and 1.0

int fp_frac(int a, int b)
{
    if ((b>>16)>0)
        return a;

    int sign = (a<0?-1:1);
    unsigned int a1 = (unsigned int) (a<0?-a:a);
    unsigned int b1 = (unsigned int) b;

    int lowera = (a1 & lower16);
    int uppera = ((a1 & upper16)>>16);

    return sign * (((lowera*b1)>>16) + uppera*b1);
}

// Multiplies two numbers. Probably inefficient.

int fp_mul(int a, int b)
{
    int sign = (a<0?-1:1)*(b<0?-1:1);
    unsigned int a1 = (unsigned int)(a<0?-a:a);
    unsigned int b1 = (unsigned int)(b<0?-b:b);

    unsigned int  lowera = (a1 & lower16);
    unsigned int  lowerb = (b1 & lower16);
    unsigned int  uppera = ((a1 & upper16)>>16);
    unsigned int  upperb = ((b1 & upper16)>>16);

    int mult1 = (int)(((lowera*lowerb) & upper16)>>16);
    int mult2 = (int)(lowera*upperb + lowerb*uppera);
    int mult3 = (int)(((uppera*upperb) & lower16)<<16);

    return (mult1+mult2+mult3)*sign;
}

// Estimate the reciprocal using the Newton-Raphson method of finding zeroes
// of real-valued functions. Number of correct bits in estimate squares per
// iteration. To ensure a good estimate after 2 iterations, the argument given
// should be between 0.5 and 1.0

int fp_recip(int a)
{
    // This is the optimal starting estimate for the reciprocal of values known
    // to be in the range 0.5 to 1.0
    int estimate    = 185043 - fp_frac(123362, a);

    // Two iterations of the method should suffice.
    estimate        = fp_mul(estimate, 131072 - fp_frac(estimate, a) );
    estimate        = fp_mul(estimate, 131072 - fp_frac(estimate, a) );

    return estimate;
}

// Find the reciprocal of a number by shifting it until it lies between
// 0.5 and 1.0, then call the function above to use Newton's method

int fp_recip_gen(int a, int id)
{
    if (a==0)
    {
      rt_error (RTE_DIV0); //##
      //##        io_printf(IO_STD, "Fixed-point arithmetic error: div by 0 in fp_recip_gen %d.\n", id);
    }

    int sign = (a<0?-1:1);
    unsigned int a1 = (unsigned int)(a<0?-a:a);

    int shifted = 0;

    while (a1>65536)
    {
        a1>>=1;
        shifted -= 1;
    }
    while (a1<=32768)
    {
        a1<<=1;
        shifted +=1 ;
    }
    a1 = fp_recip(a1);
    a1 = (shifted>0 ? (a1<<shifted) : (a1>>-(shifted)) );
    return sign*((int)a1);
}

// To divide the numerator by the denominator, estimate the reciprocal of the
// denominator and multiply the two. The function for finding the reciprocal
// only works on denominators between 0.5 and 1, so we have to bitshift both
// arguments until it lies in this range.

int fp_div(int num, int denom)
{
    if (denom==0)
    {
      rt_error (RTE_DIV0); //##
      //##  io_printf(IO_STD, "Fixed-point arithmetic error: div by 0 in fp_div.\n");
    }

    // fp_recip works only on positive values
    if (denom<0)
    {
        num=-num;
        denom=-denom;
    }

    // fp_recip works only on values in range 0.5 to 1.0
    while(denom>65536) // while greater than 1
    {
        num>>=1;
        denom>>=1;
    }
    while(denom<=32768) // while less than half
    {
        num<<=1;
        denom<<=1;
    }

    return fp_mul(num,fp_recip(denom));
}

// Ken Turkowski's implementation of fast fixed point square root.
// Don't ask me how it works, but it's faster than any function I wrote for it
// (Note: square roots are still expensive Anyone looking to optimize
// should start here)

int fp_sqrt(int x)
{
    unsigned int root, remHi, remLo, testDiv, count;

    root = 0;
    remHi = 0;
    remLo = x;
    count = (15 + (16>>1));

    do
    {
        // Get 2 bits of arg
        remHi = (remHi << 2) | (remLo >> 30); remLo <<= 2;
        root <<= 1;                 // Get ready for the next bit in the root
        testDiv = (root << 1) + 1;  // Test radical
        if (remHi >= testDiv) {
            remHi -= testDiv;
            root += 1;
        }
    }   while (count-- != 0);
    return(root);
}


// A look-up table  for cos(x), with x in range 0.0 to Pi/2 (1/4 of the cycle)
// (Note: the mapping might be slighly off, knocking some angles off by about
// a degree.  Warrants investigation.)

int cos_lookup[101] = {65536, 65527, 65503, 65463, 65406, 65333, 65245, 65140,
65019, 64882, 64729, 64560, 64375, 64174, 63957, 63725, 63477, 63213, 62933,
62638, 62328, 62002, 61661, 61305, 60933, 60547, 60145, 59729, 59298, 58853,
58393, 57918, 57429, 56926, 56409, 55878, 55333, 54775, 54203, 53618, 53019,
52408, 51783, 51146, 50496, 49833, 49159, 48472, 47773, 47063, 46340, 45607,
44862, 44106, 43339, 42562, 41774, 40975, 40167, 39349, 38521, 37683, 36836,
35980, 35115, 34242, 33360, 32470, 31572, 30666, 29752, 28831, 27903, 26969,
26027, 25079, 24125, 23165, 22199, 21228, 20251, 19270, 18283, 17293, 16298,
15299, 14296, 13289, 12280, 11267, 10252, 9234, 8213, 7191, 6167, 5141, 4115,
3087, 2058, 1029, 0};

#define ONE_OVER_2_PI   10430

// Transforms a into an index for the above lookup table.  The table covers
// 1/4 of the cycle of the cos function.  For the other quarters, either
// the looked-up value, the look-up index, or both are flipped.

int fp_cos(int a)
{
    int lookup_size = 100;
    int factor = (65535/(lookup_size*4-1));

    // a is an angle in rads
    // We only care about it in the range 0 -> 2*pi

    // this is between 0 and 65536
    int unscaledIndex = (fp_frac(a, ONE_OVER_2_PI) & lower16);

    int indexFraction = unscaledIndex % factor;
    // This is now a lookup index between 0 and lookup_size*4-1
    int index = unscaledIndex / factor;
    // Second index for interpolation
    int index2 = (index+1)%(lookup_size*4);
    // How far between the 2 indices does true angle lie? (for interpolation)
    indexFraction = indexFraction & lower16;

    // Lookup value 1 for interpolation
    // Flip the value and/or index as required
    int sign = 1;
    if (index >= lookup_size && index < lookup_size*2)
    {
        index = lookup_size - 1 - (index % lookup_size);
        sign = -1;
    }
    else if (index >= lookup_size*2 && index < lookup_size*3)
    {
        index = index % lookup_size;
        sign = -1;
    }
    else if (index >= lookup_size*3 && index < lookup_size*4)
    {
        index = lookup_size - 1 - (index % lookup_size);
    }
    int value = sign * cos_lookup[index];

    // Lookup value 2 for interpolation
    // Flip the value and/or index as required
    int sign2 = 1;
    if (index2 >= lookup_size && index2 < lookup_size*2)
    {
        index2 = lookup_size - (index2 % lookup_size);
        sign2 = -1;
    }
    else if (index2 >= lookup_size*2 && index2 < lookup_size*3)
    {
        index2 = index2 % lookup_size;
        sign2 = -1;
    }
    else if (index2 >= lookup_size*3 && index2 < lookup_size*4)
    {
        index2 = lookup_size - (index2 % lookup_size);
    }
    int value2 = sign2 * cos_lookup[index2];

    // Interpolate between the two values
    return (value2*indexFraction)/factor + \
    (value*(factor-indexFraction))/factor;
}

// End of arithmetic.c





// geometry.h
// Points, directions, colours

struct Vector3
{      
    int x, y, z;
};

struct Ray
{
    struct Vector3 origin;
    struct Vector3 direction; // unit vector
};

struct Sphere
{
    struct Vector3  position;
    int             radius;
    struct Vector3  colourT;     // transmitted colour (0->1)
    struct Vector3  colourE;     // emitted colour     (0->255)
    int             specularity;
    int             specularTightness;
    int             transparency;
    int             refractiveIndex;
    int             reflectivity;
};

// geometry.c

//-------- Vector functions ---------------------------------------------------

struct Vector3 vecInvert(struct Vector3 a)
{
    struct Vector3 result = {-a.x, -a.y, -a.z};
    return result;
}

struct Vector3 vecAdd(struct Vector3 a, struct Vector3 b)
{
    struct Vector3 result = {a.x+b.x, a.y+b.y, a.z+b.z};
    return result;
}

struct Vector3 vecSub(struct Vector3 a, struct Vector3 b)
{
    struct Vector3 result = {a.x-b.x, a.y-b.y, a.z-b.z};
    return result;
}

// Note: For some functions there are multiple copies to take advantage of
// the users knowledge of the variables involved. E.g., due to fixed point
// arithmetic, we can save time if we know that one of the operands always
// has an absolute value less than 1.0 (by skipping two of the integer
// multiplications)

// Multiply a vector by a scalar value between 0.0 and 1.0

struct Vector3 vecScalarFrac(struct Vector3 a, int b)
{
    struct Vector3 result = {fp_frac(a.x,b), fp_frac(a.y,b), fp_frac(a.z,b)};
    return result;
}

// Multiply a vector by a scalar value

struct Vector3 vecScalarMul(struct Vector3 a, int b)
{
    struct Vector3 result = {fp_mul(a.x,b), fp_mul(a.y,b), fp_mul(a.z,b)};
    return result;
}

// Element-wise multiplication of two vectors where the elements of vector b
// are between 0.0 and 1.0

struct Vector3 vecFrac(struct Vector3 a, struct Vector3 b)
{
    struct Vector3 result = \
        {fp_frac(a.x,b.x), fp_frac(a.y,b.y), fp_frac(a.z,b.z)};
    return result;
}

// Element-wise multiplication of two vectors

struct Vector3 vecMul(struct Vector3 a, struct Vector3 b)
{
    struct Vector3 result = \
        {fp_mul(a.x,b.x), fp_mul(a.y,b.y), fp_mul(a.z,b.z)};
    return result;
}

int vecMagnitude(struct Vector3 in)
{
    return fp_sqrt(fp_mul(in.x,in.x)+fp_mul(in.y,in.y)+fp_mul(in.z,in.z));
}

struct Vector3 vecNorm(struct Vector3 in, int id)
{
    int vecMagnitudeReciprocal = fp_recip_gen(vecMagnitude(in), id);

    if (vecMagnitudeReciprocal>(1<<16))
    {
        struct Vector3 result = {   fp_mul(in.x,vecMagnitudeReciprocal),
                                    fp_mul(in.y,vecMagnitudeReciprocal),
                                    fp_mul(in.z,vecMagnitudeReciprocal) };
        return result;
    }
    else
    {
        struct Vector3 result = {   fp_frac(in.x,vecMagnitudeReciprocal),
                                    fp_frac(in.y,vecMagnitudeReciprocal),
                                    fp_frac(in.z,vecMagnitudeReciprocal) };
        return result;
    }
}

// The dot product of two vectors

int vecDotProd(struct Vector3 a, struct Vector3 b)
{
    return fp_mul(a.x,b.x) + fp_mul(a.y,b.y) + fp_mul(a.z,b.z);
}

// The dot product of two vectors that we know to be unit vectors

int vecUnitDotProd(struct Vector3 a, struct Vector3 b)
{
    return fp_unit_mul(a.x,b.x) + \
            fp_unit_mul(a.y,b.y) + fp_unit_mul(a.z,b.z);
}

// The dot product of two vectors where b is known to be a unit vector

int vecUnitDotProd2(struct Vector3 a, struct Vector3 b)
{
    return fp_unit_mul2(a.x,b.x) + \
            fp_unit_mul2(a.y,b.y) + fp_unit_mul2(a.z,b.z);
}

// The cross product of two vectors

struct Vector3 vecCrossProd(struct Vector3 a, struct Vector3 b)
{
    struct Vector3 result = {   fp_mul(a.y,b.z)-fp_mul(a.z,b.y),
                                fp_mul(a.z,b.x)-fp_mul(a.x,b.z),
                                fp_mul(a.x,b.y)-fp_mul(a.y,b.x)   };
    return result;
}

// Rotate the first vector around the second (amount is in degrees)

struct Vector3 vecRotate(struct Vector3 rotated,
                                struct Vector3 rotateAbout,int amount, int id)
{
    amount = fp_mul(amount,FP_PI)/180;
    int c = fp_cos(amount);
    int s = fp_cos(amount - (FP_PI>>1));
    int t = (1<<16)-c;

    // The rotation (sorry about the readability!)

    struct Vector3 result;
    result.x = \
        fp_mul(rotated.x,(fp_mul(t,fp_mul(rotateAbout.x,rotateAbout.x))+c))\
        + fp_mul(rotated.y,(fp_mul(t,fp_mul(rotateAbout.x,rotateAbout.y)) \
            + fp_mul(s,rotateAbout.z))) + \
        fp_mul(rotated.z,(fp_mul(t,fp_mul(rotateAbout.x,rotateAbout.z)) \
            - fp_mul(s,rotateAbout.y)));

    result.y = \
        fp_mul(rotated.x,(fp_mul(t,fp_mul(rotateAbout.x,rotateAbout.y)) \
            - fp_mul(s,rotateAbout.z))) + \
        fp_mul(rotated.y,(fp_mul(t,fp_mul(rotateAbout.y,rotateAbout.y)) \
            + c)) + \
        fp_mul(rotated.z,(fp_mul(t,fp_mul(rotateAbout.y,rotateAbout.z)) \
            + fp_mul(s,rotateAbout.x)));

    result.z = \
        fp_mul(rotated.x,(fp_mul(t,fp_mul(rotateAbout.z,rotateAbout.x)) \
            + fp_mul(s,rotateAbout.y))) + \
        fp_mul(rotated.y,(fp_mul(t,fp_mul(rotateAbout.y,rotateAbout.z)) \
            - fp_mul(s,rotateAbout.x))) + \
        fp_mul(rotated.z,(fp_mul(t,fp_mul(rotateAbout.z,rotateAbout.z))+c));

    return vecNorm(result, id);
}

// Clamp the elements of a vector between a lower and upper value

struct Vector3 vecClamp(struct Vector3 in, int loClamp, int hiClamp)
{
    struct Vector3 clamped = {
        (in.x > hiClamp) ? hiClamp : ((in.x < loClamp) ? loClamp : in.x), 
        (in.y > hiClamp) ? hiClamp : ((in.y < loClamp) ? loClamp : in.y), 
        (in.z > hiClamp) ? hiClamp : ((in.z < loClamp) ? loClamp : in.z)
    };

    return clamped;
}

// Calculate the refraction when vector n1 goes from a material with refractive
// index mu1 into one with refractive index mu2, via a surface with normal s
// (s goes in the direction in to the new material, so may actually be the
// inverse of the collision-object surface normal depending on if the ray is
// entering or exiting the object)

struct Vector3 vecRefract(struct Vector3 n1, 
                    struct Vector3 s, int mu1, int mu2)
{
    int n1DotS      = vecUnitDotProd(n1,s);
    struct Vector3 firstTerm   = vecScalarMul(s,n1DotS);
    int sqrtTerm    = fp_sqrt(  (fp_mul(mu2,mu2)) - \
                                (fp_mul(mu1,mu1)) + \
                                (fp_mul(n1DotS,n1DotS)) );
    struct Vector3 result = \
        vecAdd( vecSub(n1, firstTerm),
            vecScalarMul(s, sqrtTerm) );

    return result;  
}

int vecSum(struct Vector3 inVector)
{
    return inVector.x+inVector.y+inVector.z;
}

//-------- Vectors ------------------------------------------------------------




//-------- Ray functions ------------------------------------------------------

// Nudges a ray along slightly in its direction vector.  Used to get rays away
// from the surfaces on which they have just reflected/refracted to avoid re-
// interacting with the same surface

struct Ray rayNudge(struct Ray ray)
{
    ray.origin = \
        vecAdd(ray.origin, vecScalarFrac(ray.direction,1<<10));
    return ray;
}

//-------- Rays ---------------------------------------------------------------





//-------- Sphere functions ---------------------------------------------------

// If the ray intersects the sphere, return nearest distance along ray at which
// it happens. If it doesn't, return -1

int sphereIntersection(struct Sphere sphere, struct Ray ray)
{
    // The intersection algorithm assumes that the ray starts at the origin,
    // so translate the sphere first.
    sphere.position = vecSub(sphere.position, ray.origin);

    int directionDotPosition = \
        vecUnitDotProd2(sphere.position, ray.direction);
    int sqrtTerm = fp_mul(directionDotPosition,directionDotPosition) \
                    - vecDotProd(sphere.position, sphere.position) \
                    + fp_mul(sphere.radius,sphere.radius);
    
    int result = -1; // Return value for no intersection

    if (sqrtTerm >= 0) // If there is an intersection (more likely 2)
    {
        sqrtTerm = fp_sqrt(sqrtTerm);
        // There are usually two solutions, for the two intersection points
        // between the ray and sphere.
        int solution1 = directionDotPosition + sqrtTerm;
        int solution2 = directionDotPosition - sqrtTerm;

        // We want the nearest non-negative (behind the ray origin) intersection
        if (solution1 >= 0)
        {
            result = solution1;
            if (solution2 >= 0 && solution2 < solution1)
                result = solution2;
        }
        else if (solution2 >= 0)
            result = solution2;
    }

    return result;
}

//-------- Spheres ------------------------------------------------------------

// end of geometry.c

// scene.h

// Camera, with viewplane

struct ViewPlane
{
  // these are unit vectors and represent the direction to the four
  // corners of the view plane from the camera
    struct Vector3 topLeft;
    struct Vector3 topRight;
    struct Vector3 bottomLeft;
    struct Vector3 bottomRight;
};

struct Camera
{
    struct Vector3      position;
    struct Vector3      lookDirection;
    struct Vector3      upDirection;

    int                 horizontalFieldOfView;
    int                 verticalFieldOfView;
    int                 horizontalPixels;
    int                 verticalPixels;

    int                 antialiasing;

    struct Vector3      rightDirection;
    struct ViewPlane    viewPlane;  // The two view angles are used to construct the viewplane
};

// scene.c

// Given a camera with a valid position, look vector, up vector, fields of view and horizontal pixel resolution,
// construct a view plane in front of the camera and calculate a right vector

struct Camera setupCamera(struct Camera camera)
{
    struct Vector3 rightDirection = vecCrossProd(camera.lookDirection,camera.upDirection);
    camera.rightDirection = rightDirection; 

    struct ViewPlane viewPlane;
    // The look direction needs to be rotated to the four corners of the viewPlane
    // TODO: this method distorts the image so we've had to move the camera back and narrow its field of view.
    //   Fix it, then tidy it up.
    struct Vector3 corner = camera.lookDirection;
    struct Vector3 right  = camera.rightDirection;
    corner = vecRotate(corner, camera.upDirection, -camera.horizontalFieldOfView/2, 1013);
    right  = vecCrossProd(corner, camera.upDirection);
    corner = vecRotate(corner, right, -camera.verticalFieldOfView/2, 1014);
    viewPlane.topLeft = vecNorm(corner, 1000);
    corner = camera.lookDirection;
    corner = vecRotate(corner, camera.upDirection, camera.horizontalFieldOfView/2, 1015);
    right  = vecCrossProd(corner, camera.upDirection);
    corner = vecRotate(corner, right, -camera.verticalFieldOfView/2, 1016);
    viewPlane.topRight = vecNorm(corner, 1001);
    corner = camera.lookDirection;
    corner = vecRotate(corner, camera.upDirection, -camera.horizontalFieldOfView/2, 1017);
    right  = vecCrossProd(corner, camera.upDirection);
    corner = vecRotate(corner, right, camera.verticalFieldOfView/2, 1018);
    viewPlane.bottomLeft = vecNorm(corner, 1002);
    corner = camera.lookDirection;
    corner = vecRotate(corner, camera.upDirection, camera.horizontalFieldOfView/2, 1019);
    right  = vecCrossProd(corner, camera.upDirection);
    corner = vecRotate(corner, right, camera.verticalFieldOfView/2, 1020);
    viewPlane.bottomRight = vecNorm(corner, 1003);

    camera.viewPlane = viewPlane;
    return camera;
}

// end of scene.c





int random2()
{
  return (int) (sark_rand () % (1 << 16));
}

int random1()
{
  return ((int) sark_rand () % (1 << 16)) - (1 << 15); 
}


int diffuseBDRF(struct Vector3 in, struct Vector3 out, struct Vector3 collisionPoint, struct Vector3 surfaceNormal)
{
    return vecDotProd(surfaceNormal,out);
}

// Cosine-weighted diffuse reflection

struct Ray diffuseReflectanceFunction(struct Vector3 collisionPoint, struct Vector3 surfaceNormal, struct Vector3 inwardVector, int *BDRF)
{
    struct Ray outwardRay;
    outwardRay.origin = collisionPoint;

    outwardRay.direction.x = random1();
    outwardRay.direction.y = random1();
    outwardRay.direction.z = random1();
    while (vecSum(outwardRay.direction) < 1000)
    {
        outwardRay.direction.x = random1();
        outwardRay.direction.y = random1();
        outwardRay.direction.z = random1();
    }
    outwardRay.direction = vecNorm(outwardRay.direction, 1005);

    outwardRay.direction = vecAdd(outwardRay.direction, vecScalarMul(surfaceNormal,(270<<8)));
    outwardRay.direction = vecNorm(outwardRay.direction, 1006);

    outwardRay = rayNudge(outwardRay);

    *BDRF = diffuseBDRF(inwardVector, outwardRay.direction, collisionPoint, surfaceNormal);

    return outwardRay;
}

int specularBDRF(struct Vector3 in, struct Vector3 out, struct Vector3 collisionPoint, struct Vector3 surfaceNormal, int tightness)
{

    struct Vector3 reflectedOut = vecSub(out , vecScalarMul(surfaceNormal, 2*vecUnitDotProd(out,surfaceNormal))) ;

    int phongTerm   = vecUnitDotProd(in, reflectedOut);

    phongTerm       = (phongTerm < 0 ? -phongTerm : phongTerm); // having to get abs of value because dot product is probably using an inverted vector
    int i;
    for (i=0; i<tightness; i++)
    {
        phongTerm = fp_frac(phongTerm,phongTerm);
    }

    return phongTerm;
}


// Perfectly specular reflection

struct Ray specularReflectanceFunction (struct Vector3 collisionPoint, struct Vector3 surfaceNormal,
					struct Vector3 inwardVector, int tightness, int *BDRF)
{
    // First, find a vector that's perpendicular to the surface normal (there are an infinite number of these...)
    // All methods require conditional logic.  One method is to find the element of the surface normal vector that's closest to zero
    // Produce another vector where this element is one and the others are zero
    // The dot product of this vector and the surface normal is perpendicular to the surface normal.
    int minimumElement = 0;
    if (abs(surfaceNormal.y) < abs(surfaceNormal.x))
        minimumElement = 1;
    if (abs(surfaceNormal.z) < abs(surfaceNormal.x) && abs(surfaceNormal.z) < abs(surfaceNormal.y))
        minimumElement = 2;
    struct Vector3 crossProdVector = {(minimumElement==0 ? (1<<16) : 0), (minimumElement==1 ? (1<<16) : 0), (minimumElement==2 ? (1<<16) : 0)};
    struct Vector3 perpendicularVector = vecNorm(vecCrossProd(surfaceNormal, crossProdVector), 1040);

    // Set the outward ray to be the same as the surface normal
    struct Ray outwardRay;
    outwardRay.origin = collisionPoint;
    outwardRay.direction = surfaceNormal;
    // Rotate the outward ray about the perpendicular vector by theta (random2() 0.0 and 1.0.. raise it to power to produce specular effect)
    // result is actually random2()^(2^power)
    int theta = random2();
    int i;
    for (i=0; i<tightness; i++)
    {
        theta = fp_mul(theta,theta);
    }
    
    outwardRay.direction = vecRotate(outwardRay.direction, perpendicularVector, theta*60, 1030);

    // Rotate the outward ray about the surface normal by phi (0 to 360)  
    outwardRay.direction = vecNorm(vecRotate(outwardRay.direction, surfaceNormal, random2()*360, 1031), 1007);

    *BDRF = specularBDRF(inwardVector, outwardRay.direction, collisionPoint, surfaceNormal, tightness);

    // Voila: perfectly diffuse reflection
    return outwardRay;
}

int mirrorBDRF(struct Vector3 in, struct Vector3 out, struct Vector3 collisionPoint, struct Vector3 surfaceNormal)
{
    return (1<<16);
}


// Perfect mirror reflection

struct Ray mirrorReflectanceFunction (struct Vector3 collisionPoint, struct Vector3 surfaceNormal,
				      struct Vector3 inwardVector, int *BDRF)
{
    struct Ray reflectedRay = {collisionPoint, vecNorm(vecSub(inwardVector, vecScalarMul(surfaceNormal, 2*vecUnitDotProd(inwardVector,surfaceNormal))), 1008) };
    reflectedRay = rayNudge(reflectedRay); // Nudge the ray along to stop it re-interacting with the same surface

    *BDRF = mirrorBDRF(inwardVector, reflectedRay.direction, collisionPoint, surfaceNormal);

    return reflectedRay;
}

int refractiveBDRF (struct Vector3 in, struct Vector3 out, struct Vector3 collisionPoint,
		    struct Vector3 surfaceNormal)
{
    return (1<<16);
}

struct Ray refractiveReflectanceFunction(struct Vector3 collisionPoint, struct Vector3 surfaceNormal,
					 struct Vector3 inwardVector, int inverted, int refractiveIndex1,
					 int refractiveIndex2, int *BDRF)
{
    // Do entry refraction (inverted surface normal?)
    if (inverted)
        surfaceNormal = vecInvert(surfaceNormal);

    struct Ray transmittedRay = {collisionPoint, vecRefract(inwardVector, surfaceNormal, refractiveIndex1, refractiveIndex2)};
    transmittedRay = rayNudge(transmittedRay);

    *BDRF = refractiveBDRF(inwardVector, transmittedRay.direction, collisionPoint, surfaceNormal);

    return transmittedRay;
}

// given a collision point, a surface normal, and the direction of the inward ray, produce an outward ray (probabilistic)

struct Ray reflectanceFunction(struct Vector3 collisionPoint, struct Vector3 surfaceNormal,
			       struct Vector3 inwardVector, struct Sphere sphere, int *BDRF)
{
    struct Ray outwardRay;

    if ( random2() < sphere.specularity )
        outwardRay = specularReflectanceFunction(collisionPoint, surfaceNormal, inwardVector, sphere.specularTightness, BDRF);
    else if ( random2() < sphere.transparency )
        outwardRay = refractiveReflectanceFunction(collisionPoint, surfaceNormal, inwardVector, 1, (1<<16), sphere.refractiveIndex, BDRF);
    else if ( random2() < sphere.reflectivity )
        outwardRay = mirrorReflectanceFunction(collisionPoint, surfaceNormal, inwardVector, BDRF);
    else
        outwardRay = diffuseReflectanceFunction(collisionPoint, surfaceNormal, inwardVector, BDRF);

    return outwardRay;
}

// given two vectors and a material, return the BDRF

int materialBDRF(struct Vector3 inward, struct Vector3 outward, struct Sphere sphere,
		 struct Vector3 collisionPoint, struct Vector3 surfaceNormal)
{
    int BDRF;

    if ( random2() < sphere.specularity )
        BDRF = specularBDRF(inward, outward, collisionPoint, surfaceNormal, sphere.specularTightness);
    else if ( random2() < sphere.transparency )
        BDRF = fp_mul(sphere.specularity, specularBDRF(inward, outward, collisionPoint, surfaceNormal, sphere.specularTightness));
    else if ( random2() < sphere.reflectivity )
        BDRF = fp_mul(sphere.specularity, specularBDRF(inward, outward, collisionPoint, surfaceNormal, sphere.specularTightness));
    else
        BDRF = diffuseBDRF(inward, outward, collisionPoint, surfaceNormal);

    return BDRF;
}

struct Vector3 nilColour = {-1,-1,-1};
struct Vector3 black     = {0,0,0};

struct Vector3 traceRay(struct Ray ray, int depth, int terminationDepth,
                        struct Sphere *spheres,
                        int numberOfSpheres)
{
    
    // Determine the closest intersecting sphere.
    int closestIntersection = 0;
    int intersectingObject  = -1;
    
    int i;  // Check intersection will all spheres
    for (i=0; i<numberOfSpheres; i++)
    {
        int sphereIntersect = sphereIntersection(spheres[i], ray);
        // Find nearest positive intersection
        if (sphereIntersect > 0 && (sphereIntersect < closestIntersection || closestIntersection==0))
        {
            closestIntersection = sphereIntersect;
            intersectingObject = i;
        }
    }

    // If we hit something
    if (intersectingObject != -1)
    {  

        int BDRF = 0;

        int nonSpecular = (1<<16) - spheres[intersectingObject].specularity;
        int directLightingFactor = fp_mul((1<<16)-fp_mul(spheres[intersectingObject].reflectivity,nonSpecular),(1<<16)-fp_mul(spheres[intersectingObject].transparency,nonSpecular));

        struct Vector3 colour = {0, 0, 0};

        struct Vector3 collisionPoint = vecAdd(ray.origin, vecScalarMul(ray.direction, closestIntersection));
        struct Vector3 surfaceNormal  = vecSub(collisionPoint, spheres[intersectingObject].position);
        if (vecMagnitude(surfaceNormal)>0)
            surfaceNormal  = vecNorm(surfaceNormal, 1009);
        else
            return nilColour;

        struct Vector3 inwardVector   = ray.direction;

        // Catch exit refraction case
        if (vecMagnitude(vecSub(ray.origin,spheres[intersectingObject].position)) < spheres[intersectingObject].radius)
        {
            return traceRay(refractiveReflectanceFunction(collisionPoint, surfaceNormal, inwardVector, 0, spheres[intersectingObject].refractiveIndex, (1<<16), &BDRF), depth+1, terminationDepth, spheres, numberOfSpheres);
        }
        else // not exit refraction, do all else
        {

            // If we hit a light, then BINGO, stop bouncing around
            if (vecMagnitude(spheres[intersectingObject].colourE) > 50)
            {
                return spheres[intersectingObject].colourE;
            }

            // Calculate direct lighting
            // Check which lights are visible
            struct Vector3 directLighting = {0,0,0};
            int lightIndex;
            for (lightIndex=0; lightIndex<numberOfSpheres; lightIndex++)
            {
                if (vecMagnitude(spheres[lightIndex].colourE)<10)
                    continue; // not a light

                int lightVisible = 1; // Occlusion check

                // instead of using the light centre, pick a random point on it's surface
                struct Vector3 surfacePoint = {random1(),random1(),random1()};
                while (vecSum(surfacePoint) < 1000)
                {
                    surfacePoint.x = random1();
                    surfacePoint.y = random1();
                    surfacePoint.z = random1();
                }
                surfacePoint = vecNorm(surfacePoint, 1010);
                surfacePoint = vecAdd(spheres[lightIndex].position,vecScalarMul(surfacePoint, spheres[lightIndex].radius));

                struct Vector3 shadowRayDirection   = vecSub(surfacePoint, collisionPoint);
                int shadowRayLength                 = vecMagnitude(shadowRayDirection);
                shadowRayDirection                  = vecNorm(shadowRayDirection, 1011);
                struct Ray shadowRay = {collisionPoint, shadowRayDirection};
                shadowRay = rayNudge(shadowRay); // Nudge the ray along to stop it re-interacting with the same surface

                int visibility = 1<<16; // used to accumulate shadows from TRANSPARENT objects

                int occludingObjectIndex;
                for (occludingObjectIndex=0; occludingObjectIndex<numberOfSpheres; occludingObjectIndex++) // Check all objects for occlusion
                {
                    if (lightIndex == occludingObjectIndex) // a light can't occlude itself
                        continue;
                    int shadowIntersect = sphereIntersection(spheres[occludingObjectIndex], shadowRay); // < 0.0 if there is no intersection, otherwise it is the length along the ray at which the intersection happens

                    if (shadowIntersect >= 0 && shadowIntersect < shadowRayLength ) // If the light is ocluded
                    {
                        if (spheres[occludingObjectIndex].transparency<10) // If the occluding object is opaque, stop checking
                        {
                            lightVisible = 0; // Opaque object blocks light
                            break;
                        }
                        else
                        {
                            visibility = fp_frac(visibility, (2*spheres[occludingObjectIndex].transparency)/3); // Transparent object blocks some light
                        }
                    }
                }
                if (lightVisible)
                {
                    // Direct lighting
                    BDRF = materialBDRF(ray.direction, shadowRay.direction, spheres[intersectingObject],  collisionPoint, surfaceNormal);
                    directLighting = vecAdd(directLighting, vecScalarMul(vecScalarFrac(vecFrac(spheres[lightIndex].colourE, spheres[intersectingObject].colourT ), visibility), BDRF) );
                }
            }
            // end of direct lighting calculation

            colour = vecAdd(colour, vecScalarFrac(directLighting, directLightingFactor));

            // Calculate the indirect lighting
            if (depth<terminationDepth)
            {
                int sample, numberOfSamples;
                numberOfSamples = (depth==0 ? 4 : 1); // Take samples from ten directions on the first bounce, as those first bounces are more important

                struct Vector3 indirectLighting = {0,0,0};                

                for (sample = 0; sample<numberOfSamples; sample++)
                {
                    // new ray, according to the BRDF
                    struct Ray reflectedRay = reflectanceFunction(collisionPoint, surfaceNormal, inwardVector, spheres[intersectingObject], &BDRF);

                    indirectLighting = vecAdd(indirectLighting, vecScalarMul(vecMul(spheres[intersectingObject].colourT, traceRay(reflectedRay, depth+1, terminationDepth, spheres, numberOfSpheres)), BDRF) );
                }

                indirectLighting.x/=numberOfSamples;
                indirectLighting.y/=numberOfSamples;
                indirectLighting.z/=numberOfSamples; 

                colour = vecAdd(colour, indirectLighting);
            }

            return colour;
        }
    }
    else
        return vecClamp(black, 0, (255<<16));
}













// Sphere objects (position, radius, transmitted colour, emitted colour,       specularity, specularTightness, transparency, refractiveIndex, reflectivity
struct Sphere spheres[12] = {    
                                {{0,(150<<9),0},    20<<9,  {0,0,0},     {250<<16,220<<16,170<<16} ,0,2,0,(1<<16),0 } , // light

                                {{30<<9,0,-(40<<9)},    20<<9,  {210<<8,200<<8,200<<8},     {0,0,0} ,(0<<8),2,0,(1<<16),(1<<16) } , // Mirror ball
                                {{-(50<<9),0,-(60<<9)},    20<<9,  {210<<8,150<<8,120<<8},     {0,0,0} ,(200<<8),2,0,(1<<16),0 } , // Brass ball
                                {{(30<<9),0,72<<9},       20<<9,  {1<<16,1<<16,1<<16},        {0,0,0} ,(25<<8),4,(255<<8),(270<<8),0 } , // Glass ball
                                {{-(30<<9),0,(10<<9)}, 20<<9,  {180<<8,180<<8,180<<8},      {0,0,0} ,(25<<8),2,0,(1<<16),0 } , // White ball 1

                                {{160<<16,0,0},         (160<<16)-(100<<9), {120<<8,120<<8,120<<8},     {0,0,0} ,0,2,0,(1<<16),0 } , 
                                {{-(160<<16),0,0},      (160<<16)-(300<<9), {120<<8,120<<8,120<<8},     {0,0,0} ,0,2,0,(1<<16),0 } , 

                                {{0,0,160<<16},         (160<<16)-(100<<9), {30<<8,30<<8,110<<8},       {0,0,0} ,0,2,0,(1<<16),0 } , 
                                {{0,0,-(160<<16)},      (160<<16)-(100<<9), {110<<8,30<<8,30<<8},       {0,0,0} ,0,2,0,(1<<16),0 } , 

                                {{0,160<<16,0},         (160<<16)-(180<<9), {120<<8,120<<8,120<<8},     {0,0,0} ,0,2,0,(1<<16),0 } , 
                                {{0,-(160<<16),0},      (160<<16)-(22<<9),  {120<<8,120<<8,120<<8},     {0,0,0} ,0,2,0,(1<<16),0 } , 

                            };

struct Vector3* horizontalInterpolations1;
struct Vector3* horizontalInterpolations2;




int actualAA = 1;
int aaInc = 1;

// Do the full ray trace.  NodeID is used to determine the first pixel we care about.  Number of nodes is used to determine how far we jump
void trace(int horizontalPixels, int verticalPixels, int horizontalFieldOfView, int verticalFieldOfView, int antialiasing, int x, int y, int z, int lookX, int lookY, int lookZ, int upX, int upY, int upZ, int nodeID, int numberOfNodes)
{

    int numberOfSpheres = sizeof(spheres) / sizeof(struct Sphere);

    // Camera
    struct Camera camera = { {x,y,z}, {lookX,lookY,lookZ}, {upX,upY,upZ}, horizontalFieldOfView, verticalFieldOfView };
    camera.horizontalPixels = horizontalPixels;
    camera.verticalPixels   = verticalPixels;
    camera.antialiasing = antialiasing;
    camera = setupCamera(camera); // Sets up the right vector and the viewplane

    // For use in antialiasing
    int antialiasingStartPoint  = -(1<<16) + (1<<16)/camera.antialiasing; // antialiasing isn't bitshifted, so normal divide
    int antialiasingStepSize    = (2<<16)/camera.antialiasing;

    // For repeated use in interpolation
    struct Vector3 topRightMinusTopLeft = vecSub(camera.viewPlane.topRight, camera.viewPlane.topLeft);
    struct Vector3 bottomRightMinusBottomLeft = vecSub(camera.viewPlane.bottomRight, camera.viewPlane.bottomLeft);

    int i,j;
    // Determine the first pixel this node cares about based on its ID
    int iFirst = 0;
    int jFirst = nodeID;
    while (jFirst >= camera.verticalPixels)
        jFirst -= camera.verticalPixels;
        iFirst += 1;
    int overshoot = jFirst;

    // Main loop
    i = iFirst;
    while (i<camera.horizontalPixels)
    {

        if (i==iFirst && actualAA <= camera.antialiasing)    
        {
            aaInc ++;
            if (aaInc>=2)
            {
                actualAA ++;
                antialiasingStartPoint  = -(1<<16) + (1<<16)/camera.antialiasing; // antialiasing isn't bitshifted, so normal divide
                antialiasingStepSize    = (2<<16)/camera.antialiasing;
                aaInc=0;
            }
            //io_printf(IO_STD, "%dx%d antialiasing, pass %d.\n", actualAA,actualAA,aaInc);
        }


        uint iSegment1 = ((i&upper8)>>8); // for MC transmission
        uint iSegment2 = ((i&lower8)<<24);

        // This gets a little messy because of the antialiasing, but we're really just iterating over sub-pixels, casting rays, and averaging over pixels
        int k;
        for (k=0; k<actualAA; k++)
        {   
            horizontalInterpolations1[k] = vecAdd(vecScalarMul(topRightMinusTopLeft, ((i<<16)+(antialiasingStartPoint+antialiasingStepSize*k))/camera.horizontalPixels), camera.viewPlane.topLeft);
            horizontalInterpolations2[k] = vecAdd(vecScalarMul(bottomRightMinusBottomLeft, ((i<<16)+(antialiasingStartPoint+antialiasingStepSize*k))/camera.horizontalPixels), camera.viewPlane.bottomLeft);
        }

        j = overshoot;
        while (j<camera.verticalPixels)
        {
            struct Vector3 accumulatedColour = {0,0,0}; // to accumulate colours from sub-pixels (antialiasing)
            int numberSuccessful = 0;

            for (k=0; k<actualAA; k++)
            {
                // For repeated use in vertical interpolation
                struct Vector3 horizontal2MinusHorizontal1 = vecSub(horizontalInterpolations2[k] , horizontalInterpolations1[k]);

                int l; // vertical antialiasing
                for (l=0; l<actualAA; l++)
                {
                    struct Vector3 rayDirection = vecNorm(vecAdd(vecScalarMul(horizontal2MinusHorizontal1, ((j<<16)+(antialiasingStartPoint+antialiasingStepSize*l))/camera.verticalPixels), horizontalInterpolations1[k]), 1012);
                    struct Ray ray = {camera.position, rayDirection};

                    struct Vector3 result = traceRay(ray, 0, 4, spheres, numberOfSpheres);
                    if (result.x != -1)
                    {
                        accumulatedColour = vecAdd(accumulatedColour, result);
                        numberSuccessful += 1;
                    }
                }
            }

            if (numberSuccessful != 0)
            {
                struct Vector3 colour = vecScalarFrac(vecScalarFrac(accumulatedColour, (1<<16)/(numberSuccessful)),1);

                colour = vecClamp(colour, 0, 255);

                uint key        = (j<<16) | proc001Key | iSegment1;
                uint payload    = iSegment2 | (((uint)colour.x)<<16) | (((uint)colour.y)<<8) | ((uint)colour.z);

#ifdef API
		spin1_send_mc_packet(key,payload,1);
#else
                pkt_tx_kd (key, payload);
#endif
            }

            // End of inner loop
            j+=numberOfNodes;
            if (j >= camera.verticalPixels)
                overshoot = j - (j/camera.verticalPixels)*camera.verticalPixels;

        }

        i++;
        if(i>=camera.horizontalPixels)
            i=iFirst;
    }

}


void sdp_packet_callback(uint msg, uint port)
{
  sdp_msg_t *sdp_msg = (sdp_msg_t *) msg;

  io_printf (IO_BUF, "SDP cmd %d\n", sdp_msg->cmd_rc);

  if (sdp_msg->cmd_rc == 4) // if it's a trace message   
    {
      int camx = 0, camy = 0, camz = 0, lookx = 0, looky = 0, lookz = 0, upx = 0;
      int upy = 0, upz = 0, width = 0, height = 0, hField = 0, vField = 0;
      int antialiasing = 0, nodeID=0, numberOfNodes=16;

      for (int i=0; i<(sdp_msg->length-24)/4; i++)
        {
	  int var = 0;
	  var += (sdp_msg->data[i*4]<<0);
	  var += (sdp_msg->data[i*4+1]<<8);
	  var += (sdp_msg->data[i*4+2]<<16);
	  var += (sdp_msg->data[i*4+3]<<24);      

	  switch(i)
            {
	    case 0: camx = var; break;
	    case 1: camy = var; break;
	    case 2: camz = var; break;
	    case 3: lookx = var; break;
	    case 4: looky = var; break;
	    case 5: lookz = var; break;
	    case 6: upx = var; break;
	    case 7: upy = var; break;
	    case 8: upz = var; break;
	    case 9: width = var; break;
	    case 10: height = var; break;
	    case 11: hField = var; break;
	    case 12: vField = var; break;
	    case 13: antialiasing = var; break;
	    case 14: nodeID = var; break;
	    case 15: numberOfNodes = var; break;
            }
        }

            //io_printf(IO_STD, "x %d \ny %d \nz  %d \nx  %d \ny  %d \nz  %d \nx  %d \ny  %d \nz  %d \nw  %d \nh  %d \nhF  %d \nvF  %d \naa  %d \nid  %d \nnum  %d \n ", camx, camy, camz, lookx, looky, lookz, upx, upy, upz, width, height, hField, vField, antialiasing, nodeID, numberOfNodes);

        //io_printf(IO_STD, "I am core %d %d %d.  I've been told I'm node %d of %d.\n", (spin1_get_chip_id()&secondByte)>>8, spin1_get_chip_id()&firstByte, spin1_get_core_id(), nodeID, numberOfNodes );

      sark_delay_us (1000 * sark_core_id ());

      sdp_msg->cmd_rc     = 5; // trace ack
      uchar temp_port     = sdp_msg->srce_port;
      ushort temp_addr    = sdp_msg->srce_addr;
      sdp_msg->srce_port = sdp_msg->dest_port;
      sdp_msg->srce_addr = sdp_msg->dest_addr;
      sdp_msg->dest_port = temp_port;
      sdp_msg->dest_addr = temp_addr;
      sdp_msg->length    = SDP_HEADER_SIZE;
      sark_msg_send(sdp_msg, 10); // reply

#ifdef API
      spin1_callback_off(SDP_PACKET_RX);
#else
      event_enable (EVENT_SDP, 0);
#endif
      sark_delay_us (800000 + 32000 * sark_core_id ());

      trace (width, height, hField, vField, antialiasing, camx, camy, camz, lookx,
	     looky, lookz, upx, upy, upz, nodeID, numberOfNodes);
    }
  else if (sdp_msg->cmd_rc == 6) // status request
    {
      sark_delay_us (1000 * sark_core_id ());

      sdp_msg->cmd_rc     = 7; // status ack
      uchar temp_port     = sdp_msg->srce_port;
      ushort temp_addr    = sdp_msg->srce_addr;
      sdp_msg->srce_port = sdp_msg->dest_port;
      sdp_msg->srce_addr = sdp_msg->dest_addr;
      sdp_msg->dest_port = temp_port;
      sdp_msg->dest_addr = temp_addr;

      sark_msg_send(sdp_msg, 10); // reply
    }

  sark_msg_free (sdp_msg);
}



void load_mc_routing_tables()
{
  if (sark_core_id () != sark_app_lead ())
    return;

  //!!  uint xy = sark_chip_id ();
  uint xy = sv->board_addr;

  uint x = CHIP_X (xy);
  uint y = CHIP_Y (xy);

  uint destination;    

  uint e = rtr_alloc (1);
  if (e == 0)
    rt_error (RTE_ABORT);

  // Routing table entry for sending pixel packets to core 1 of the root chip
  // We basically use the same route used by the P2P tables to route back to to
  // (0, 0) since this route will avoid dead links and chips etc.
  uint route;
  uint p2p_route = rtr_p2p[0] & P2P_BMASK;
  switch (p2p_route) {
    case 6:  rt_error (RTE_ABORT); return;
    case 7:  route = MC_CORE_ROUTE(1); break;
    default: route = MC_LINK_ROUTE(p2p_route); break;
  }
  rtr_mc_set (e, proc001Key, multicastControlMask, route);
}

void timer_callback (uint time, uint none)
{
  io_printf (IO_BUF, "I am alive\n");
}


void c_main()
{
  io_printf (IO_BUF, "Started tracer\n");

  sark_srand (sark_chip_id() + sark_core_id());

  load_mc_routing_tables();

  horizontalInterpolations1 = sark_alloc (20, sizeof(struct Vector3));
  horizontalInterpolations2 = sark_alloc (20, sizeof(struct Vector3));

#ifdef API
  spin1_set_timer_tick(1000000 + 1000*spin1_get_core_id() + spin1_get_chip_id());
  spin1_callback_on(SDP_PACKET_RX, sdp_packet_callback, 2);

  spin1_start (SYNC_NOWAIT);
#else
  event_register_queue (sdp_packet_callback, EVENT_SDP, SLOT_1, PRIO_0);
  event_register_queue (timer_callback, EVENT_TIMER, SLOT_2, PRIO_1);
  event_register_pkt (64, SLOT_0);

  event_start (1000000 + 1000*sark_core_id () + sark_chip_id (), 0, SYNC_NOWAIT);
#endif
}
