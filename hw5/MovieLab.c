/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2024                   */
/* MovieLab.c: source file for the main function                     */
/* Please use this template for your HW5.                            */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#include "FileIO.h"
#include "DIPs.h"
#include "Movie.h"
#include "Constants.h"
#include "Image.h"

/* Load one movie frame from the input file */
YUVImage* LoadOneFrame(const char* fname, int n,
	unsigned int width, unsigned height);

/* Load the movie frames from the input file */
Movie *LoadMovie(const char *fname, int frameNum,
	unsigned int width, unsigned height);

/* Saves one movie frame to the output file */
void SaveOneFrame(YUVImage *image, const char *fname, FILE *file);

/* Save the movie frames to the output file */
int SaveMovie(const char *fname, Movie *movie);

/* Print the command-line arguments usage of the program */
void PrintUsage();


int main(int argc, char *argv[])
{
	int x = 0;
	char *fin = NULL;
	char *fout = NULL;
	Movie *movie = NULL;
	
	unsigned int width = 512;
	unsigned int height = 288;

	unsigned int frameNum;
	unsigned int newFrameNum;
	
	/* the while loop to check options in the command line */
	while (x < argc) {
		/* the input file name */
		if (strcmp(argv[x], "-i") == 0) {
			if (x < argc - 1) {
				fin = argv[x + 1];
				printf("The movie file %s has been read successfully!\n", fin);
			} /*fi*/
			else {
				printf("Missing argument for the input file name!\n");
				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/

		/* the output file name */
		if (strcmp(argv[x], "-o") == 0) {
			if (x < argc - 1) {
				fout = argv[x + 1];
			} /*fi*/
			else {
				printf("Missing argument for the output file name!\n");
				return 5;
			} /*esle*/
			x += 2;
			continue;
		} /*fi*/

		// frame number option
		if (strcmp(argv[x], "-f") == 0) {
			if (x < argc - 1)
			{
				frameNum = atoi(argv[x + 1]);
			}
			else
			{
				printf("Missing argument for the number of frames!\n");
				return 5;
			}
			x += 2;
			continue;
		} 

		// resolution option
		if (strcmp(argv[x], "-s") == 0) {
			if (x < argc - 1)
			{
				if (sscanf(argv[x + 1], "%ux%u", &width, &height) == 2)
				{
					// input is correct, width and height stored
				}
			}
			else
			{
				printf("Missing argument for input resolution!\n");
				return 5;
			}
			x += 2;
			continue;
		} 

		// black and white option
		if (strcmp(argv[x], "-bw") == 0) {
			// if movie has not been loaded, load movie and convert to RGB
			if (!movie)
			{
				movie = LoadMovie(fin, frameNum, width, height);
				YUV2RGBMovie(movie);
			}

			IENTRY *imgEntry, *nextEntry;
			ILIST *frames = movie->Frames;
			imgEntry = frames->First;

			// perform operation on each frame
			while (imgEntry)
			{
				nextEntry = imgEntry->Next;
				BlackNWhite(imgEntry->image);
				imgEntry = nextEntry;
			}
			printf("Operation Black and White is done!\n");
			x++;
			continue;
		} 
		
		// hmirror option
		if (strcmp(argv[x], "-hmirror") == 0) {
			// if movie has not been loaded, load movie and convert to RGB
			if (!movie)
			{
				movie = LoadMovie(fin, frameNum, width, height);
				YUV2RGBMovie(movie);
			}

			IENTRY *imgEntry, *nextEntry;
			ILIST *frames = movie->Frames;
			imgEntry = frames->First;

			// perform operation on each frame
			while (imgEntry)
			{
				nextEntry = imgEntry->Next;
				HMirror(imgEntry->image);
				imgEntry = nextEntry;
			}
			printf("Operation HMirror is done!\n");
			x++;
			continue;
		}

		// edge option
		if (strcmp(argv[x], "-edge") == 0) {
			// if movie has not been loaded, load movie and convert to RGB
			if (!movie)
			{
				movie = LoadMovie(fin, frameNum, width, height);
				YUV2RGBMovie(movie);
			}

			IENTRY *imgEntry, *nextEntry;
			ILIST *frames = movie->Frames;
			imgEntry = frames->First;

			// perform operation on each frame
			while (imgEntry)
			{
				nextEntry = imgEntry->Next;
				Edge(imgEntry->image);
				imgEntry = nextEntry;
			}
			printf("Operation Edge is done!\n");
			x++;
			continue;
		}

		// crop function
		if (strcmp(argv[x], "-crop") == 0) {
			unsigned int start, end;

			// if movie has not been loaded, load movie and convert to RGB
			if (!movie)
			{
				movie = LoadMovie(fin, frameNum, width, height);
				YUV2RGBMovie(movie);
			}

			// perform operation
			if (x < argc - 1) {
				if (sscanf(argv[x + 1], "%u-%u", &start, &end) == 2) {
					newFrameNum = (end - start) + 1; 
					CropImageList(movie->Frames, start, end);
					printf("Operation Crop is done! New number of frames is %u.\n", newFrameNum);		
				} 
			}
			else {
				printf("Missing argument for start and end!\n");
				return 5;
			} 
			x += 2;
			continue;
		}

		// fast forward function
		if (strcmp(argv[x], "-fast") == 0) {
			unsigned int factor;

			// if movie has not been loaded, load movie and convert to RGB
			if (!movie)
			{
				movie = LoadMovie(fin, frameNum, width, height);
				YUV2RGBMovie(movie);
			}

			// perform operation
			if (x < argc - 1) {
				factor = atoi(argv[x + 1]);
				FastImageList(movie->Frames, factor);
				printf("Operation Fast Forward is done! New number of frames is %u.\n", movie->Frames->Length);
			}
			else {
				printf("Missing argument for fast forward factor!\n");
				return 5;
			}
			x += 2; 
			continue;
		}

		// reverse option
		if (strcmp(argv[x], "-reverse") == 0) {
			// if movie has not been loaded, load movie and convert to RGB
			if (!movie)
			{
				movie = LoadMovie(fin, frameNum, width, height);
				YUV2RGBMovie(movie);
			}

			// perform operation
			ReverseImageList(movie->Frames);

			printf("Operation Reverse is done!\n");
			x++;
			continue;
		}
		
		// watermark option
		if (strcmp(argv[x], "-watermark") == 0) {
			srand(time(NULL));
			unsigned int topLeftX, topLeftY;
			char *watermarkFile;

			// if movie has not been loaded, load movie and convert to RGB
			if (!movie)
			{
				movie = LoadMovie(fin, frameNum, width, height);
				YUV2RGBMovie(movie);
			}

			if (x < argc - 1)
			{
				watermarkFile = argv[x + 1];

				IENTRY *imgEntry, *nextEntry;
				ILIST *frames = movie->Frames;
				imgEntry = frames->First;
				Image *watermarkImage = LoadImage(watermarkFile);

				while (imgEntry)
				{
					// randomly generate topLeftX and topLeftY coordinates
					// (make sure entire watermark is in image)
					do 
					{
						topLeftX = rand() % ImageWidth(imgEntry->image);
						topLeftY = rand() % ImageHeight(imgEntry->image);
					} while (topLeftX + ImageWidth(watermarkImage) >= ImageWidth(imgEntry->image) &&
							 topLeftY + ImageHeight(watermarkImage) >= ImageHeight(imgEntry->image));
					
					// apply watermark for a constant amount of consecutive frames
					for (int i = 0; i < WATERMARK_PER_FRAMES; ++i)
					{
						if (imgEntry == NULL) break;
						nextEntry = imgEntry->Next;
						Watermark(imgEntry->image, watermarkImage, topLeftX, topLeftY);
						imgEntry = nextEntry;
					}
				}

				printf("Operation Watermark is done!\n");
			}
			else
			{
				printf("Missing argument for watermark file!\n");
				return 5;
			}
			
			x++;
			continue;
		}

		// spotlight option
		if (strcmp(argv[x], "-spotlight") == 0) {
			// if movie has not been loaded, load movie and convert to RGB
			if (!movie)
			{
				movie = LoadMovie(fin, frameNum, width, height);
				YUV2RGBMovie(movie);
			}

			IENTRY *imgEntry, *nextEntry, *lastEntry;
			ILIST *frames = movie->Frames;
			imgEntry = frames->First;
			lastEntry = frames->Last;
			unsigned int radius;
			int centerX = width / 2;
			int centerY = height / 2;
			double maxRadius = sqrt(centerX * centerX + centerY * centerY);

			// copy first and last frame 20 times
			for (int x = 0; x < 20; ++x)
			{
				PrependRGBImage(frames, CopyRGBImage(imgEntry->image));
				AppendRGBImage(frames, CopyRGBImage(lastEntry->image));
			}
			
			// perform spotlight operation on first 20 frames
			imgEntry = frames->First;
			for (int x = 0; x < 20; ++x)
			{
				nextEntry = imgEntry->Next;
				
				// perform operation with linearly increasing radius
				radius = (unsigned int)(maxRadius * (x / 19.0));
				Spotlight(imgEntry->image, centerX, centerY, radius);

				imgEntry = nextEntry;
			}

			// perform spotlight operation on last 20 frames (in reverse)
			imgEntry = frames->Last;
			for (int x = 0; x < 20; ++x)
			{
				nextEntry = imgEntry->Prev;
				
				// perform operation with linearly decreasing radius
				radius = (unsigned int)(maxRadius * (x / 19.0));
				Spotlight(imgEntry->image, centerX, centerY, radius);

				imgEntry = nextEntry;
			}

			// if final movie has only 40 frames, print error and exit
			if (frames->Length <= 40)
			{
				printf("Error: Operation Spotlight failed!\n");
				return 5;
			}

			printf("Operation Spotlight is done!\n");
			x++;
			continue;
		}

		// rotate option
		if (strcmp(argv[x], "-rotate") == 0) {
			// if movie has not been loaded, load movie and convert to RGB
			if (!movie)
			{
				movie = LoadMovie(fin, frameNum, width, height);
				YUV2RGBMovie(movie);
			}

			IENTRY *imgEntry, *nextEntry, *lastEntry;
			ILIST *frames = movie->Frames;
			imgEntry = frames->First;
			lastEntry = frames->Last;

			// copy first and last frame 20 times
			for (int x = 0; x < 20; ++x)
			{
				PrependRGBImage(frames, CopyRGBImage(imgEntry->image));
				AppendRGBImage(frames, CopyRGBImage(lastEntry->image));
			}
			
			// perform rotate operation on first 20 frames
			imgEntry = frames->First;
			for (int x = 20; x >= 1; --x)
			{
				nextEntry = imgEntry->Next;

				double Angle = (double)(x);
				double ScaleFactor = (0.20) + ((20 - x) * 0.04);
				
				// perform operation with linearly increasing angle and zoom factor
				imgEntry->image = Rotate(imgEntry->image, Angle, ScaleFactor);

				imgEntry = nextEntry;
			}

			// perform rotate operation on last 20 frames (in reverse)
			imgEntry = frames->Last;
			for (int x = 20; x >= 1; --x)
			{
				nextEntry = imgEntry->Prev;
				
				double Angle = (double)(x);
				double ScaleFactor = (0.20) + ((20 - x) * 0.04);
				
				// perform operation with linearly increasing angle and zoom factor
				imgEntry->image = Rotate(imgEntry->image, Angle, ScaleFactor);

				imgEntry = nextEntry;
			}

			// if final movie has only 40 frames, print error and exit
			if (frames->Length <= 40)
			{
				printf("Error: Operation Rotate failed!\n");
				return 5;
			}

			printf("Operation Rotate is done!\n");
			x++;
			continue;
		}

		/* the help option */
		if (strcmp(argv[x], "-h") == 0) {
			PrintUsage();
			return 0;
		} /*fi*/

		x++;
	} /*elihw*/

	if (fin == NULL) {
		printf("Missing argument for the input file name!\n");
		PrintUsage();
		return 5;
	}

	if (fout == NULL) {
		printf("Missing argument for the output file name!\n");
		PrintUsage();
		return 5;
	}

	// convert movie back to YUV
	RGB2YUVMovie(movie);

	// save and delete movie
	SaveMovie(fout, movie);
	DeleteMovie(movie);
	movie = NULL;

	fin = NULL;
	fout = NULL;

	return 0;
}

void PrintUsage()
{
	printf("\nUsage: MovieLab -i <file> -o <file> -f <framenum> [options]\n"
	       "Options:\n"
	       "-s <WidthxHeight>     To set the resolution of the input stream (WidthxHeight)\n"
	       "-bw                   Activate the black and white filter on every movie frame\n"
	       "-hmirror              Activate horizontal mirror on every movie frame\n"
	       "-edge                 Activate the edge filter on every movie frame\n"
	       "-crop <start-end>     Crop the movie frames from <start> to <end>\n"
	       "-fast <factor>        Fast forward the movie by <factor>\n"
	       "-reverse              Reverse the frame order of the input movie\n"
	       "-watermark <file>     Add a watermark from <file> to every movie frame\n"
	       "-spotlight            Spotlight the fade in and fade out\n"
	       "-rotate               Rotate and zoom in/out the input movie\n"
	       "-h                    Display this usage information\n"
	      );
}

/* Load one movie frame from the input file */
YUVImage* LoadOneFrame(const char* fname, int n,
	unsigned int width, unsigned height)
{
	FILE *file;
	unsigned int x, y;
	unsigned char c;
	YUVImage* YUVimage;

	/* Check errors */
	assert(fname);
	assert(n >= 0);

	YUVimage = CreateYUVImage(width, height);
	if (YUVimage == NULL) {
		return NULL;
	}

	/* Open the input file */
	file = fopen(fname, "r");
	if (file == NULL) {
		DeleteYUVImage(YUVimage);
		return NULL;
	}

	/* Find the desired frame */
	fseek(file, 1.5 * n * width * height, SEEK_SET);

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			c = fgetc(file);
			SetPixelY(YUVimage, x, y, c);
		} /*rof*/
	}

	for (y = 0; y < height; y += 2) {
		for (x = 0; x < width; x += 2) {
			c = fgetc(file);
			SetPixelU(YUVimage, x, y, c);
			SetPixelU(YUVimage, x + 1, y, c);
			SetPixelU(YUVimage, x, y + 1, c);
			SetPixelU(YUVimage, x + 1, y + 1, c);
		}
	}

	for (y = 0; y < height; y += 2) {
		for (x = 0; x < width; x += 2) {
			c = fgetc(file);
			SetPixelV(YUVimage, x, y, c);
			SetPixelV(YUVimage, x + 1, y, c);
			SetPixelV(YUVimage, x, y + 1, c);
			SetPixelV(YUVimage, x + 1, y + 1, c);
		}
	}

	/* Check errors */
	assert(ferror(file) == 0);

	/* Close the input file and return */
	fclose(file);
	file = NULL;
	return YUVimage;
}

/* Load the movie frames from the input file */
Movie *LoadMovie(const char *fname, int frameNum,
	unsigned int width, unsigned int height)
{
	FILE *file;
	YUVImage *YUVImage = NULL;
	Movie *movie = NULL;
	unsigned int x;

	// check errors
	assert(fname);
	assert(frameNum >= 0);
	assert(width > 0);
	assert(height > 0);

	movie = CreateMovie();
	if (movie == NULL) return NULL;

	// open input file
	file = fopen(fname, "r");
	if (file == NULL)
	{
		DeleteMovie(movie);
		return NULL;
	}

	// iterate through each frame
	for (x = 0; x < frameNum; ++x)
	{
		// load the frame and store it in YUV image
		YUVImage = LoadOneFrame(fname, x, width, height);
		if (YUVImage == NULL)
		{
			DeleteMovie(movie);
			return NULL;
		} 

		// append the image to the frames list
		AppendYUVImage(movie->Frames, YUVImage);
	}

	// check errors
	assert(ferror(file) == 0);

	// close file and return
	fclose(file);
	file = NULL;

	return movie;
}

/* Save the movie frames to the output file */
int SaveMovie(const char *fname, Movie *movie)
{
	int count;
	FILE *file;
	IENTRY *curr;

	/* Open the output file */
	file = fopen(fname, "w");
	if (file == NULL) {
		return 1;
	}

	count = 0;
	curr = movie->Frames->First;
	while (curr != NULL) {

		SaveOneFrame(curr->image, fname, file);
		curr = curr->Next;
		count++;
	}

	fclose(file);
	file = NULL;

	printf("The movie file %s has been written successfully!\n", fname);
	printf("%d frames are written to the file %s in total.\n", count, fname);
	return 0;
}

/* Saves one movie frame to the output file */
void SaveOneFrame(YUVImage *image, const char *fname, FILE *file)
{
	int x, y;
	for (y = 0; y < image->H; y++) {
		for (x = 0; x < image->W; x++) {
			fputc(GetPixelY(image, x, y), file);
		}
	}

	for (y = 0; y < image->H; y += 2) {
		for (x = 0; x < image->W; x += 2) {
			fputc(GetPixelU(image, x, y), file);
		}
	}

	for (y = 0; y < image->H; y += 2) {
		for (x = 0; x < image->W; x += 2) {
			fputc(GetPixelV(image, x, y), file);
		}
	}
}

/* EOF */
