Shell Implementation (Parts A, B, C, and D)

Introduction

In this project, we will implement a shell that can execute simple Linux commands. 
The shell supports the following functionalities:

Part A: Simple commands execution in foreground and background.
Part B: Built-in commands cd and exit.
Part C: Support for foreground and background execution of commands.
Part D: The shell can handle Ctrl+C signal gracefully.


Part A: Simple commands execution in foreground and background
In this part, we will implement the functionality of executing simple Linux commands 
in the foreground and background. The shell prompts the user for a command, parses the 
input and executes the command. If the command is not a built-in command, the shell 
forks a child process to execute the command. The shell then waits for the child to 
finish if the command is executed in the foreground, or continues to prompt for 
commands if the command is executed in the background.

The user can specify if a command should run in the background by appending 
an ampersand (&) to the command. If the ampersand is present, the shell executes 
the command in the background and does not wait for the child to finish.


Part B: Built-in commands cd and exit
In this part, we will implement two built-in commands, cd and exit. The cd 
command changes the current working directory of the shell. The exit command 
terminates the shell.


Part C: Support for foreground and background execution of commands
In this part, we will extend the shell to support the execution of multiple 
commands in the foreground and background. Multiple user commands separated 
by semicolons (;) should be executed one after the other in the foreground or 
background. The shell should move on to the next command in the sequence only 
after the previous one has completed (successfully or with errors) and the 
corresponding terminated child reaped by the parent. The shell should return to 
the command prompt after all the commands in the sequence have finished execution.

The user can specify if a sequence of commands should run in the background by 
appending an ampersand (&) to the last command in the sequence. If the ampersand 
is present, the shell executes all commands in the background and does not wait 
for any of the children to finish.


Part D: Handling Ctrl+C signal gracefully
In this part, we will handle the Ctrl+C signal gracefully. The shell must terminate 
all foreground processes running in the foreground and return to the command prompt.


Conclusion
This project provides a basic implementation of a shell that supports simple 
Linux commands execution in the foreground and background, built-in commands cd 
and exit, and handling of the Ctrl+C signal. The code can be extended further to 
add more functionalities to the shell.