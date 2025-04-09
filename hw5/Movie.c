/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2024                   */
/* Movie.c: source file for basic movie manipulations                */
/*********************************************************************/

#include <stdlib.h>
#include <assert.h>
#include "Movie.h"

/* Allocate the memory space for the movie and the memory space */
/* for the frame list. Return the pointer to the movie. */
Movie *CreateMovie(void)
{
	// allocate memory for movie
	Movie *movie = (Movie *)malloc(sizeof(Movie));
	if (movie == NULL) return NULL;
	
	// allocate memory for frame list
	movie->Frames = (ILIST *)malloc(sizeof(ILIST));
	if (movie->Frames == NULL)
	{
		DeleteMovie(movie);
		return NULL;
	}
	
	movie->Frames->Length = 0;
	movie->Frames->First = NULL;
	movie->Frames->Last = NULL;

	return movie;
}

/* Release the memory space for the frame list. */
/* Release the memory space for the movie. */
void DeleteMovie(Movie *movie)
{
	// make sure object exists
	assert(movie);
	assert(movie->Frames);

	// free up memory space
	DeleteImageList(movie->Frames);

	free(movie);
	movie = NULL;
}

/* Helper function to convert between RGB and YUV */
static unsigned int clip(int x) {return (x > 255) ? 255 : (x < 0) ? 0 : x;}

/* Convert a YUV movie to a RGB movie */
void YUV2RGBMovie(Movie *movie)
{
	assert(movie);
	assert(movie->Frames);
	assert(movie->Frames->First);

	IENTRY *imgEntry = movie->Frames->First;
	IENTRY *nextEntry = NULL;
	ILIST *RGBList = CreateImageList();
	Image *RGBImage = NULL;

	unsigned char Y, U, V, R, G, B;
	int C, D, E;
	int width = YUVImageWidth(imgEntry->image);
	int height = YUVImageHeight(imgEntry->image);

	// iterate through each frame
	while (imgEntry)
	{
		nextEntry = imgEntry->Next;
		// create a new RGB image for the frame
		RGBImage = CreateImage(width, height);
		
		/* get YUV pixel from each frame, convert to RGB,
		   and set values to new image */
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				Y = GetPixelY(imgEntry->image, x, y);
				U = GetPixelU(imgEntry->image, x, y);
				V = GetPixelV(imgEntry->image, x, y);

				C = Y - 16;
				D = U - 128;
				E = V - 128;

				R = clip((298 * C           + 409 * E + 128) >> 8);
				G = clip((298 * C - 100 * D - 208 * E + 128) >> 8);
				B = clip((298 * C + 516 * D           + 128) >> 8);

				SetPixelR(RGBImage, x, y, R);
				SetPixelG(RGBImage, x, y, G);
				SetPixelB(RGBImage, x, y, B);
			}
		}

		// append RGB image to list
		AppendRGBImage(RGBList, RGBImage);
		imgEntry = nextEntry;
	}

	// delete YUV list and assign RGB list to movie
	DeleteImageList(movie->Frames);
	movie->Frames = RGBList;
}

/* Convert a RGB movie to a YUV movie */
void RGB2YUVMovie(Movie *movie)
{
	assert(movie);
	assert(movie->Frames);
	assert(movie->Frames->First);

	IENTRY *imgEntry = movie->Frames->First;
	IENTRY *nextEntry = NULL;
	ILIST *YUVList = CreateImageList();
	YUVImage *YUVImage = NULL;

	unsigned char Y, U, V, R, G, B;
	int width = ImageWidth(imgEntry->image);
	int height = ImageHeight(imgEntry->image);

	// iterate through each frame
	while (imgEntry)
	{
		nextEntry = imgEntry->Next;
		// create a new YUV image for the frame
		YUVImage = CreateYUVImage(width, height);
		
		/* get RGB pixel from each frame, convert to YUV,
		   and set values to new image */
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				R = GetPixelR(imgEntry->image, x, y);
				G = GetPixelG(imgEntry->image, x, y);
				B = GetPixelB(imgEntry->image, x, y);

				Y = clip((( 66 * R + 129 * G +  25 * B + 128) >> 8) +  16);
				U = clip(((-38 * R -  74 * G + 112 * B + 128) >> 8) + 128);
				V = clip(((112 * R -  94 * G -  18 * B + 128) >> 8) + 128);

				SetPixelY(YUVImage, x, y, Y);
				SetPixelU(YUVImage, x, y, U);
				SetPixelV(YUVImage, x, y, V);
			}
		}

		// append YUV image to list
		AppendYUVImage(YUVList, YUVImage);
		imgEntry = nextEntry;
	}

	// delete RGB list and assign YUV list to movie
	DeleteImageList(movie->Frames);
	movie->Frames = YUVList; 
}

/* EOF */
