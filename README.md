# SimpleShell – Operating Systems Project

## Overview

This project implements a simple UNIX-like shell in C. It supports both internal and external commands, along with background execution and input/output redirection. The aim of this project is to demonstrate how a basic shell works, including process creation and command execution.

--------------------------------------------------

## Features

### Internal Commands
- cd – change directory
- clr – clear the screen
- dir – list directory contents
- environ – display environment variables
- echo – print text
- help – display the user manual
- pause – wait for user input
- quit – exit the shell

--------------------------------------------------

### External Commands
Any command not recognised as internal is executed using fork() and execvp(). Examples include: ls, pwd, whoami, date.

--------------------------------------------------

### Background Execution
Commands can be run in the background using & (e.g., sleep 5 &). The shell immediately returns to the prompt without waiting.

--------------------------------------------------

### Input and Output Redirection
- > redirects output and overwrites file (e.g., ls > out.txt)
- >> appends output to a file (e.g., echo hello >> file.txt)
- < takes input from a file (e.g., sort < names.txt)
- Combined redirection is supported (e.g., sort < names.txt > sorted.txt)

--------------------------------------------------

## Project Structure

Stage2/
├── makefile
├── src/
│ ├── simpleshell.c
│ ├── utility.c
│ └── simpleshell.h
├── bin/
│ └── simpleshell
└── manual/
└── readme.txt

--------------------------------------------------

## Compilation

From the Stage2 directory, run:

make

This generates the executable in bin/simpleshell.

--------------------------------------------------

## Running the Shell

Interactive mode:
./bin/simpleshell

Batch mode:
./bin/simpleshell batchfile

--------------------------------------------------

## Notes

- Internal commands are executed within the shell
- External commands use fork() and execvp()
- Redirection is implemented using dup2()
- Background execution allows commands to run without blocking

--------------------------------------------------

## Author

Bhavya Pandey
Operating Systems Project – SimpleShell