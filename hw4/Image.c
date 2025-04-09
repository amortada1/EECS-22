#include "Constants.h"
#include "Image.h" // for function declarations
#include <stdlib.h>
#include <assert.h>

/* Get the R intensity of pixel (x, y) in image */
unsigned char GetPixelR(const Image *image, unsigned int x,  unsigned int y)
{
    // make sure image is valid
    assert(image);
    assert(image->R != NULL);
    assert(x < image->W);
    assert(y < image->H);
    
    return image->R[x + y*image->W];
}

/* Get the G intensity of pixel (x, y) in image */
unsigned char GetPixelG(const Image *image, unsigned int x,  unsigned int y)
{
    // make sure image is valid
    assert(image);
    assert(image->G != NULL);
    assert(x < image->W);
    assert(y < image->H);
    
    return image->G[x + y*image->W];
}

/* Get the B intensity of pixel (x, y) in image */
unsigned char GetPixelB(const Image *image, unsigned int x,  unsigned int y)
{
    // make sure image is valid
    assert(image);
    assert(image->B != NULL);
    assert(x < image->W);
    assert(y < image->H);
    
    return image->B[x + y*image->W];
}

/* Set the R intensity of pixel (x, y) in image to r */
void SetPixelR(Image *image, unsigned int x,  unsigned int y, unsigned char r)
{
    // make sure image is valid
    assert(image);
    assert(image->R != NULL);
    assert(r <= MAX_PIXEL);
    assert(x < image->W);
    assert(y < image->H);
    
    // set pixel's intensity
    image->R[x + y*image->W] = r;
}

/* Set the G intensity of pixel (x, y) in image to g */
void SetPixelG(Image *image, unsigned int x,  unsigned int y, unsigned char g)
{
    // make sure image is valid
    assert(image);
    assert(image->G != NULL);
    assert(g <= MAX_PIXEL);
    assert(x < image->W);
    assert(y < image->H);
    
    // set pixel's intensity
    image->G[x + y*image->W] = g;
}

/* Set the B intensity of pixel (x, y) in image to b */
void SetPixelB(Image *image, unsigned int x,  unsigned int y, unsigned char b)
{
    // make sure image is valid
    assert(image);
    assert(image->B != NULL);
    assert(b <= MAX_PIXEL);
    assert(x < image->W);
    assert(y < image->H);
    
    // set pixel's intensity
    image->B[x + y*image->W] = b;
}

/* Allocate dynamic memory for the image structure and its R/G/B values */
/* Return the pointer to the image, or NULL in case of error */
Image *CreateImage(unsigned int Width, unsigned int Height)
{
    // allocate space in memory for image
    Image *image = (Image *)malloc(sizeof(Image));

    // if object is not created, return NULL
    if (!image) return NULL;

    // assign values to image
    image->W = Width;
    image->H = Height;
    image->R = (unsigned char *)calloc(Width * Height, sizeof(unsigned char));
    image->G = (unsigned char *)calloc(Width * Height, sizeof(unsigned char));
    image->B = (unsigned char *)calloc(Width * Height, sizeof(unsigned char));

    return image;
}

/* Free the memory for the R/G/B values and Image structure */
void DeleteImage(Image *image)
{
    assert(image);
    free(image->R);
    image->R = NULL;
    free(image->G);
    image->G = NULL;
    free(image->B);
    image->B = NULL;
    free(image);
    image = NULL;
}

/* Return the image's width in pixels */
unsigned int ImageWidth(const Image *image) 
{
    assert(image);
    return image->W;
}

/* Return the image's height in pixels */
unsigned int ImageHeight(const Image *image) 
{
    assert(image);
    return image->H;
}