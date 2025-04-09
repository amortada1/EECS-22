#######################################################################
# Homework Assignment 5, for EECS 22, Winter 2024
#
# Initial author: Tim Schmidt
#
# Makefile: Makefile for HW 5
#
#######################################################################

# Variables
CC = gcc
CFLAGS = -Wall -std=c11 -g -c
LFLAGS = -Wall -std=c11 -g -lm

# Default target
default: MovieLab

run: default
	./MovieLab -i EngPlaza_512_288.yuv -o out.yuv -f 60 -s 512x288 -bw

test: default
	valgrind --leak-check=full --track-origins=yes --show-leak-kinds=all -s ./MovieLab -i EngPlaza_512_288.yuv -o crop.yuv -f 60 -crop 10-20

yay: 
	~eecs22/bin/yay -s 512x288 out.yuv

ref:
	~eecs22/bin/yay -s 512x288 ~eecs22/public/hw5/demo/rotate.yuv

######################### Generate object files #######################
# Target for MovieLab.o
MovieLab.o: MovieLab.c Constants.h FileIO.h Image.h DIPs.h Movie.h
	$(CC) $(CFLAGS) MovieLab.c -o MovieLab.o

# Target for FileIO.o
FileIO.o: FileIO.c FileIO.h Constants.h Image.h
	$(CC) $(CFLAGS) FileIO.c -o FileIO.o

# Target for DIPs.o
DIPs.o: DIPs.c DIPs.h Constants.h Image.h
	$(CC) $(CFLAGS) DIPs.c -o DIPs.o

# Target for Image.o
Image.o: Image.c Image.h 
	$(CC) $(CFLAGS) Image.c -o Image.o

# Target for ImageList.o
ImageList.o: ImageList.c ImageList.h Image.h
	$(CC) $(CFLAGS) ImageList.c -o ImageList.o

# Target for Movie.o
Movie.o: Movie.c Movie.h ImageList.h
	$(CC) $(CFLAGS) Movie.c -o Movie.o

######################### Generate the executable #####################
# Target for MovieLab
MovieLab: MovieLab.o FileIO.o DIPs.o Image.o ImageList.o Movie.o
	$(CC) $(LFLAGS) MovieLab.o FileIO.o DIPs.o Image.o ImageList.o Movie.o -o MovieLab

###############################  others  ##############################
# Target for clean-up
clean:
	rm -f *.o
	rm -f MovieLab
	rm -f out.yuv