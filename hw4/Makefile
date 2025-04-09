# Makefile: PhotoLab

# macro definitions
CC = gcc
CFLAGS = -Wall -std=c11 -g

# convenience targets
all: PhotoLab

clean: 
	rm -f *.o
	rm -f PhotoLab
	rm -f PhotoLabTest

run: PhotoLab
	./PhotoLab

test: PhotoLabTest
	./PhotoLabTest

# compilation rules
Image.o: Image.c Image.h Constants.h
	$(CC) $(CFLAGS) -c Image.c -o Image.o

FileIO.o: FileIO.c FileIO.h Image.h Constants.h
	$(CC) $(CFLAGS) -c FileIO.c -o FileIO.o

DIPs.o: DIPs.c DIPs.h Image.h Constants.h
	$(CC) $(CFLAGS) -c DIPs.c -o DIPs.o

Advanced.o: Advanced.c Advanced.h Image.h Constants.h
	$(CC) $(CFLAGS) -c Advanced.c -o Advanced.o

Test.o: Test.c Test.h Image.h FileIO.h DIPs.h Advanced.h
	$(CC) $(CFLAGS) -c Test.c -o Test.o

PhotoLab: Image.o FileIO.o DIPs.o Advanced.o Test.o
	$(CC) $(CFLAGS) Image.o FileIO.o DIPs.o Advanced.o Test.o PhotoLab.c -o PhotoLab -DNEBUG

PhotoLabTest: Image.o FileIO.o DIPs.o Advanced.o Test.o
	$(CC) $(CFLAGS) Image.o FileIO.o DIPs.o Advanced.o Test.o PhotoLab.c -o PhotoLabTest -DDEBUG