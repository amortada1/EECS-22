/*********************************************************************/
/* PhotoLab.c: Assignment 2 for EECS 22, Winter 2024                 */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*** global definitions ***/
#define WIDTH  512		/* image width */
#define HEIGHT 288		/* image height */
#define SLEN    80		/* maximum length of file names */

/*** function declarations ***/

/* print a menu */
void PrintMenu(void);

/* read image from a file */
int LoadImage(const char fname[SLEN],
	      unsigned char R[WIDTH][HEIGHT],
	      unsigned char G[WIDTH][HEIGHT],
	      unsigned char B[WIDTH][HEIGHT]);

/* save a processed image */
int SaveImage(const char fname[SLEN],
	      unsigned char R[WIDTH][HEIGHT],
	      unsigned char G[WIDTH][HEIGHT],
	      unsigned char B[WIDTH][HEIGHT]);

/* change a color image to black & white */
void BlackNWhite(unsigned char R[WIDTH][HEIGHT],
		 unsigned char G[WIDTH][HEIGHT],
		 unsigned char B[WIDTH][HEIGHT]);

/* reverse image color */
void Negative(unsigned char R[WIDTH][HEIGHT],
	      unsigned char G[WIDTH][HEIGHT],
	      unsigned char B[WIDTH][HEIGHT]);

/* color filter */
void ColorFilter(unsigned char R[WIDTH][HEIGHT],
		 unsigned char G[WIDTH][HEIGHT],
                 unsigned char B[WIDTH][HEIGHT],
		 int target_r, int target_g, int target_b, int threshold,
		 int replace_r, int replace_g, int replace_b);

/* edge detection */
void Edge(unsigned char R[WIDTH][HEIGHT],
	  unsigned char G[WIDTH][HEIGHT],
          unsigned char B[WIDTH][HEIGHT]);

/* mirror image horizontally */
void HMirror(unsigned char R[WIDTH][HEIGHT],
	     unsigned char G[WIDTH][HEIGHT],
             unsigned char B[WIDTH][HEIGHT]);

/* shuffle the image */
void Shuffle(unsigned char R[WIDTH][HEIGHT],
	     unsigned char G[WIDTH][HEIGHT],
             unsigned char B[WIDTH][HEIGHT]);


/* add border */
void AddBorder(unsigned char R[WIDTH][HEIGHT],
	       unsigned char G[WIDTH][HEIGHT],
               unsigned char B[WIDTH][HEIGHT],
	       char color[SLEN], int border_width);

/* flip image vertically */
void VFlip(unsigned char R[WIDTH][HEIGHT],
	   unsigned char G[WIDTH][HEIGHT],
           unsigned char B[WIDTH][HEIGHT]);

/* test all functions */
void AutoTest(unsigned char R[WIDTH][HEIGHT],
	      unsigned char G[WIDTH][HEIGHT],
	      unsigned char B[WIDTH][HEIGHT]);

int main(void)
{
    /* Two dimensional arrays to hold the current image data, */
    /* one array for each color component.                    */
    unsigned char   R[WIDTH][HEIGHT];
    unsigned char   G[WIDTH][HEIGHT];
    unsigned char   B[WIDTH][HEIGHT];

    int choice;
    char fileName[SLEN];

    int target_r, target_g, target_b, threshold, replace_r, replace_g, replace_b;
    
    /* Please replace the following code with proper menu with function calls for DIP operations */
    if (LoadImage("EngPlaza", R, G, B) != 0)
    {
      return 1;
    }

    // print the menu
    PrintMenu();

    // main program loop
    while (1)
    {
        // prompt user for input
        scanf("%d", &choice);

        // if user enters 12, exit main loop
        if (choice == 12) break;

        // run each function based on user choice
        switch (choice)
        {
            case 1:
                printf("Please input the file name to load: ");
                scanf("%s", fileName);
                LoadImage(fileName, R, G, B);
                printf("\n");
                break;
            case 2:
                printf("Please input the file name to save: ");
                scanf("%s", fileName);
                SaveImage(fileName, R, G, B);
                printf("\n");
                break;
            case 3:
                BlackNWhite(R, G, B);
                printf("\"Black & White\" operation is done!\n");
                break;
            case 4:
                Negative(R, G, B);
                printf("\"Negative\" operation is done!\n");
                break;
            case 5:
                // prompt user for input (make sure these values are between 0 and 255)
                do 
                {
                    printf("Enter Red component for the target color: ");
                    scanf("%d", &target_r);
                    printf("Enter Green component for the target color: ");
                    scanf("%d", &target_g);
                    printf("Enter Blue component for the target color: ");
                    scanf("%d", &target_b);
                } while (!((target_r >= 0) && (target_r <= 255)) &&
                         !((target_g >= 0) && (target_g <= 255)) &&
                         !((target_b >= 0) && (target_b <= 255)));
                
                printf("Enter threshold for the color difference: ");
                scanf("%d", &threshold);

                printf("Enter value for Red component in the target color: ");
                scanf("%d", &replace_r);
                printf("Enter value for Green component in the target color: ");
                scanf("%d", &replace_g);
                printf("Enter value for Blue component in the target color: ");
                scanf("%d", &replace_b);

                // call function
                ColorFilter(R, G, B, target_r, target_g, target_b, threshold,
                            replace_r, replace_g, replace_b);

                printf("\"Color Filter\" operation is done!\n");
                break;
            case 6:
                Edge(R, G, B);
                printf("\"Edge\" operation is done!\n");
                break;
            case 7:
                Shuffle(R, G, B);
                printf("\"Shuffle\" operation is done!\n");
                break;
            case 8:
                VFlip(R, G, B);
                printf("\"VFlip\" operation is done!\n");
                break;
            case 9:
                HMirror(R, G, B);
                printf("\"HMirror\" operation is done!\n");
                break;
            case 10:
                // pixelate image would go here if it was implemented
                break;
            case 11:
                AutoTest(R, G, B);
                break;
            default:
                break;
        }

        // print menu
        PrintMenu();
    }
    
    printf("\n");

    /* End of replacing */

    return 0;
}

int LoadImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    FILE *File;
    char Type[SLEN];
    int  Width, Height, MaxValue;
    int  x, y;
    char ftype[] = ".ppm";
    char fname_ext[SLEN + sizeof(ftype)];

    strcpy(fname_ext, fname);
    strcat(fname_ext, ftype);

    File = fopen(fname_ext, "r");
    if (!File) {
        printf("\nCannot open file \"%s\" for reading!\n", fname);
        return 1;
    }
    fscanf(File, "%79s", Type);
    if (Type[0] != 'P' || Type[1] != '6' || Type[2] != 0) {
        printf("\nUnsupported file format!\n");
        return 2;
    }
    fscanf(File, "%d", &Width);
    if (Width != WIDTH) {
        printf("\nUnsupported image width %d!\n", Width);
        return 3;
    }
    fscanf(File, "%d", &Height);
    if (Height != HEIGHT) {
        printf("\nUnsupported image height %d!\n", Height);
        return 4;
    }
    fscanf(File, "%d", &MaxValue);
    if (MaxValue != 255) {
        printf("\nUnsupported image maximum value %d!\n", MaxValue);
        return 5;
    }
    if ('\n' != fgetc(File)) {
        printf("\nCarriage return expected!\n");
        return 6;
    }
    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            R[x][y] = fgetc(File);
            G[x][y] = fgetc(File);
            B[x][y] = fgetc(File);
        }
    }
    if (ferror(File)) {
        printf("\nFile error while reading from file!\n");
        return 7;
    }
    printf("%s was read successfully!\n", fname_ext);
    fclose(File);
    return 0;
}

int SaveImage(const char fname[SLEN], unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT])
{
    FILE *File;
    char ftype[] = ".ppm";
    char fname_ext[SLEN + sizeof(ftype)];
    char SysCmd[SLEN * 5];
    int  x, y;

    strcpy(fname_ext, fname);
    strcat(fname_ext, ftype);

    File = fopen(fname_ext, "w");
    if (!File) {
        printf("\nCannot open file \"%s\" for writing!\n", fname);
        return 1;
    }
    fprintf(File, "P6\n");
    fprintf(File, "%d %d\n", WIDTH, HEIGHT);
    fprintf(File, "255\n");

    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            fputc(R[x][y], File);
            fputc(G[x][y], File);
            fputc(B[x][y], File);
        }
    }

    if (ferror(File)) {
        printf("\nFile error while writing to file!\n");
        return 2;
    }
    fclose(File);
    printf("%s was saved successfully. \n", fname_ext);

    /*
     * rename file to image.ppm, convert it to ~/public_html/<fname>.jpg
     * and make it world readable
     */
    sprintf(SysCmd, "~eecs22/bin/pnmtojpeg_hw2.tcsh %s", fname_ext);
    if (system(SysCmd) != 0) {
        printf("\nError while converting to JPG:\nCommand \"%s\" failed!\n", SysCmd);
        return 3;
    }
    printf("%s.jpg was stored for viewing. \n", fname);

    return 0;
}

/*DO NOT EDIT AUTOTEST*/
/*DO NOT EDIT AUTOTEST*/
void AutoTest(unsigned char R[WIDTH][HEIGHT], unsigned char G[WIDTH][HEIGHT], unsigned char B[WIDTH][HEIGHT]) {
	char fname[SLEN] = "EngPlaza";
	char sname[SLEN];

	LoadImage(fname, R, G, B);
	Negative(R, G, B);
	strcpy(sname, "negative");
	SaveImage(sname, R, G, B);
	printf("Negative tested!\n\n");

	LoadImage(fname, R, G, B);
	ColorFilter(R, G, B, 130, 130, 150, 30, 0, 255, 255);
	strcpy(sname, "colorfilter");
	SaveImage(sname, R, G, B);
	printf("Color Filter tested!\n\n");

	LoadImage(fname, R, G, B);
	Edge(R, G, B);
	strcpy(sname, "edge");
	SaveImage(sname, R, G, B);
	printf("Edge Detection tested!\n\n");

	LoadImage(fname, R, G, B);
	HMirror(R, G, B);
	strcpy(sname, "hmirror");
	SaveImage(sname, R, G, B);
	printf("HMirror tested!\n\n");
	
	LoadImage(fname, R, G, B);
	BlackNWhite(R, G, B);
	strcpy(sname, "bw");
	SaveImage(sname, R, G, B);
	printf("Black & White tested!\n\n");

	LoadImage(fname, R, G, B);
	VFlip(R, G, B);
	strcpy(sname, "vflip");
	SaveImage(sname, R, G, B);
	printf("VFlip tested!\n\n");

	LoadImage(fname, R, G, B);
	Shuffle(R, G, B);
	strcpy(sname, "shuffle");
	SaveImage(sname, R, G, B);
	printf("Shuffle tested!\n\n");

}

/**************************************************************/
/* Please add your function definitions here...               */
/**************************************************************/

// print the menu
void PrintMenu(void)
{
	printf("--------------------------------\n");
	printf("1: Load a PPM image\n");
	printf("2: Save an image in PPM and JPEG format\n");
	printf("3: Change a color image to Black & White\n");
	printf("4: Make a negative of an image\n");
	printf("5: Color filter an image\n");
	printf("6: Sketch the edge of an image\n");
	printf("7: Shuffle an image\n");
	printf("8: Flip an image vertically\n");
	printf("9: Mirror an image horizontally\n");
	printf("10: Pixelate an image\n");
	printf("11: Test all functions\n");
	printf("12: Exit\n");
	printf("Please make your choice: ");	
	return;
}

// change color image to black and white
void BlackNWhite(unsigned char R[WIDTH][HEIGHT],
		 unsigned char G[WIDTH][HEIGHT],
		 unsigned char B[WIDTH][HEIGHT])
{
    int i, j;
    unsigned char greyscale;

    // loop through each pixel and replace it with average of r, g, b values
    for (i = 0; i < WIDTH; ++i)
    {
        for (j = 0; j < HEIGHT; ++j)
        {
            // new grey intensity
            greyscale = (R[i][j] + G[i][j] + B[i][j]) / 3;

            // assign to each color
            R[i][j] = greyscale;
            G[i][j] = greyscale;
            B[i][j] = greyscale;
        }
    }

    SaveImage("bw", R, G, B);

    return;
}

// reverse image color
void Negative(unsigned char R[WIDTH][HEIGHT],
	      unsigned char G[WIDTH][HEIGHT],
	      unsigned char B[WIDTH][HEIGHT])
{
    int i, j;

    // loop through each pixel
    for (i = 0; i < WIDTH; ++i)
    {
        for (j = 0; j < HEIGHT; ++j)
        {
            // replace each pixel with its "negative" value
            R[i][j] = 255 - R[i][j];
            G[i][j] = 255 - G[i][j];
            B[i][j] = 255 - B[i][j];
        }
    }

    SaveImage("negative", R, G, B);

    return;
}

// apply a color filter
void ColorFilter(unsigned char R[WIDTH][HEIGHT],
		 unsigned char G[WIDTH][HEIGHT],
                 unsigned char B[WIDTH][HEIGHT],
		 int target_r, int target_g, int target_b, int threshold,
		 int replace_r, int replace_g, int replace_b)
{
    int i, j;

    // loop through each pixel
    for (i = 0; i < WIDTH; ++i)
    {
        for (j = 0; j < HEIGHT; ++j)
        {
            // check if the pixel's RGB values are within the threshold
            if ((R[i][j] >= (target_r - threshold)) && (R[i][j] <= (target_r + threshold))
             && (G[i][j] >= (target_g - threshold)) && (G[i][j] <= (target_g + threshold))
             && (B[i][j] >= (target_b - threshold)) && (B[i][j] <= (target_b + threshold)))
            {
                // if so, replace the color values
                R[i][j] = replace_r;
                G[i][j] = replace_g;
                B[i][j] = replace_b;
            }
        }
    }

    SaveImage("colorfilter", R, G, B);

    return;
}

// edge detection
void Edge(unsigned char R[WIDTH][HEIGHT],
	      unsigned char G[WIDTH][HEIGHT],
          unsigned char B[WIDTH][HEIGHT])
{
    int i, j;
    int intR, intG, intB;
    int intensityR[WIDTH][HEIGHT];
    int intensityG[WIDTH][HEIGHT];
    int intensityB[WIDTH][HEIGHT];
    
    
    for (i = 0; i < WIDTH; ++i)
    {
        for (j = 0; j < HEIGHT; ++j)
        {
            // set border values to 0 intensity   
            if ((i == 0) || (i == WIDTH - 1) || (j == 0) || (j == HEIGHT - 1))
            {
                intensityR[i][j] = 0;
                intensityG[i][j] = 0;
                intensityB[i][j] = 0;
                continue;
            }

            // calculate intensity values using surrounding pixels
            intR = -1*R[i-1][j-1] + -1*R[i][j-1] + -1*R[i+1][j-1] + -1*R[i-1][j] + 8*R[i][j] + -1*R[i+1][j] + -1*R[i-1][j+1] + -1*R[i][j+1] + -1*R[i+1][j+1];
            intG = -1*G[i-1][j-1] + -1*G[i][j-1] + -1*G[i+1][j-1] + -1*G[i-1][j] + 8*G[i][j] + -1*G[i+1][j] + -1*G[i-1][j+1] + -1*G[i][j+1] + -1*G[i+1][j+1];
            intB = -1*B[i-1][j-1] + -1*B[i][j-1] + -1*B[i+1][j-1] + -1*B[i-1][j] + 8*B[i][j] + -1*B[i+1][j] + -1*B[i-1][j+1] + -1*B[i][j+1] + -1*B[i+1][j+1];

            // set any value larger than 255 to 255 and less than 0 to 0
            intR = (intR > 255) ? 255 : (intR < 0) ? 0 : intR;
            intG = (intG > 255) ? 255 : (intG < 0) ? 0 : intG;
            intB = (intB > 255) ? 255 : (intB < 0) ? 0 : intB;

            // assign intensity values to temporary arrays
            intensityR[i][j] = intR;
            intensityG[i][j] = intG;
            intensityB[i][j] = intB;
        }
    }

    // assign values from temporary arrays into R, G, B
    for (i = 0; i < WIDTH; ++i)
    {
        for (j = 0; j < HEIGHT; ++j)
        {
            R[i][j] = intensityR[i][j];
            G[i][j] = intensityG[i][j];
            B[i][j] = intensityB[i][j];
        }
    }

    SaveImage("edge", R, G, B);
    return;
}

// shuffle an image
void Shuffle(unsigned char R[WIDTH][HEIGHT],
	         unsigned char G[WIDTH][HEIGHT],
             unsigned char B[WIDTH][HEIGHT])
{
    /* 
        i and j loop through the big blocks, k and l
        loop through the individual pixels 
    */ 
    int i, j, k, l;
    
    // create 4D arrays for original RGB values
    unsigned char origR[4][4][WIDTH / 4][HEIGHT / 4];
    unsigned char origG[4][4][WIDTH / 4][HEIGHT / 4];
    unsigned char origB[4][4][WIDTH / 4][HEIGHT / 4];

    // create 4D arrays for copy of each block
    unsigned char copyR[4][4][WIDTH / 4][HEIGHT / 4];
    unsigned char copyG[4][4][WIDTH / 4][HEIGHT / 4];
    unsigned char copyB[4][4][WIDTH / 4][HEIGHT / 4];

    // copy original RGB values into 4D array version (easier to work with)
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            for (k = 0; k < (WIDTH / 4); ++k)
            {
                for (l = 0; l < (HEIGHT / 4); ++l)
                {
                    origR[i][j][k][l] = R[i * (WIDTH / 4) + k][j * (HEIGHT / 4) + l];
                    origG[i][j][k][l] = G[i * (WIDTH / 4) + k][j * (HEIGHT / 4) + l];
                    origB[i][j][k][l] = B[i * (WIDTH / 4) + k][j * (HEIGHT / 4) + l];
                }
            }
        }
    }

    // perform swap operation
    for (i = 0; i < 2; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            for (k = 0; k < (WIDTH / 4); ++k)
            {
                for (l = 0; l < (HEIGHT / 4); ++l)
                {
                    // copy first block
                    copyR[i][j][k][l] = origR[i][j][k][l];
                    copyG[i][j][k][l] = origG[i][j][k][l];
                    copyB[i][j][k][l] = origB[i][j][k][l];

                    // swap second block into first block
                    origR[i][j][k][l] = origR[3 - i][3 - j][k][l];
                    origG[i][j][k][l] = origG[3 - i][3 - j][k][l];
                    origB[i][j][k][l] = origB[3 - i][3 - j][k][l];

                    // put the copy of first block into second block
                    origR[3 - i][3 - j][k][l] = copyR[i][j][k][l];
                    origG[3 - i][3 - j][k][l] = copyG[i][j][k][l];
                    origB[3 - i][3 - j][k][l] = copyB[i][j][k][l];
                }
            }
        }
    }

    // copy values from 4D array back into original RGB array
    for (i = 0; i < 4; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            for (k = 0; k < (WIDTH / 4); ++k)
            {
                for (l = 0; l < (HEIGHT / 4); ++l)
                {
                    R[i * (WIDTH / 4) + k][j * (HEIGHT / 4) + l] = origR[i][j][k][l];
                    G[i * (WIDTH / 4) + k][j * (HEIGHT / 4) + l] = origG[i][j][k][l];
                    B[i * (WIDTH / 4) + k][j * (HEIGHT / 4) + l] = origB[i][j][k][l];
                }
            }
        }
    }

    SaveImage("shuffle", R, G, B);
    return;
}

// vertically flip an image
void VFlip(unsigned char R[WIDTH][HEIGHT],
	       unsigned char G[WIDTH][HEIGHT],
           unsigned char B[WIDTH][HEIGHT])
{
    int i, j;
    unsigned char flipR[WIDTH][HEIGHT];
    unsigned char flipG[WIDTH][HEIGHT];
    unsigned char flipB[WIDTH][HEIGHT];

    // iterate through width normally
    for (i = 0; i < WIDTH; ++i)
    {
        // iterate through height from the bottom up
        for (j = HEIGHT - 1; j >= 0; --j)
        {
            // switch vertical coordinate of intensity with the one across from it
            flipR[i][j] = R[i][HEIGHT - j - 1];
            flipG[i][j] = G[i][HEIGHT - j - 1];
            flipB[i][j] = B[i][HEIGHT - j - 1];
        }
    }

    // copy temporary intensity array into original R, G, B arrays
    for (i = 0; i < WIDTH; ++i)
    {
        for (j = 0; j < HEIGHT; ++j)
        {
            R[i][j] = flipR[i][j];
            G[i][j] = flipG[i][j];
            B[i][j] = flipB[i][j];
        }
    }

    SaveImage("vflip", R, G, B);
    return;
}

// horizontally mirror an image
void HMirror(unsigned char R[WIDTH][HEIGHT],
	         unsigned char G[WIDTH][HEIGHT],
             unsigned char B[WIDTH][HEIGHT])
{
    int i, j;
    unsigned char flipR[WIDTH][HEIGHT];
    unsigned char flipG[WIDTH][HEIGHT];
    unsigned char flipB[WIDTH][HEIGHT];

    // iterate through width from left to halfway point
    for (i = 0; i < (WIDTH / 2); ++i)
    {
        // iterate through height normally
        for (j = 0; j < HEIGHT; ++j)
        {
            // copy original array into temporary array
            flipR[i][j] = R[i][j];
            flipG[i][j] = G[i][j];
            flipB[i][j] = B[i][j];
        }
    }

    // copy temporary intensity array into original R, G, B arrays
    // reverse left side of image onto right side
    for (i = WIDTH - 1; i >= (WIDTH / 2); --i)
    {
        for (j = 0; j < HEIGHT; ++j)
        {
            R[i][j] = flipR[WIDTH - 1 - i][j];
            G[i][j] = flipG[WIDTH - 1 - i][j];
            B[i][j] = flipB[WIDTH - 1 - i][j];
        }
    }
    
    SaveImage("hmirror", R, G, B);
    return;
}

/* EOF */
