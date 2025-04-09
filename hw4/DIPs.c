#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#include "DIPs.h" // for function declarations

/* Black and White */
Image *BlackNWhite(Image *image)
{
    int x, y;
    int width = ImageWidth(image);
    int height = ImageHeight(image);
    unsigned int tmp;

    for (x = 0; x < width; ++x)
    {
        for (y = 0; y < height; ++y)
        {
            tmp = (GetPixelR(image, x, y) + GetPixelG(image, x, y) + GetPixelB(image, x, y)) / 3;
            SetPixelR(image, x, y, tmp);
            SetPixelG(image, x, y, tmp);
            SetPixelB(image, x, y, tmp);
        }
    }
    
    return image;
}

/* Reverse image color */
Image *Negative(Image *image)
{
    int x, y;
    int width = ImageWidth(image);
    int height = ImageHeight(image);
    
    for (x = 0; x < width; ++x)
    {
        for (y = 0; y < height; ++y)
        {
            SetPixelR(image, x, y, MAX_PIXEL - GetPixelR(image, x, y));
            SetPixelG(image, x, y, MAX_PIXEL - GetPixelG(image, x, y));
            SetPixelB(image, x, y, MAX_PIXEL - GetPixelB(image, x, y));
        }
    }
    
    return image;
}

/* Color filter */
Image *ColorFilter(Image *image, int target_r, int target_g, int target_b, int threshold, int replace_r, int replace_g, int replace_b)
{
    int x, y;
    int width = ImageWidth(image);
    int height = ImageHeight(image);
    int r, g, b;

	replace_r = (replace_r > MAX_PIXEL) ? MAX_PIXEL : (replace_r < 0) ? 0 : replace_r;
	replace_g = (replace_g > MAX_PIXEL) ? MAX_PIXEL : (replace_g < 0) ? 0 : replace_g;
	replace_b = (replace_b > MAX_PIXEL) ? MAX_PIXEL : (replace_b < 0) ? 0 : replace_b;

    for (y = 0; y < height; y++){
		for (x = 0; x < width; x++){
            r = GetPixelR(image, x, y);
            g = GetPixelG(image, x, y);
            b = GetPixelB(image, x, y);
			if (abs(r - target_r) <= threshold
					&& abs(g - target_g) <= threshold
					&& abs(b - target_b) <= threshold) {
                SetPixelR(image, x, y, replace_r);
                SetPixelG(image, x, y, replace_g);
                SetPixelB(image, x, y, replace_b);
			}
		}
	}
    
    return image;
}

/* Edge detection */
Image *Edge(Image *image)
{
    int x, y, m, n, a, b;
    int width = ImageWidth(image);
    int height = ImageHeight(image);

    // create temporary image and copy values over
    Image *tmp = CreateImage(width, height);

    for (y = 0; y < height; y++){
		for (x = 0; x < width; x++) {
            SetPixelR(tmp, x, y, GetPixelR(image, x, y));
            SetPixelG(tmp, x, y, GetPixelG(image, x, y));
            SetPixelB(tmp, x, y, GetPixelB(image, x, y));
		}
	}

    // perform edge detection operation
    int sumR = 0;   /* sum of the intensity differences with neighbors */
	int sumG = 0;
	int sumB = 0;
	for (y = 1; y < height - 1; y++){
		for (x = 1; x < width - 1; x++){
			for (n = -1; n <= 1; n++){
				for (m = -1; m <= 1; m++) {
					a = (x + m >= width) ? width - 1 : (x + m < 0) ? 0 : x + m;
					b = (y + n >= height) ? height - 1 : (y + n < 0) ? 0 : y + n;
					sumR += (GetPixelR(tmp, x, y) - GetPixelR(tmp, a, b)); 
					sumG += (GetPixelG(tmp, x, y) - GetPixelG(tmp, a, b));
					sumB += (GetPixelB(tmp, x, y) - GetPixelB(tmp, a, b));
				}
			}

            SetPixelR(image, x, y, (sumR > MAX_PIXEL) ? MAX_PIXEL: (sumR < 0) ? 0: sumR);
            SetPixelG(image, x, y, (sumG > MAX_PIXEL) ? MAX_PIXEL: (sumG < 0) ? 0: sumG);
            SetPixelB(image, x, y, (sumB > MAX_PIXEL) ? MAX_PIXEL: (sumB < 0) ? 0: sumB);

			sumR = sumG = sumB = 0;
		}
	}

    /* set all 24 borders to 0 */
	for (y = 0; y < height; y++) {
        SetPixelR(image, 0, y, 0);
        SetPixelG(image, 0, y, 0);
        SetPixelB(image, 0, y, 0);
        SetPixelR(image, width - 1, y, 0);
        SetPixelG(image, width - 1, y, 0);
        SetPixelB(image, width - 1, y, 0);
	}
	for (x = 0; x < width; x++) {
        SetPixelR(image, x, 0, 0);
        SetPixelG(image, x, 0, 0);
        SetPixelB(image, x, 0, 0);
        SetPixelR(image, x, height - 1, 0);
        SetPixelG(image, x, height - 1, 0);
        SetPixelB(image, x, height - 1, 0);
	}
    
    // delete temporary image
    DeleteImage(tmp);

    return image;
}

/* Flip image vertically */
Image *VFlip(Image *image)
{
    int x, y;
    int width = ImageWidth(image);
    int height = ImageHeight(image);
	unsigned char r, g, b;

	for (y = 0; y < height/2; y++) {
		for (x = 0; x < width; x++) {
            r = GetPixelR(image, x, height - 1 - y);
            g = GetPixelG(image, x, height - 1 - y);
            b = GetPixelB(image, x, height - 1 - y);

            SetPixelR(image, x, height - 1 - y, GetPixelR(image, x, y));
            SetPixelG(image, x, height - 1 - y, GetPixelG(image, x, y));
            SetPixelB(image, x, height - 1 - y, GetPixelB(image, x, y));

            SetPixelR(image, x, y, r);
            SetPixelG(image, x, y, g);
            SetPixelB(image, x, y, b);
		}
	}
    
    return image;
}

/* Mirror image horizontally */
Image *HMirror(Image *image)
{
    int x, y;
    int width = ImageWidth(image);
    int height = ImageHeight(image);

    for (y = 0; y < height; y++) {
		for (x = 0; x < width / 2; x++) {
            SetPixelR(image, width - 1 - x, y, GetPixelR(image, x, y));
            SetPixelG(image, width - 1 - x, y, GetPixelG(image, x, y));
            SetPixelB(image, width - 1 - x, y, GetPixelB(image, x, y));
		}
	}

    return image;
}

/* Add border to an image */
Image *AddBorder(Image *image, char *color, int border_width)
{
    int x, y;
    int width = ImageWidth(image);
    int height = ImageHeight(image);

    int border_r = 255;
	int border_g = 255;
	int border_b = 255;
	if (!strcmp(color, "black")) {
		border_r = 0;
		border_g = 0;
		border_b = 0;
	} else if (!strcmp(color, "white")) {
		border_r = 255;
		border_g = 255;
		border_b = 255;
	} else if (!strcmp(color, "red")) {
		border_r = 255;
		border_g = 0;
		border_b = 0;
	} else if (!strcmp(color, "green")) {
		border_r = 0;
		border_g = 255;
		border_b = 0;
	} else if (!strcmp(color, "blue")) {
		border_r = 0;
		border_g = 0;
		border_b = 255;
	} else if (!strcmp(color, "yellow")) {
		border_r = 255;
		border_g = 255;
		border_b = 0;
	} else if (!strcmp(color, "cyan")) {
		border_r = 0;
		border_g = 255;
		border_b = 255;
	} else if (!strcmp(color, "pink")) {
		border_r = 255;
		border_g = 192;
		border_b = 203;
	} else if (!strcmp(color, "orange")) {
		border_r = 255;
		border_g = 165;
		border_b = 0;
	} else {
		printf("Unsupported color.\n");
		return image;
	}
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			if ((y < border_width) || (y > height - 1 - border_width) ||
					(x < border_width) || (x > width - 1 - border_width)) {
                SetPixelR(image, x, y, border_r);
                SetPixelG(image, x, y, border_g);
                SetPixelB(image, x, y, border_b);
			}
		}
	}
    
    return image;
}

/* Shuffle an image */
Image *Shuffle(Image *image)
{
    int x, y, i, j;
    int width = ImageWidth(image);
    int height = ImageHeight(image);
    int block_width = width / SHUFF_WIDTH_DIV;
	int block_height = height / SHUFF_HEIGHT_DIV;

    Image *tmp = CreateImage(width, height);

    for (i = 0; i < SHUFF_HEIGHT_DIV/2; i++){ // only need to iterate through half of the blocks
		for (j = 0; j < SHUFF_WIDTH_DIV; j++){
			for (x = 0; x < block_width; x++){
				for (y = 0; y < block_height; y++){
                    SetPixelR(tmp, x, y, GetPixelR(image, x + (j * block_width), y + (i * block_height)));
                    SetPixelG(tmp, x, y, GetPixelG(image, x + (j * block_width), y + (i * block_height)));
                    SetPixelB(tmp, x, y, GetPixelB(image, x + (j * block_width), y + (i * block_height)));

                    SetPixelR(image, x + (j * block_width), y + (i * block_height), GetPixelR(image, x + ((SHUFF_WIDTH_DIV - 1 -j) * block_width), y + ((SHUFF_HEIGHT_DIV - 1 -i) * block_height)));
                    SetPixelG(image, x + (j * block_width), y + (i * block_height), GetPixelG(image, x + ((SHUFF_WIDTH_DIV - 1 -j) * block_width), y + ((SHUFF_HEIGHT_DIV - 1 -i) * block_height)));
                    SetPixelB(image, x + (j * block_width), y + (i * block_height), GetPixelB(image, x + ((SHUFF_WIDTH_DIV - 1 -j) * block_width), y + ((SHUFF_HEIGHT_DIV - 1 -i) * block_height)));

                    SetPixelR(image, x + ((SHUFF_WIDTH_DIV - 1 -j) * block_width), y + ((SHUFF_HEIGHT_DIV - 1 -i) * block_height), GetPixelR(tmp, x, y));
                    SetPixelG(image, x + ((SHUFF_WIDTH_DIV - 1 -j) * block_width), y + ((SHUFF_HEIGHT_DIV - 1 -i) * block_height), GetPixelG(tmp, x, y));
                    SetPixelB(image, x + ((SHUFF_WIDTH_DIV - 1 -j) * block_width), y + ((SHUFF_HEIGHT_DIV - 1 -i) * block_height), GetPixelB(tmp, x, y));
				}
			}
		}
	}

    DeleteImage(tmp);
    
    return image;
}

/* Pixelate an image */
Image *Pixelate(Image *image, int block_size)
{
    int x, y, i, j, count;
    int width = ImageWidth(image);
    int height = ImageHeight(image);
    int sumR, sumG, sumB;
    
    for (y = 0; y < height; y += block_size)
    {
        for (x = 0; x < width; x += block_size)
        {
            sumR = sumG = sumB = 0;
            count = 0;

            for (j = 0; j < block_size; ++j)
            {
                for (i = 0; i < block_size; ++i)
                {
                    sumR += GetPixelR(image, x + i, y + j);
                    sumG += GetPixelG(image, x + i, y + j);
                    sumB += GetPixelB(image, x + i, y + j);
                    ++count;
                }
            }

            sumR /= count;
            sumG /= count;
            sumB /= count;

            for (j = 0; j < block_size; ++j)
            {
                for (i = 0; i < block_size; ++i)
                {
                    SetPixelR(image, x + i, y + j, sumR);
                    SetPixelG(image, x + i, y + j, sumG);
                    SetPixelB(image, x + i, y + j, sumB);
                }
            }
        }
    }
    
    return image;
}

/* Shift an image */
Image *Shift(Image *image, int shiftX, int shiftY)
{
    int x, y;
    int width = ImageWidth(image);
    int height = ImageHeight(image);

    // create temporary image
    Image *tmp = CreateImage(width, height);

    // main part of shift
    for (y = shiftY; y < height; ++y)
    {
        for (x = shiftX; x < width; ++x)
        {
            SetPixelR(tmp, x, y, GetPixelR(image, x - shiftX, y - shiftY));
            SetPixelG(tmp, x, y, GetPixelG(image, x - shiftX, y - shiftY));
            SetPixelB(tmp, x, y, GetPixelB(image, x - shiftX, y - shiftY));
        }
    }

    // copy over "y" shifted part
    for (y = 0; y < shiftY; ++y)
    {
        for (x = 0; x < width; ++x)
        {
            if (x - shiftX < 0)
            {
                SetPixelR(tmp, x, y, GetPixelR(image, x + width - shiftX, y + height - shiftY));
                SetPixelG(tmp, x, y, GetPixelG(image, x + width - shiftX, y + height - shiftY));
                SetPixelB(tmp, x, y, GetPixelB(image, x + width - shiftX, y + height - shiftY));
            } 
            else
            {
                SetPixelR(tmp, x, y, GetPixelR(image, x - shiftX, y + height - shiftY));
                SetPixelG(tmp, x, y, GetPixelG(image, x - shiftX, y + height - shiftY));
                SetPixelB(tmp, x, y, GetPixelB(image, x - shiftX, y + height - shiftY));
            }
        }
    }

    // copy over "x" shifted part
    for (y = 0; y < height; ++y)
    {
        for (x = 0; x < shiftX; ++x)
        {
            if (y - shiftY < 0)
            {
                SetPixelR(tmp, x, y, GetPixelR(image, x + width - shiftX, y + height - shiftY));
                SetPixelG(tmp, x, y, GetPixelG(image, x + width - shiftX, y + height - shiftY));
                SetPixelB(tmp, x, y, GetPixelB(image, x + width - shiftX, y + height - shiftY));
            } 
            else
            {
                SetPixelR(tmp, x, y, GetPixelR(image, x + width - shiftX, y - shiftY));
                SetPixelG(tmp, x, y, GetPixelG(image, x + width - shiftX, y - shiftY));
                SetPixelB(tmp, x, y, GetPixelB(image, x + width - shiftX, y - shiftY));
            }
        }
    }
    
    // delete original image and return new image
    DeleteImage(image);
    return tmp;
}