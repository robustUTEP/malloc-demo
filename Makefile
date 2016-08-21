all: printfile

CFLAGS = -g -O0

printfile: malloc-demo.o strcopy.o readlines.o
	cc -o printfile malloc-demo.o strcopy.o readlines.o
clean:
	rm -f  *.o printfile*
