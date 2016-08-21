
#include <sys/types.h>		/* for lseek, read */
#include <sys/stat.h>		/* for read */
#include <fcntl.h>		/* for read */
#include <unistd.h>		/* for read */
#include <stdlib.h>		/* for malloc */

#include <assert.h>		/* for assert */
#include "assert2.h"		/* for assert2 */
#include "strcopy.h"  		/* for strcopy */
#include "readlines.h"		/* for consistency */

#define BUFLEN 4096		/* read buffer length */

// Count the number of lines in open file fd.
// Also computes maximum line length
int countLines(int fd, int *pMaxLineLen)
{
  char buf[BUFLEN], *pBuf, *pBufLimit; /* read buffer */
  char lastChar;		       /* last character read */
  int numBytesRead;		       /* from last read */
  int numLines = 0, lineLen = 0, maxLineLen = 0;
  
  while(numBytesRead = read(fd, &buf, BUFLEN)) {
    assert2(numBytesRead >= 0, "error reading file");
    pBufLimit = buf + numBytesRead; // immediately following last byte read 
    for (pBuf = buf; pBuf != pBufLimit; pBuf++) { // ...every character in buf
      lastChar = *pBuf;
      if (lastChar == '\n') {
	numLines++;
	maxLineLen = lineLen > maxLineLen ? lineLen : maxLineLen;
	lineLen = 0;
      } else
	lineLen ++;
    }
  }
  if (lastChar != '\n') { // notice if last line isn't terminated
    numLines++;
    maxLineLen = lineLen > maxLineLen ? lineLen : maxLineLen;
  }
  if (pMaxLineLen) *pMaxLineLen = maxLineLen;
  return numLines;
}

// Reads lines from open fd
// Fd must be seekable
char **readLines(int fd)
{
  int numBytesRead, numLines, maxLineLen, numLinesRead = 0;
  char readBuf[BUFLEN], *pReadBuf, *pReadBufLimit, lastChar;
  char **lines, *lineBuf, **pLines, **pLinesLimit;
  off_t initialFileOffset;

  /* save initial file offset */
  assert2((initialFileOffset = lseek(fd, 0, SEEK_CUR)) >= 0, 
	  "fd must be seekable");

  numLines = countLines(fd, &maxLineLen); // count number of lines & maximum line length

  /* allocate vector to store words */
  lines = (char **)malloc(sizeof(char*) * (numLines + 1)); 
  pLinesLimit = lines + numLines;
  assert2(lines != 0, "can't allocate memory to store vector of input lines");
  lines[numLines] = 0;		/* terminate */

  /* allocate line buffer */
  assert((lineBuf = (char *)malloc(maxLineLen+1)) != 0);
  
  /* re-initialize file offset  */
  assert2(lseek(fd, initialFileOffset, SEEK_SET) == initialFileOffset,
	  "fd must be seekable");

  for (pLines = lines; pLines < pLinesLimit; pLines++) { // for each line 
    char *pLineBuf = lineBuf;
    while(numBytesRead = read(fd, &readBuf, BUFLEN)) {
      assert2(numBytesRead >= 0, "error reading input file");
      char *pReadBufLimit = readBuf + numBytesRead;
      for (pReadBuf = readBuf; pReadBuf != pReadBufLimit; pReadBuf++) {
	lastChar = *pReadBuf;
	if (lastChar == '\n') {	// end of word found
	  *pLineBuf = 0;	// terminate word 
	  *(pLines++) = strcopy(lineBuf);
	  pLineBuf = lineBuf;
	} else
	  *(pLineBuf++) = lastChar;
      }
      if (lastChar != '\n') {	// last line isn't terminated
	*pLineBuf = 0;	// terminate word 
	*(pLines++) = strcopy(lineBuf);
      }	
    }
  }
  free(lineBuf);
  return lines;
}
