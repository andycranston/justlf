/*
 *  @(!--#) @(#) justlf.c, version 010, 07-may-2020
 *
 *  remove CR characters (\r) from a text file just leaving LF (\n)
 *  behind. handy for handling files copied from a Windows system
 *  to a UNIX/Linux one.
 *
 *  also make sure last line has a LF (\n) at the end of it.
 *
 *  uses two temporary files to mitigate against dataloss should
 *  the rename calls fail for some reason (e.g. disk full).
 *
 *  add a check to make sure file is a text file before
 *  changing it.
 *
 */

/**********************************************************************/

/*
 *  includes
 */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

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

#define MAX_FILENAME_LENGTH   256
#define TEMP_FILENAME_PADDING 12

/**********************************************************************/

/*
 *  globals
 */

char *progname;

/**********************************************************************/

char *basefilename(filename)
  char   *filename;
{
  char   *s;
  char   *bfile;

  bfile = filename;

  s = filename;

  while (*s != '\0') {
    if ((*s == '\\') || (*s == '/')) {
      bfile = s + 1;
    }

    s++;
  }

  return bfile;
}

/**********************************************************************/

void usage()
{
  fprintf(stderr, "%s: %s [ -q ] filename\n", progname, progname);
  exit(2);
}

/**********************************************************************/ 

int isplainasciitextfile(fname)
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
    if ((c >= 32) && (c <= 126)) {
      continue;
    }

    if ((c == '\n') || (c == '\r') || (c == '\t')) {
      continue;
    }

    flag = FALSE;
    break;
  }

  fclose(f);

  return flag;
}

/**********************************************************************/

int justlf(filename, quiet)
  char *filename;
  int   quiet;
{
  struct stat stbuf;
/*
  struct utim utimbuf;
*/
  char        tempfname1[MAX_FILENAME_LENGTH + TEMP_FILENAME_PADDING + sizeof(char)];
  char        tempfname2[MAX_FILENAME_LENGTH + TEMP_FILENAME_PADDING + sizeof(char)];
  int         tempf1;
  int         tempf2;
  FILE       *f;
  FILE       *f1;
  long int    filesize;
  int         changed;
  int         lc;
  int         c;

  if (! quiet) {
    printf("%s: filename=\"%s\"\n", progname, filename);
  }

  if (strlen(filename) > MAX_FILENAME_LENGTH) {
    fprintf(stderr, "%s: file name \"%s\" is too long, limit is %d characters\n", progname, filename, MAX_FILENAME_LENGTH);
    return(1);
  }

  if (access(filename, R_OK) != 0) {
    fprintf(stderr, "%s: file \"%s\" is not readable\n", progname, filename);
    return(1);
  }

  if (access(filename, W_OK) != 0) {
    fprintf(stderr, "%s: file \"%s\" is not writable\n", progname, filename);
    return(1);
  }

  if (stat(filename, &stbuf) != 0) {
    fprintf(stderr, "%s: cannot get status of file \"%s\"\n", progname, filename);
    return(1);
  }

  if (stbuf.st_uid != getuid()) {
    fprintf(stderr, "%s: file \"%s\" is not owned by the user running this command\n", progname, filename);
    return(1);
  }

  if ((f = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "%s: cannot open file \"%s\" for reading\n", progname, filename);
    return 1;
  }

  if (! isplainasciitextfile(filename)) {
    fprintf(stderr, "%s: file \"%s\" is not a plain ASCII text file\n", progname, filename);
    exit(1);
  }

  strncpy(tempfname1, filename, MAX_FILENAME_LENGTH);
  strncat(tempfname1, ".tmp1.XXXXXX", MAX_FILENAME_LENGTH + TEMP_FILENAME_PADDING);

  if ((tempf1 = mkstemp(tempfname1)) == -1) {
    fprintf(stderr, "%s: cannot create first temporary file \"%s\"\n", progname, tempfname1);
    return 1;
  }

  if (! quiet) {
    printf("%s: first temp file=\"%s\"\n", progname, tempfname1);
  }

  if ((f1 = fdopen(tempf1, "w")) == NULL) {
    fprintf(stderr, "%s: cannot create writable file stream for first temporary file \"%s\"\n", progname, tempfname1);
    return 1;
  }

  strncpy(tempfname2, filename, MAX_FILENAME_LENGTH);
  strncat(tempfname2, ".tmp2.XXXXXX", MAX_FILENAME_LENGTH + TEMP_FILENAME_PADDING);

  if ((tempf2 = mkstemp(tempfname2)) == -1) {
    fprintf(stderr, "%s: cannot create second temporary file \"%s\"\n", progname, tempfname2);
    return 1;
  }

  if (! quiet) {
    printf("%s: second temp file=\"%s\"\n", progname, tempfname2);
  }

  if (chmod(tempfname1, stbuf.st_mode) != 0) {
    fprintf(stderr, "%s: not able to set matching mode on temporary file \"%s\"\n", progname, tempfname1);
    return 1;
  }

  filesize = 0;
  changed  = FALSE;
  lc       = EOF;

  while ((c = fgetc(f)) != EOF) {
    if (c != '\r') {
      fputc(c, f1);
      lc = c;
      filesize++;
    } else {
      changed = TRUE;
    }
  }

  if (filesize > 0) {
    if (lc != '\n') {
      if (! quiet) {
        printf("%s: adding linefeed character \\n to end of file \"%s\"\n", progname, filename);
      }
      fputc('\n', f1);
      changed = TRUE;
    }
  }

  fflush(f1);
  fclose(f1);

  fclose(f);

  if (changed) {
    if (! quiet) {
      printf("%s: changes made to the file - renaming temporary files\n", progname);
    }
    if (rename(filename, tempfname2) != 0) {
      fprintf(stderr, "%s: unable to rename file \"%s\" to temporary location \"%s\"\n", progname, filename, tempfname2);
      return 1;
    }

    if (rename(tempfname1, filename) != 0) {
      fprintf(stderr, "%s: unable to rename temporary file \"%s\" to \"%s\"\n", progname, tempfname1, filename);
      return 1;
    }

    if (chmod(filename, stbuf.st_mode) != 0) {
      fprintf(stderr, "%s: not able to set mode/permissions on file \"%s\"\n", progname, filename);
      return 1;
    }
/*

    utim.actime  = stbuf.st_atime;
    utim.modtime = stbuf.st_mtime;

    if (utime(filename, &utim) != 0) {
      fprintf(stderr, "%s: not able to set access and modification time on file \"%s\"\n", progname, filename);
      return 1;
    }
  } else {
    if (! quiet) {
      printf("%s: no changes required\n", progname);
    }
*/
  }

  if (! quiet) {
    printf("%s: deleting both temporary files\n", progname);
  }
  unlink(tempfname1);
  unlink(tempfname2);

  return 0;
}


/**********************************************************************/

/*
 *  Main
 */

int main(argc, argv)
  int   argc;
  char *argv[];
{
  int   quiet = FALSE;
  char *filename;

  progname = basefilename(argv[0]);

  if (argc > 3) {
    usage();
  }

  if (argc == 3) {
    if (strcmp(argv[1], "-q") == 0) {
      quiet = TRUE;
      argc--;
      argv++;
    } else {
      usage();
    }
  }

  if (argc != 2) {
    usage();
  }

  filename = argv[1];

  return justlf(filename, quiet);
}

/**********************************************************************/

/* end of file */
