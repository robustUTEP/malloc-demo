all: myCat

CFLAGS = -g -O2

myCat: myCat.o strcopy.o readlines.o
	cc -o myCat myCat.o strcopy.o readlines.o
clean:
	rm -f  *.o myCat myCat.exe
