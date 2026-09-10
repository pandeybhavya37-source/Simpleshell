


Running the shell

--------------------------------------------------

Interactive mode:
../bin/simpleshell

Batch mode:
../bin/simpleshell batchfile

--------------------------------------------------

Internal commands


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

--------------------------------------------------

External commands


If a command is not recognised as an internal command, the shell will attempt to run it as an external command using fork() and execvp().

Examples:
ls
pwd
whoami
date

-------

Background execution

--------------------------------------------------

Commands can be executed in the background using the & symbol.

Example:
sleep 5 &

The shell will immediately return to the prompt without waiting for the command to finish.

--------------------------------------------------

Output redirection (>)


Redirects the output of a command to a file.
If the file already exists, it is overwritten.

Example:
ls > out.txt

The output will be written to out.txt instead of the screen.

--------------------------------------------------

Append redirection (>>)


Appends the output of a command to a file instead of overwriting it.

Example:
echo hello >> file.txt

This will add "hello" to the end of file.txt.


-----------------------------------------------
Input redirection (<)


Takes input from a file instead of the keyboard.

Example:
sort < names.txt

The command will read input from names.txt.

--------------------------------------------------

Combined redirection


Input and output redirection can be used together.

Example:
sort < names.txt > sorted.txt

This reads from names.txt and writes the sorted output to sorted.txt.

--------------------------------------------------

Environment variables


The shell uses environment variables such as:

shell
Contains the path of the shell executable.

parent
Set when executing external commands to indicate the parent shell.

These can be viewed using the environ command.

