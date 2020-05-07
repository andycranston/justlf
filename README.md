# justlf

A command to strip carriage return characters from a plain ASCII text file

## Overview

The `justlf` command strips carriage return charaters from a plain
ASCII text file (named on the command line) in a similar fashion to the
`dos2unix` command.

However, there are differences between the `justlf` command and the
`dos2unix` command:

+ If the file is already stripped of carriage return characters then
the file is not modified. The `dos2unix` command always rewrites the
file even if it does not have to.

+ If the last line in the file does not end in a linefeed/newline
character (\n) then the `justlf` command will add one. This prevents a
file having an "Incomplete last line" (a message the vi editor will report
when editing such a file). The `dos2unix` command leaves incomplete last
lines as they are.

+ The `justlf` command has a strict (some might say too strict)
interpretation of what a plain ASCII text file looks like. It is the
ASCII charcater codes between 32 and 126 inclusive and codes 10 (\n),
13 (\r) and 8 (\t).  The `dos2unix` command

+ The `justlf` command chevks that the file is writable by the
user running the `jutlf` command.

+ The `justlf` command checks the file is owned by the same user running
the `justlf` command.

The `justlf` command is intended for the UNIX/Linux platform and it
takes the name of a file on the command line. The file is examined to
see if it is a plain ASCII text file. If it is not then an error message is
printed and the program exits.

Assuming the file is a plain ASCII text file then it is stripped of any carriage
return (\r) characters. Also if the last line in the file does not end
with a linefeed (\n) character then one is added.

To compile do something similar to:

```
gcc -o $HOME/bin/justlf justlf.c
cd $HOME/bin
chmod u=rwx,go=rx justlf
```

The command creates temporary file names. If the name of the file being
processed is `foo.txt` then the following temporary files are created:

```
foo.txt.tmp1.XXXXXX
foo.txt.tmp2.XXXXXX
```

where `XXXXXX` is replaced by a uniquely random pattern. The `mkstemp`
function is used to do this. These temporary files are usually deleted
after a successful run of the `justlf` command but if the command was
interrupted or subjected to an unusual error condition then either or
both of the temporary files might not get deleted.

The most common use of `justlf` is after a text file from a Windows
platform has been copied in binary mode to a UNIX/Linux system. When
the file is edited using the `vi` editor every line ends with the:

```
^M
```

control character. Simply exit `vi` and type:

```
justlf filename
```

where `filename` is the name of file.

Then edit again with `vi` and all of the `^M` should have disappeared.

The `justlf` command displays information on standard output. For example:

```
justlf: filename="file"
justlf: first temp file="file.tmp1.e1L3wX"
justlf: second temp file="file.tmp2.5rIKAS"
justlf: changes made to the file - renaming temporary files
justlf: deleting both temporary files
```

To have the command run without displaying this output use the `-q`
command line option as follows:

```
justlf -q file
```

Note that error messages are always sent to standard error regardless
of whether the `-q` command line option is specified or not.

--------------------------------------

End of README.md
