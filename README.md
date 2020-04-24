# lineutils

Utilities to handles lines in plain text files.

## justlf.c

Intended for the UNIX/Linux platform. The `justlf` command takes the
name of a file. The file
is examined to see if it is a plain text file. If it is not then an
error message is printed and the program exits.

Assuming the file is a plain text file it is stripped of any carriage
return (\r) characters. Also if the last line in the file does not end
with a line feed (\n) character then one is added.

To compile do something similar to:

```
gcc -o $HOME/bin/justlf justlf.c
cd $HOME/bin
chmod u=rwx,go=rx justlf
```

The command creates temporary file names.  If the name of the file being
processed is `foo.txt` then the following temporary files are created:

```
foo.txt.tmp1.XXXXXX
foo.txt.tmp2.XXXXXX
```

where `XXXXXX` is replaced by a uniquely random pattern. The `mkstemp`
function is used to do this.
These temporary file are usually deleted after a successful run of the `justlf`
command but if the command was interrupted or subjected to an unusual
error condition then either or both of the temporary files might not
get deleted.

The most common use of `justlf` is after a text file from a Windows
platform has been copied in binary mode to a UNIX/Linux system.  When the
file is edited using the `vi` editor every line ends with the:

```
^M
```

control character.  Simply exit `vi` and type:

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
