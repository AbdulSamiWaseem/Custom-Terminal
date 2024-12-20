Custom Terminal
Overview
This is a custom-built terminal application created in C++ using fundamental operating system concepts like process management, I/O redirection, and history management. It supports common shell functionalities, including command execution, handling input/output redirection, maintaining a history of commands, and a basic implementation of job control using process forking.

Key Features
Command Execution: Execute shell commands directly using exec.
Command History:
View the last 10 commands entered.
Re-execute commands using !! (last command) or !n (nth previous command).
Directory Navigation: Use the cd command to change directories, mimicking standard shell behavior.
Input/Output Redirection: Supports input (<) and output (>) redirection for file handling.
Process Control:
Implements forking to handle multiple processes.
Waits for child processes to complete unless executed with the & flag (background processes).
Custom Tokenization:
Tokenizes user input to handle special symbols like |, <, >, and &.
Handles complex command structures gracefully.
How to Run
Clone the repository and navigate to the project directory:
bash
Copy code
git clone <repository-url>
cd custom-terminal
Compile the program using a C++ compiler:
bash
Copy code
g++ -o terminal terminal.cpp
Run the terminal:
bash
Copy code
./terminal
Usage
Enter commands like in a regular shell.
Use history to view the last 10 commands.
Execute the last command with !! or any previous command using !n (replace n with the command's index in history).
Redirect input/output with < and >:
bash
Copy code
cat < input.txt > output.txt
Use cd to change directories:
bash
Copy code
cd /path/to/directory
To run a command in the background, append &:
bash
Copy code
long_running_command &
Dependencies
Linux/Unix-based operating system.
GCC or another C++11-compatible compiler.
