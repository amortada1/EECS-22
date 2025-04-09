#include <stdio.h>
#include <string.h>

#include "Test.h"
#include "Constants.h"
#include "FileIO.h"
#include "Image.h"
#include "DIPs.h"
#include "Advanced.h"

/* print the menu */
void PrintMenu();

int main(void)
{
    // DEBUG mode
	#ifdef DEBUG

	printf("DEBUG mode is on, running AutoTest()...\n\n");
	AutoTest();
	return 0;
	
	#else // otherwise execute program normally
	
	int option;
    char fname[SLEN];		/* input file name */
	char colorOption[SLEN];

    PrintMenu();
	printf("Please make your choice: ");
	scanf("%d", &option);

    Image *image = NULL;			/* Image object */
	Image *watermark_image = NULL;  /* Watermark image object */
	/* ColorFilter() parameters */
	int target_r, target_g, target_b, threshold;
	double factor_r, factor_g, factor_b;
	/* AddBorder() parameter */
	int border_width;
	/* Pixelate() parameter */
	int block_size;
	/* Shift() parameters */
	int shiftX, shiftY;
	/* Crop() parameters */
	int x, y, w, h;
	/* Resize() parameters */
	int newWidth, newHeight;
	/* RotateBy90() parameter */
	int rotateDirection;

    while (option != EXIT) {
		if (option == 1) {
			printf("Please input the file name to load: ");
			scanf("%75s", fname);
			image = LoadImage(fname);
		}
		/* menu item 2 - 16 requires image is loaded first */
		else if (option >= 2 && option <= 16) {
			if (image == NULL)	 {
				printf("No image is read.\n");
			}
			/* now image is loaded */
			else {
				switch(option) {
					case 2:
						printf("Please input the file name to save: ");
						scanf("%75s", fname);
						SaveImage(fname, image);
						break;
					case 3:
						image = BlackNWhite(image);
						printf("\"Black & White\" operation is done!\n");
						break;
					case 4:
						image = Negative(image);
						printf("\"Negative\" operation is done!\n");
						break;
					case 5:
						printf("Enter Red   component for the target color: ");
						scanf("%d", &target_r);
						printf("Enter Green component for the target color: ");
						scanf("%d", &target_g);
						printf("Enter Blue  component for the target color: ");
						scanf("%d", &target_b);
						printf("Enter threshold for the color difference: ");
						scanf("%d", &threshold);
						printf("Enter value for Red component in the target color: ");
						scanf("%lf", &factor_r);
						printf("Enter value for Green component in the target color: ");
						scanf("%lf", &factor_g);
						printf("Enter value for Blue  component in the target color: ");
						scanf("%lf", &factor_b);
						image = ColorFilter(image, target_r, target_g, target_b, threshold, factor_r, factor_g, factor_b);
						printf("\"Color Filter\" operation is done!\n");
						break;
					case 6:
						image = Edge(image);
						printf("\"Edge\" operation is done!\n");
						break;
					case 7:
						image = Shuffle(image);
						printf("\"Shuffle\" operation is done!\n");
						break;
					case 8:
						image = VFlip(image);
						printf("\"VFlip\" operation is done!\n");
						break;
					case 9:
						image = HMirror(image);
						printf("\"HMirror\" operation is done!\n");
						break;
					case 10:
						printf("Enter border width:");
						scanf("%d", &border_width);
						printf("Available border colors : black, white, red, green, blue, yellow, cyan, pink, orange\n");
						printf("Select border color from the options: ");
						scanf("%s",colorOption);
						image = AddBorder(image, colorOption, border_width);
						printf("\"Border\" operation is done!\n");
						break;
					case 11:
						printf("Enter block size: ");
						scanf("%d", &block_size);
						image = Pixelate(image, block_size);
						printf("\"Pixelate\" operation is done!\n");
						break;
					case 12:
						printf("Enter the amount to shift in the X direction: ");
						scanf("%d", &shiftX);
						printf("Enter the amount to shift in the Y direction: ");
						scanf("%d", &shiftY);
						image = Shift(image, shiftX, shiftY);
						printf("\"Shift\" operation is done!\n");
						break;
					case 13:
						printf("Please enter the X offset value: ");
						scanf("%d", &x);
						printf("Please enter the Y offset value: ");
						scanf("%d", &y);
						printf("Please input the crop width: ");
						scanf("%d", &w);
						printf("Please input the crop height: ");
						scanf("%d", &h);
						image = Crop(image, x, y, w, h);
						printf("\"Crop\" operation is done!\n");
						break;
					case 14:
						printf("Please input the new image width: ");
						scanf("%d", &newWidth);
						printf("Please input the new image height: ");
						scanf("%d", &newHeight);
						image = Resize(image, newWidth, newHeight);
						printf("\"Resizing the image\" operation is done!\n");
						break;
					case 15:
						watermark_image = LoadImage("watermark_template");
						image = Watermark(image, watermark_image);
						DeleteImage(watermark_image);
						printf("\"Watermark\" operation is done!\n");
						break;
					case 16:
						printf("Please imput the direction of rotation (0: clockwise, 1: counterclockwise): ");
						scanf("%d", &rotateDirection);
						image = RotateBy90(image, rotateDirection);
						printf("\"RotateBy90\" operation is done!\n");
						break;
					default:
						break;
				}
			}
		}
		else if (option == 17) {
			AutoTest();
			//r24 = SUCCESS;	/* set returned code SUCCESS, since image is loaded */
		}
		else {
			printf("Invalid selection!\n");
		}

		/* Process finished, waiting for another input */
		PrintMenu();
		printf("Please make your choice: ");
		scanf("%d", &option);
	}

	#endif

	printf("You exit the program.\n");
	return 0;
}

void PrintMenu() {
	printf("\n----------------------------\n");
	printf(" 1: Load a PPM image\n");
	printf(" 2: Save an image in PPM and JPEG format\n");
	printf(" 3: Change a color image to Black & White\n");
	printf(" 4: Make a negative of an image\n");
	printf(" 5: Color filter an image\n");
	printf(" 6: Sketch the edge of an image\n");
	printf(" 7: Shuffle an image\n");
	printf(" 8: Flip an image vertically\n");
	printf(" 9: Mirror an image horizontally\n");
	printf("10: Add border to an image\n");
	printf("11: Pixelate an image\n");
	printf("12: Shift an image\n");
	printf("13: Crop an image\n");
	printf("14: Resize an image\n");
    printf("15: Add Watermark to an image\n");
    printf("16: Rotate an image by 90 degrees\n");
	printf("17: Test all functions\n");
	printf("18: Exit\n");
}