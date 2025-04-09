#include "Advanced.h"

/* Crop */
Image *Crop(Image *image, int x, int y, int W, int H)
{
    int i, j;
    // width and height values for original image
    int width = ImageWidth(image);
    int height = ImageHeight(image);
    
    // create new image with given parameters
    Image *newImage = CreateImage(W, H);

    // start loop at given x and y coordinates
    for (j = 0; j < H; ++j)
    {
        for (i = 0; i < W; ++i)
        {
            // if we go out of bounds, continue
            if (i >= width || j >= height) continue;

            // otherwise copy values into new image
            SetPixelR(newImage, i, j, GetPixelR(image, x + i, y + j));
            SetPixelG(newImage, i, j, GetPixelG(image, x + i, y + j));
            SetPixelB(newImage, i, j, GetPixelB(image, x + i, y + j));
        }
    }
    
    // delete old image and return new image
    DeleteImage(image);
    return newImage;
}

/* Resize */
Image *Resize(Image *image, int newWidth, int newHeight)
{
    int x, y, xPrime, yPrime;
    // width and height values for original image
    int width = ImageWidth(image);
    int height = ImageHeight(image);

    double scaleX = (double)newWidth / width;
    double scaleY = (double)newHeight / height;

    // create new image
    Image *newImage = CreateImage(newWidth, newHeight);

    // resize operation loop
    for (y = 0; y < newHeight; ++y)
    {
        for (x = 0; x < newWidth; ++x)
        {
            // calculate new coordinates for resized image
            xPrime = (int)(x / scaleX);
            yPrime = (int)(y / scaleY);

            // make sure new coordinates are within old image's bounds
            xPrime = (xPrime >= width) ? width - 1 : xPrime;
            yPrime = (yPrime >= height) ? height - 1 : yPrime;

            // apply new coordinates to old image to create resized image
            SetPixelR(newImage, x, y, GetPixelR(image, xPrime, yPrime));
            SetPixelG(newImage, x, y, GetPixelG(image, xPrime, yPrime));
            SetPixelB(newImage, x, y, GetPixelB(image, xPrime, yPrime));
        }
    }
    
    // delete old image and return new image
    DeleteImage(image);
    return newImage;
}

/* Watermark */
Image *Watermark(Image *image, const Image *watermark_image)
{
    int x, y, i, j;
    // width and height values for original image
    int width = ImageWidth(image);
    int height = ImageHeight(image);
    // width and height values for watermark image
    int watermarkWidth = ImageWidth(watermark_image);
    int watermarkHeight = ImageHeight(watermark_image);
    unsigned char r, g, b;
    int satR, satG, satB;

    // loop for original image
    for (y = 0; y < height; ++y)
    {
        for (x = 0; x < width; ++x)
        {
            // calculate coordinates of watermark image to "wrap around" original image
            i = x % watermarkWidth;
            j = y % watermarkHeight;
            
            r = GetPixelR(watermark_image, i, j);
            g = GetPixelG(watermark_image, i, j);
            b = GetPixelB(watermark_image, i, j);

            // copy watermark onto original image
            if (r == 0 && g == 0 && b == 0)
            {
                // calculate saturated values
                satR = (GetPixelR(image, x, y) * 1.45 > 255) ? 255 : GetPixelR(image, x, y) * 1.45;
                satG = (GetPixelG(image, x, y) * 1.45 > 255) ? 255 : GetPixelG(image, x, y) * 1.45;
                satB = (GetPixelB(image, x, y) * 1.45 > 255) ? 255 : GetPixelB(image, x, y) * 1.45;
                
                SetPixelR(image, x, y, satR);
                SetPixelG(image, x, y, satG);
                SetPixelB(image, x, y, satB);
            }
        }
    }
    
    return image;
}

/* Rotate by 90 */
Image *RotateBy90(Image *image, int rotateDirection)
{
    int x, y;
    // width and height values for original image
    int width = ImageWidth(image);
    int height = ImageHeight(image);

    // create new image with width and height switched
    int newWidth = height;
    int newHeight = width;
    Image* newImage = CreateImage(newWidth, newHeight);

    if (rotateDirection == 1) // rotate counterclockwise
    {
        // iterate over new image
        for (y = 0; y < newHeight; ++y)
        {
            for (x = 0; x < newWidth; ++x)
            {
                SetPixelR(newImage, x, y, GetPixelR(image, width - 1 - y, x));
                SetPixelG(newImage, x, y, GetPixelG(image, width - 1 - y, x));
                SetPixelB(newImage, x, y, GetPixelB(image, width - 1 - y, x));
            }
        }
    }
    else // rotate clockwise
    {
        // iterate over new image
        for (y = 0; y < newHeight; ++y)
        {
            for (x = 0; x < newWidth; ++x)
            {
                SetPixelR(newImage, x, y, GetPixelR(image, y, height - 1 - x));
                SetPixelG(newImage, x, y, GetPixelG(image, y, height - 1 - x));
                SetPixelB(newImage, x, y, GetPixelB(image, y, height - 1 - x));
            }
        }
    }

    // delete original image and return new image
    DeleteImage(image);
    return newImage;
}