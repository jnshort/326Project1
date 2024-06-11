Short, Justin  
CECS 326  
Due 06/10/24  

# Project 1: Interprocess Communication
The program is designed to take 2 arguments an input file and an output file. Handles for these files are created before a pipe is made and before the program is forked. This allows both the parent and child processes to have access to both files. When the program is then forked, the process id can then be used in controll flow to perform separate actions depending on what process is being executed.  
In the child process, characters are read in from the input file, then written to the "WRITE_END" of the pipe that was created. The parent process will wait for the child process to complete, then read from the "READ_END" of the pipe. The values that are read can then be written to the output file one character at a time.  
Errors can occur when opening a file, writing to a pipe, or reading from a pipe. In the even that an error is encounted. The program will respond by closing all open file descriptors and files, then exiting with an error status.  


