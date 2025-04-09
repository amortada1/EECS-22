# Makefile: PhotoLab_v2

# convenience targets
all: PhotoLab

clean:
	rm -f *.o
	rm -f PhotoLab
	rm -f PhotoLabTest

test: PhotoLab
	./PhotoLab

# compilation rules

FileIO.o: FileIO.c FileIO.h Constants.h
	gcc -Wall -std=c11 -c FileIO.c -o FileIO.o

DIPs.o: DIPs.c DIPs.h Constants.h
	gcc -Wall -std=c11 -c DIPs.c -o DIPs.o

Advanced.o: Advanced.c Advanced.h Constants.h
	gcc -Wall -std=c11 -c Advanced.c -o Advanced.o

libfilter.a: DIPs.o
	ar rc libfilter.a DIPs.o
	ranlib libfilter.a
	
PhotoLab: FileIO.o DIPs.o Advanced.o libfilter.a
	gcc -Wall -std=c11 FileIO.o DIPs.o Advanced.o PhotoLab.c -lfilter -L. -lm -o PhotoLab 

PhotoLabTest: FileIO.o DIPs.o Advanced.o libfilter.a
	gcc -Wall -std=c11 FileIO.o DIPs.o Advanced.o PhotoLab.c -lfilter -L. -lm  -o PhotoLabTest -DDEBUG
