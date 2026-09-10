SimpleShell User Manual

Running the shell
-----------------
Interactive mode:
../bin/simpleshell

Batch mode:
../bin/simpleshell batchfile

Internal commands
-----------------
cd [directory]
    Change the current directory.
        If no directory is given, prints the current directory.

clr
   Clears the screen.

dir [directory]
   Lists the contents of a directory using ls -al.
   If no directory is given, lists the current directory.

environ
   Displays all environment variables.

echo [text]
   Displays the text entered.

help
   Displays this manual using more.

pause
   Pauses the shell until Enter is pressed.

quit
   Exits the shell.
