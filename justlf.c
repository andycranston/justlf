/*
 *  @(!--#) @(#) justlf.c, version 004, 11-march-2020
 *
 *  remove CR characters (\r) from a text file just leaving LF (\n)
 *  behind.  handy for handling files copied from a Windows system
 *  to a UNIX/Linux one.
 *
 *  also make sure last line has a LF (\n) at the end of it.
 *
 *  uses two temporary files to mitigate against dataloss should
 *  the rename calls fail for some reason (e.g. disk full).
 *
 *  add a check to make sure file is a text file before messing
 *  with it.
 *
 */

/**********************************************************************/

/*
 *  includes
 */

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/**********************************************************************/

/*
 *  defines
 */

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define MAX_FILE_NAME_SIZE 80

/**********************************************************************/

/*
 *  globals
 */

char *progname;

int   debug = 0;

/**********************************************************************/

int istextfile(fname)
  char *fname;
{
  FILE *f;
  int   c;
  int   flag;

  if ((f = fopen(fname, "r")) == NULL) {
    return FALSE;
  }

  flag = TRUE;

  while ((c = getc(f)) != EOF) {
    if (isprint(c)) {
      continue;
    }

    if (isspace(c)) {
      continue;
    }

    flag = FALSE;
    break;
  }

  fclose(f);

  return flag;
}

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
  char  tempfname1[MAX_FILE_NAME_SIZE + 6];
  char  tempfname2[MAX_FILE_NAME_SIZE + 6];
  FILE *tempf1;
  struct stat stbuf;
  int   lc;
  int   c;

  progname = argv[0];

  if (argc < 2) {
    fprintf(stderr, "%s: must specify a file name\n", progname);
    exit(1);
  }

  if (argc > 2) {
    fprintf(stderr, "%s: only allowed to specify one file name\n", progname);
    exit(1);
  }

  fname = argv[1];

  if (strlen(fname) > MAX_FILE_NAME_SIZE) {
    fprintf(stderr, "%s: file name \"%s\" is too long = limit is %d characters\n", progname, fname, MAX_FILE_NAME_SIZE);
    exit(1);
  }

  if (stat(fname, &stbuf) != 0) {
    fprintf(stderr, "%s: cannot get status of file \"%s\"\n", progname, fname);
    exit(1);
  }

  if (! istextfile(fname)) {
    fprintf(stderr, "%s: file \"%s\" does not appear to be a plain text file\n", progname, fname);
    exit(1);
  }

  strncpy(tempfname1, fname,   MAX_FILE_NAME_SIZE);
  strncat(tempfname1, ".tmp1", MAX_FILE_NAME_SIZE + 6);

  strncpy(tempfname2, fname,   MAX_FILE_NAME_SIZE);
  strncat(tempfname2, ".tmp2", MAX_FILE_NAME_SIZE + 6);

  if (debug) {
    printf("[%s]\n", fname);
    printf("[%s]\n", tempfname1);
    printf("[%s]\n", tempfname2);
  }

  if ((f = fopen(fname, "r")) == NULL) {
    fprintf(stderr, "%s: cannot open file \"%s\" for reading\n", progname, fname);
    exit(1);
  }

  unlink(tempfname1);

  if ((tempf1 = fopen(tempfname1, "w")) == NULL) {
    fprintf(stderr, "%s: cannot open temporary file \"%s\" for writing\n", progname, tempfname1);
    exit(1);
  }

  if (chmod(tempfname1, stbuf.st_mode) != 0) {
    fprintf(stderr, "%s: not able to set matching mode on temporary file \"%s\"\n", progname, tempfname1);
    exit(1);
  }

  lc = EOF;

  while ((c = fgetc(f)) != EOF) {
    if (c != '\r') {
      fputc(c, tempf1);
      lc = c;
    }
  }

  if (lc != '\n') {
    fputc('\n', tempf1);
  }

  fflush(tempf1);
  fclose(tempf1);

  fclose(f);

  if (rename(fname, tempfname2) != 0) {
    fprintf(stderr, "%s: unable to rename file \"%s\" to temporary location \"%s\"\n", progname, fname, tempfname2);
    exit(1);
  }

  if (rename(tempfname1, fname) != 0) {
    fprintf(stderr, "%s: unable to rename temporary file \"%s\" to \"%s\"\n", progname, tempfname1, fname);
    exit(1);
  }

  if (chmod(fname, stbuf.st_mode) != 0) {
    fprintf(stderr, "%s: not able to set mode on file \"%s\"\n", progname, fname);
    exit(1);
  }

  unlink(tempfname2);

  return(0);
}

/**********************************************************************/

/* end of file */
