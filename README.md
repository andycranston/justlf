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

These are usually deleted after a successful run of the `justlf` command but
if the command was interrupts of subjected to an unusual error condition
then either or both of the temporary files might not get deleted.

The most common use of `justlf` is after a text file from a Windows platform
has been copied in binary mode to a UNIX/Linux system.  When the file
is edited using the `vi` editor every line ends with the:

```
^M
```

control character.  Simply exit `vi` and type:

```
justlf filename
```

where `filename` is the name of file.  Then edit again with `vi` and
all of the `^M` should have disappeared.

--------------------------------------

End of README.md
