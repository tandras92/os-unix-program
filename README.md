# Simple Shell Program
A simple shell implemented in C.

## Overview
A simple interactive shell program that prompts the user for a command, parse the command, and then executes it with a child process.

## Prerequisites
Before you continue, make sure you have me the following requirements:
  * An IDE such as Visual Studio Code (If you're on Windows)
  * Command Prompt/Terminal
  * UNIX OS
  * gcc compiler
  
## Program Contents
  * `Makefile` is a file containing shell commands.  When the user types `make` in the terminal, all the commands in the makefile will be executed.
  * `minishell.h` is a header file that identifies dependencies
  * `shell.c` is the source code file
  
## How to Install
Run `git clone https://github.com/tandras92/os-unix-program.git` or download the application.

## Built-in Commands
* cd `<directoryname>`
* ls
* cat `<filename>`
* exit
* help

## Make Commands
In the terminal, cd to the directory that contains the program and run the below commands.

| Command | Description |
|-------- | ----------- |
| make    | compiles the program |
| make run| runs the program |
| make clean | removes the `shell` file |

## References
[Operating Systems by Gary Nutt](https://www.amazon.com/Operating-Systems-3rd-Gary-Nutt/dp/0201773449)

[Makefile Tutorial](https://makefiletutorial.com/)


