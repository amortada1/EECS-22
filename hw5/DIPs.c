/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2024                   */
/* DIPs.c: source file for DIP operations                            */
/*********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "DIPs.h"
#include "Constants.h"

#define PI 3.1415925 // for rotate function

/* Black and White */
Image *BlackNWhite(Image *image)
{
	assert(image);
	int x, y, tmp;

	for (y = 0; y < ImageHeight(image); y++) {
		for (x = 0; x < ImageWidth(image); x++) {
		tmp = (GetPixelR(image, x, y) + GetPixelG(image, x, y) + GetPixelB(image, x, y)) / 3;
			SetPixelR(image, x, y, tmp);
			SetPixelG(image, x, y, tmp);
		SetPixelB(image, x, y, tmp);
		}
	}
	return image;
}

/* Mirror image horizontal  */
Image *HMirror(Image *image) {
	assert(image);
	int x, y;
	for (y = 0; y < ImageHeight(image); y++) {
		for (x = 0; x < ImageWidth(image)/2; x++) {
			SetPixelG(image, ImageWidth(image) - 1 - x, y, GetPixelG(image, x, y));
			SetPixelB(image, ImageWidth(image) - 1 - x, y, GetPixelB(image, x, y));
			SetPixelR(image, ImageWidth(image) - 1 - x, y, GetPixelR(image, x, y));
		}
	}
	return image;
}

/* Edge detection */
Image *Edge(Image *image) {
	assert(image);
	Image *tmpImage = CreateImage(ImageWidth(image), ImageHeight(image));
	assert(tmpImage);

	int x, y, m, n;
	memcpy(tmpImage->R, image->R, ImageWidth(image) * ImageHeight(image) * sizeof(unsigned char));
	memcpy(tmpImage->G, image->G, ImageWidth(image) * ImageHeight(image) * sizeof(unsigned char));
	memcpy(tmpImage->B, image->B, ImageWidth(image) * ImageHeight(image) * sizeof(unsigned char));
	for (y = 1; y < ImageHeight(image) - 1; y++) {
		for (x = 1; x < ImageWidth(image) - 1; x++) {
			int tmpR = 0;
			int tmpG = 0;
			int tmpB = 0;
			for (m = -1; m <= 1; m++) {
				for (n = -1; n <= 1; n++) {
					tmpR += (GetPixelR(tmpImage, x, y) - GetPixelR(tmpImage, x + n, y + m));
					tmpG += (GetPixelG(tmpImage, x, y) - GetPixelG(tmpImage, x + n, y + m));
					tmpB += (GetPixelB(tmpImage, x, y) - GetPixelB(tmpImage, x + n, y + m));
				}
			}
			SetPixelR(image, x, y, (tmpR > 255) ? 255 : (tmpR < 0) ? 0 : tmpR);
			SetPixelG(image, x, y, (tmpG > 255) ? 255 : (tmpG < 0) ? 0 : tmpG);
			SetPixelB(image, x, y, (tmpB > 255) ? 255 : (tmpB < 0) ? 0 : tmpB);
		}
	}
	for (y = 0; y < ImageHeight(image); y++) {
		x = 0;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
		x = ImageWidth(image) - 1;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
	}
	for (x = 0; x < ImageWidth(image); x++) {
		y = 0;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
		y = ImageHeight(image) - 1;
		SetPixelR(image, x, y, 0);
		SetPixelG(image, x, y, 0);
		SetPixelB(image, x, y, 0);
	}
	DeleteImage(tmpImage);
	tmpImage = NULL;
	return image;
}

/* Add a watermark to an image */
Image *Watermark(Image *image, const Image *watermark,
	 unsigned int topLeftX, unsigned int topLeftY)
{
	assert(image);
	assert(watermark);

	unsigned char R, G, B;

	// loop through watermark image
	for (int y = 0; y < ImageHeight(watermark); ++y)
	{
		for (int x = 0; x < ImageWidth(watermark); ++x)
		{
			// check if within bounds of original image
			if (topLeftX + x < ImageWidth(image) && topLeftY + y < ImageHeight(image))
			{
				// apply watermark to image
				if (GetPixelR(watermark, x, y) == 0 && GetPixelG(watermark, x, y) == 0
					&& GetPixelB(watermark, x, y) == 0)
				{
					R = GetPixelR(image, topLeftX + x, topLeftY + y);
					G = GetPixelG(image, topLeftX + x, topLeftY + y);
					B = GetPixelB(image, topLeftX + x, topLeftY + y);

					if (R * WATERMARK_RATIO > 255)
						SetPixelR(image, topLeftX + x, topLeftY + y, 255);
					else
						SetPixelR(image, topLeftX + x, topLeftY + y, R * WATERMARK_RATIO);

					if (G * WATERMARK_RATIO > 255)
						SetPixelG(image, topLeftX + x, topLeftY + y, 255);
					else
						SetPixelG(image, topLeftX + x, topLeftY + y, G * WATERMARK_RATIO);

					if (B * WATERMARK_RATIO > 255)
						SetPixelB(image, topLeftX + x, topLeftY + y, 255);
					else
						SetPixelB(image, topLeftX + x, topLeftY + y, B * WATERMARK_RATIO);
				}
			}
		}
	}

	return image;
}

/* Spotlight */
Image *Spotlight(Image *image, int centerX, int centerY, unsigned int radius)
{
	assert(image);
	unsigned int width = ImageWidth(image);
	unsigned int height = ImageHeight(image);
	unsigned int radiusSquared = radius * radius;

	// loop through image
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			int dx = centerX - x;
			int dy = centerY - y;
			// if pixel is not within radius of center, set to black
			if (dx * dx + dy * dy > radiusSquared)
			{
				SetPixelR(image, x, y, 0);
				SetPixelG(image, x, y, 0);
				SetPixelB(image, x, y, 0);
			}
		}
	}
	
	return image;
}

/* Rotate and zoom an image (extra credit) */
Image *Rotate(Image *image, double Angle, double ScaleFactor)
{
	assert(image);

	int xPrime, yPrime;
	unsigned int width = ImageWidth(image);
	unsigned int height = ImageHeight(image);
	unsigned int CenterX = width / 2;
	unsigned int CenterY = height / 2;
	
	// create copy image
	Image* newImage = CreateImage(width, height);

	// convert angle from deg to rad
	Angle = -(Angle * PI) / 180.0;

	// loop through each pixel
	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			// calculate new coordinates for the pixels
            xPrime = ((cos(Angle)/ScaleFactor) * (x - CenterX)) + ((-sin(Angle)/ScaleFactor) * (y - CenterY)) + CenterX;
            yPrime = ((sin(Angle)/ScaleFactor) * (x - CenterX)) + ((cos(Angle)/ScaleFactor)  * (y - CenterY)) + CenterY;

			// check if new coordinates are within image boundaries
            if ((xPrime >= 0 && xPrime <= width - 1) && (yPrime >= 0 && yPrime <= height - 1))
            {
                // if so, apply new coordinates to image
				SetPixelR(newImage, x, y, GetPixelR(image, xPrime, yPrime));
				SetPixelG(newImage, x, y, GetPixelG(image, xPrime, yPrime));
				SetPixelB(newImage, x, y, GetPixelB(image, xPrime, yPrime));
            }
            else // otherwise make the pixel black
            {
				SetPixelR(newImage, x, y, 0);
				SetPixelG(newImage, x, y, 0);
				SetPixelB(newImage, x, y, 0);
            }
		}
	}

	// delete old image and return new image
	DeleteImage(image);
	return newImage;
}
/* EOF */
