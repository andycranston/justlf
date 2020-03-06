# lineterm

Utilities to handle text file line terminator differences on different
platforms such as Windows and UNIX/Linux.

## justlf.c

Intended for the UNIX/Linux platform.  The `justlf` command
takes the name of a file as the first and only command line
argument.  This file is then stripped of any carriage return (\r)
characters.  Also if the last line in the file is not
a line feed (\n) character then one is added.

To compile do something similar to:

```
gcc -o $HOME/bin/justlf justlf.c
cd $HOME/bin
chmod u=rwx,go=rx justlf
```

The command creates temporary file names.  If the name of the
file being processed is `foo.txt` then the following temporary files
are created:

```
foo.txt.tmp1
foo.txt.tmp2
```



--------------------------------------

End of README.md
