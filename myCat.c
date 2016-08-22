// for puts
#include <stdio.h>

// for exit
#include <stdlib.h>

// for open()
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// for close
#include <unistd.h>

#include "readlines.h"
#include "assert2.h"


int main(int argc, char **argv) {

  int fd;
  char **lines, **pLines;

  /* checked if invoked correctly */
  if (argc != 2) {		/* one parameter expected */
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    exit(1);
  }

  /* read lines from input file */
  fd = open(argv[1], O_RDONLY);
  assert2(fd >= 0, "Can't open input file.");
  lines = readLines(fd);
  close(fd);

  /* print lines */
  for (pLines = lines; *pLines; pLines++) { 
    puts(*pLines);
    free(*pLines);
  }
  free(lines);
  return 0;
}

  
