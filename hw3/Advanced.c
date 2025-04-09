#include <math.h> // for sqrt, pow, sin, cos
#include "Advanced.h" // for function declarations

#define PI 3.1415925 // for rotate function

// advanced DIP function definitions
/* Create a fisheye image W25 */
void FishEye(unsigned char R[WIDTH][HEIGHT], 
             unsigned char G[WIDTH][HEIGHT], 
             unsigned char B[WIDTH][HEIGHT], 
             double distortion_factor, double k, double scaling_factor)
{
    int x, y;
    int x_src, y_src;
    double dx, dy, radius, distortion, theta, new_radius;

    // create output arrays
    unsigned char R_out[WIDTH][HEIGHT];
    unsigned char G_out[WIDTH][HEIGHT];
    unsigned char B_out[WIDTH][HEIGHT];

    // initialize image center
    double center_x = WIDTH / 2;
    double center_y = HEIGHT / 2;

    // loop through each pixel
    for (x = 0; x < WIDTH; ++x)
    {
        for (y = 0; y < HEIGHT; ++y)
        {
            // calculate normalized distance from the center
            dx = (x - center_x) / center_x;
            dy = (y - center_y) / center_y;
            radius = sqrt((pow(dx, 2) + pow(dy, 2)));

            // calculate distortion factor based on radius
            distortion = 1.0 + k*pow(radius, 2);

            // apply fisheye transformation (polar coordinates)
            theta = atan2(dy, dx);
            new_radius = (radius * distortion_factor) / (distortion * scaling_factor);

            // convert back to Cartesian coordinates
            x_src = center_x + (new_radius * cos(theta) * center_x);
            y_src = center_y + (new_radius * sin(theta) * center_y);

            // check if source coordinates are within bounds and copy pixel
            if ((x_src >= 0 && x_src < WIDTH) && (y_src >= 0 && y_src < HEIGHT))
            {
                R_out[x][y] = R[x_src][y_src];
                G_out[x][y] = G[x_src][y_src];
                B_out[x][y] = B[x_src][y_src];
            } 
            else
            {
                R_out[x][y] = 0;
                G_out[x][y] = 0;
                B_out[x][y] = 0;
            }
        }
    }

    // copy result back into original image arrays
    for (x = 0; x < WIDTH; ++x)
    {
        for (y = 0; y < HEIGHT; ++y)
        {
            R[x][y] = R_out[x][y];
            G[x][y] = G_out[x][y];
            B[x][y] = B_out[x][y];
        }
    }
}

/* posterize the image */
void Posterize(unsigned char R[WIDTH][HEIGHT], 
               unsigned char G[WIDTH][HEIGHT], 
               unsigned char B[WIDTH][HEIGHT], 
               unsigned int rbits, unsigned int gbits, unsigned int bbits)
{
    int x, y;
    unsigned char upper, lower;
    
    // loop through all pixels
    for (x = 0; x < WIDTH; ++x)
    {
        for (y = 0; y < HEIGHT; ++y)
        {
            // shift out the lower n bits
            upper = (R[x][y] >> rbits) << rbits;
            // set bits 0 to (n-2) to 1
            lower = (1 << (rbits - 1)) - 1;
            // mask values on top of each other to get posterized value
            R[x][y] = upper | lower;

            // repeat for green and blue
            upper = (G[x][y] >> gbits) << gbits;
            lower = (1 << (gbits - 1)) - 1;
            G[x][y] = upper | lower;

            upper = (B[x][y] >> bbits) << bbits;
            lower = (1 << (bbits - 1)) - 1;
            B[x][y] = upper | lower;
        }
    }
}

/* rotate and zoom the image */
void Rotate(unsigned char R[WIDTH][HEIGHT], 
            unsigned char G[WIDTH][HEIGHT], 
            unsigned char B[WIDTH][HEIGHT], 
            double Angle, double ScaleFactor,  
            int CenterX, int CenterY)
{
    int x, y;
    int xPrime, yPrime;

    unsigned char R_out[WIDTH][HEIGHT];
    unsigned char G_out[WIDTH][HEIGHT];
    unsigned char B_out[WIDTH][HEIGHT];

    // convert angle from deg to rad
    Angle = -(Angle * PI) / 180.0;

    // loop through all pixels
    for (x = 0; x < WIDTH; ++x)
    {
        for (y = 0; y < WIDTH; ++y)
        {
            /* Matrix formula:
            xPrime = (cos(theta)/alpha) * (x - Cx) + (-sin(theta)/alpha) * (y - Cy) + Cx
            yPrime = (sin(theta)/alpha) * (x - Cx) + (cos(theta)/alpha)  * (y - Cy) + Cy
            */

            // calculate new coordinates for the pixels
            xPrime = ((cos(Angle)/ScaleFactor) * (x - CenterX)) + ((-sin(Angle)/ScaleFactor) * (y - CenterY)) + CenterX;
            yPrime = ((sin(Angle)/ScaleFactor) * (x - CenterX)) + ((cos(Angle)/ScaleFactor)  * (y - CenterY)) + CenterY;

            // check if new coordinates are within image boundaries
            if ((xPrime >= 0 && xPrime <= WIDTH - 1) && (yPrime >= 0 && yPrime <= HEIGHT - 1))
            {
                // if so, apply new coordinates to image
                R_out[x][y] = R[xPrime][yPrime];
                G_out[x][y] = G[xPrime][yPrime];
                B_out[x][y] = B[xPrime][yPrime];
            }
            else // otherwise make the pixel black
            {
                R_out[x][y] = 0;
                G_out[x][y] = 0;
                B_out[x][y] = 0;
            }
        }
    }

    // copy result back into original image arrays
    for (x = 0; x < WIDTH; ++x)
    {
        for (y = 0; y < HEIGHT; ++y)
        {
            R[x][y] = R_out[x][y];
            G[x][y] = G_out[x][y];
            B[x][y] = B_out[x][y];
        }
    }

}

/* motion blur (unfinished) */
void MotionBlur(int BlurAmount, 
                unsigned char R[WIDTH][HEIGHT], 
                unsigned char G[WIDTH][HEIGHT], 
                unsigned char B[WIDTH][HEIGHT])
{
    int x, y, z;
    int bound;

    unsigned char R_out[WIDTH][HEIGHT];
    unsigned char G_out[WIDTH][HEIGHT];
    unsigned char B_out[WIDTH][HEIGHT];

    unsigned char firstHalfR, firstHalfG, firstHalfB;    // for first half intensity values
    unsigned char secondHalfR, secondHalfG, secondHalfB; // for second half intensity values

    // loop through all pixels
    for (x = 0; x < WIDTH; ++x)
    {
        for (y = 0; y < HEIGHT; ++y)
        {
            // assign half intensity values
            firstHalfR = R[x][y] / 2;
            firstHalfG = G[x][y] / 2;
            firstHalfB = B[x][y] / 2;

            secondHalfR = 0;
            secondHalfG = 0;
            secondHalfB = 0;

            // if we are on the right edge, intensity is normal, skip to next iteration
            if (x == WIDTH - 1)
            {
                R_out[x][y] = R[x][y];
                G_out[x][y] = G[x][y];
                B_out[x][y] = B[x][y];
                continue;
            }

            // iterate horizontally, making sure to not go out of bounds
            bound = (BlurAmount + x >= WIDTH) ? WIDTH - 1 - x : BlurAmount;

            for (z = 1; z < bound; ++z)
            {
                // add values to the right for the second half
                secondHalfR += R[x + z][y];
                secondHalfG += G[x + z][y];
                secondHalfB += B[x + z][y];
            }

            // average the values out
            secondHalfR /= bound;
            secondHalfG /= bound;
            secondHalfB /= bound;
            
            // add both intensities and assign them to result arrays
            R_out[x][y] = firstHalfR + secondHalfR;
            G_out[x][y] = firstHalfG + secondHalfG;
            B_out[x][y] = firstHalfB + secondHalfB;
        }
    }

    // copy result back into original image arrays
    for (x = 0; x < WIDTH; ++x)
    {
        for (y = 0; y < HEIGHT; ++y)
        {
            R[x][y] = R_out[x][y];
            G[x][y] = G_out[x][y];
            B[x][y] = B_out[x][y];
        }
    }
}