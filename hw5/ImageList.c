/*********************************************************************/
/* Homework Assignment 5, for EECS 22, Winter 2024                   */
/* ImageList.c: source file for image list manipulations             */
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ImageList.h"

/* Create a new image list */
ILIST *CreateImageList(void)
{
	// create list and allocate memory for it
	ILIST *l = (ILIST *)malloc(sizeof(ILIST));

	// if list was not created, exit program
	if (!l)
	{
		perror("Out of memory! Aborting...\n");
		exit(10);
	}

	// otherwise initalize the values
	l->Length = 0;
	l->First = NULL;
	l->Last = NULL;

	return l;
}

/* Helper function for DeleteImageList() */
static Image *DeleteImageEntry(IENTRY *e)
{
	assert(e);
	Image *image = e->image;
	free(e);
	e = NULL;
	return image;
}

/* Delete an image list (and all entries) */
void DeleteImageList(ILIST *list)
{
	IENTRY *e, *n;
	Image *image;

	// make sure list exists
	assert(list);

	// if it does, delete all entries
	e = list->First;
	while (e)
	{
		n = e->Next;
		image = DeleteImageEntry(e);
		DeleteImage(image);
		e = n;
	}

	// finally, delete list
	free(list);
	list = NULL;
}

/* Helper function for AppendRGBImage() */
static IENTRY *NewRGBImageEntry(Image *image)
{
	// create new image entry
	IENTRY *e = (IENTRY *)malloc(sizeof(IENTRY));

	// if entry was not created, exit program
	if (!e)
	{
		perror("Out of memory! Aborting...\n");
		exit(10);
	}

	// otherwise initialize values
	e->List = NULL;
	e->Next = NULL;
	e->Prev = NULL;
	e->image = image;

	return e;
}

/* Insert a RGB image to the image list at the end */
void AppendRGBImage(ILIST *list, Image *RGBimage)
{
	// make sure list and image exist
	assert(list);
	assert(RGBimage);

	IENTRY *e = NewRGBImageEntry(RGBimage);
	// if list has any components, add it to the end
	if (list->Last)
	{
		e->List = list;
		e->Next = NULL;
		e->Prev = list->Last;
		list->Last->Next = e;
		list->Last = e;
	}
	else // otherwise add it as the singular component
	{
		e->List = list;
		e->Next = NULL;
		e->Prev = NULL;
		list->First = e;
		list->Last = e;
	}

	list->Length++;
}

/* Helper function for AppendYUVImage() */
static IENTRY *NewYUVImageEntry(YUVImage *image)
{
	// create new image entry
	IENTRY *e = (IENTRY *)malloc(sizeof(IENTRY));

	// if entry was not created, exit program
	if (!e)
	{
		perror("Out of memory! Aborting...\n");
		exit(10);
	}

	// otherwise initialize values
	e->List = NULL;
	e->Next = NULL;
	e->Prev = NULL;
	e->image = image;

	return e;
}

/* Insert a YUV image to the image list at the end */
void AppendYUVImage(ILIST *list, YUVImage *YUVimage)
{
	// make sure list and image exist
	assert(list);
	assert(YUVimage);

	IENTRY *e = NewYUVImageEntry(YUVimage);
	// if list has any components, add it to the end
	if (list->Last)
	{
		e->List = list;
		e->Next = NULL;
		e->Prev = list->Last;
		list->Last->Next = e;
		list->Last = e;
	}
	else // otherwise add it as the singular component
	{
		e->List = list;
		e->Next = NULL;
		e->Prev = NULL;
		list->First = e;
		list->Last = e;
	}

	list->Length++;
}

/* Crop an image list */
void CropImageList(ILIST *list, unsigned int start, unsigned int end)
{
	assert(list);

	IENTRY *imgEntry, *nextEntry;
	imgEntry = list->First;
	unsigned int index = 0;

	while (imgEntry != NULL)
	{
		nextEntry = imgEntry->Next;

		// check if entry is outside cropping range
		if (index < start || index > end) 
		{
			// remove entry from list
			if (imgEntry->Prev)
				imgEntry->Prev->Next = imgEntry->Next;
			else
				list->First = imgEntry->Next;

			if (imgEntry->Next)
				imgEntry->Next->Prev = imgEntry->Prev;
			else
				list->Last = imgEntry->Prev;

			// delete image and entry
			DeleteImage(imgEntry->image);
			DeleteImageEntry(imgEntry);

			list->Length--;
		}

		imgEntry = nextEntry;
		++index;
	}
}

/* Fast forward an image list */
void FastImageList(ILIST *list, unsigned int factor)
{
	assert(list);
	if (factor <= 1) return;

	IENTRY *imgEntry = list->First;

	while (imgEntry != NULL)
	{
		// delete next (factor - 1) entries
		for (int x = 0; x < factor - 1; ++x)
		{
			IENTRY* toDelete = imgEntry->Next;
			if (toDelete == NULL) break;

			imgEntry->Next = toDelete->Next;
			if (toDelete->Next)
				toDelete->Next->Prev = imgEntry;
			else
				list->Last = imgEntry;

			DeleteImageEntry(toDelete);
			list->Length--;
		}

		imgEntry = imgEntry->Next;
	}
}

/* Reverse an image list */
void ReverseImageList(ILIST *list)
{
	assert(list);

	IENTRY *imgEntry = list->First;
	IENTRY *temp = NULL;

	while (imgEntry != NULL)
	{
		// swap current entry's next and prev pointers
		temp = imgEntry->Prev;
		imgEntry->Prev = imgEntry->Next;
		imgEntry->Next = temp;

		// move to next entry
		imgEntry = imgEntry->Prev;
	}

	// swap first and last pointers
	temp = list->First;
	list->First = list->Last;
	list->Last = temp;
}

/* Insert a RGB image to the image list in the front */
void PrependRGBImage(ILIST *list, Image *RGBimage)
{
	// make sure list and image exist
	assert(list);
	assert(RGBimage);

	IENTRY *e = NewRGBImageEntry(RGBimage);
	// if list has any components, add it to the front
	if (list->First)
	{
		e->List = list;
		e->Prev = NULL;
		e->Next = list->First;
		list->First->Prev = e;
		list->First = e;
	}
	else // otherwise add it as the singular component
	{
		e->List = list;
		e->Next = NULL;
		e->Prev = NULL;
		list->First = e;
		list->Last = e;
	}

	list->Length++;
}


/* EOF */
