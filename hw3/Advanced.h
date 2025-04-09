#ifndef ADVANCED_H
#define ADVANCED_H

#include "Constants.h" // for global constants

// advanced DIP function declarations
/* Create a fisheye image W25 */
void FishEye(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], double distortion_factor, double k, double scaling_factor);

/* posterize the image */
void Posterize(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], unsigned int rbits, unsigned int gbits, unsigned int bbits);

/* rotate and zoom the image */
void Rotate(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT], double Angle, double ScaleFactor,  int CenterX, int CenterY);

/* motion blur */
void MotionBlur(int BlurAmount, unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]);


#endif