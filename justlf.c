/*
 *  @(!--#) @(#) justlf.c, version 002, 05-march-2020
 *
 *  remove CR characters (\r) from a text file just leaving LF (\n)
 *  behind.  handy for handling files copied from a Windows system
 *  to a UNIX/Linux one.
 *
 */

/**********************************************************************/

/*
 *  includes
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/**********************************************************************/

/*
 *  defines
 */

#define MAX_FILE_CONTENT_SIZE 2000000
#define MAX_FILE_NAME_SIZE 80

/**********************************************************************/

/*
 *  globals
 */

char *progname;

int   debug = 0;

/**********************************************************************/

/*
 *  Main
 */

int main(argc, argv)
  int   argc;
  char *argv[];
{
  char *fname;
  FILE *f;
  char  tempfname[MAX_FILE_NAME_SIZE + 5];
  FILE *tempf;
  struct stat stbuf;
  int   lc;
  int   c;

  progname = argv[0];

  if (argc < 2) {
    fprintf(stderr, "%s: must specify a file name\n", progname);
    exit(1);
  }

  fname = argv[1];

  if (strlen(fname) > MAX_FILE_NAME_SIZE) {
    fprintf(stderr, "%s: file name \"%s\" is too long = limit is %d characters\n", progname, fname, MAX_FILE_NAME_SIZE);
    exit(1);
  }

  strncpy(tempfname, fname,  MAX_FILE_NAME_SIZE);
  strncat(tempfname, ".tmp", MAX_FILE_NAME_SIZE + 5);

  if (debug) {
    printf("[%s]\n", fname);
    printf("[%s]\n", tempfname);
  }

  if (stat(fname, &stbuf) != 0) {
    fprintf(stderr, "%s: cannot get status of file \"%s\"\n", progname, fname);
    exit(1);
  }

  if ((f = fopen(fname, "r")) == NULL) {
    fprintf(stderr, "%s: cannot open file \"%s\" for reading\n", progname, fname);
    exit(1);
  }

  unlink(tempfname);

  if ((tempf = fopen(tempfname, "w")) == NULL) {
    fprintf(stderr, "%s: cannot open temporary file \"%s\" for writing\n", progname, tempfname);
    exit(1);
  }

  if (chmod(tempfname, stbuf.st_mode) != 0) {
    fprintf(stderr, "%s: not able to set matching mode on temporary file \"%s\"\n", progname, tempfname);
    exit(1);
  }

  lc = EOF;

  while ((c = fgetc(f)) != EOF) {
    if (c != '\r') {
      fputc(c, tempf);
      lc = c;
    }
  }

  if (lc != '\n') {
    fputc('\n', tempf);
  }

  fflush(tempf);
  fclose(tempf);

  fclose(f);

  rename(tempfname, fname);

  return(0);
}

/**********************************************************************/

/* end of file */
