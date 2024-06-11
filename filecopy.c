/* Short, Justin
 * CECS 326
 * Project 1: Interprocess Communication
 *
 * filecopy.c
 * This program copies files using a pipe.
 */

#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char *argv[]) {
    // Validate arguments
    if (argc != 3) {
        fprintf(stderr, "Error: Invalid arguments (input and output file names must be provided)\n");
        fprintf(stderr, "Example: ./filecopy input.txt output.txt\n");
        return -1;
    }

    // Create file handles for input and output
    FILE *input, *output;

    // Assign to handles based on argv
    input = fopen(argv[1], "r");
    output = fopen(argv[2], "w");
    if (input == NULL) {
        fprintf(stderr, "Error: Unable to open source file %s\n", argv[1]);
        return -1;
    }
    if (output == NULL) {
        printf("Error: Unalbe to open destnation file %s\n", argv[2]);
        return -1;
    }

    // Create file descriptor array and create pipe
    int fd[2];
    if (pipe(fd) == -1) {
        fprintf(stderr, "Error: Unable to open pipe\n");
        return -1;
    }

    // Fork process
    int id = fork();

    // In case fork returns error
    if (id == -1) {
        fprintf(stderr, "Error: Unable to fork process\n");
        close(fd[WRITE_END]);
        close(fd[READ_END]);
        return -1;
    }

    // In child process
    if (id == 0) {
        // Close read end and output file
        fclose(output);
        close(fd[READ_END]);

        // Write contents of input file to the pipe
        char c_in = fgetc(input);
        while (c_in != EOF) {
            if (write(fd[WRITE_END], &c_in, sizeof(char)) == -1) {
                fclose(input);
                close(fd[WRITE_END]);
                fprintf(stderr, "Error: Unable to write to pipe");
                return -1;
            }
            c_in = fgetc(input);
        }

        // Write EOF to pipe
        if (write(fd[WRITE_END], &c_in, sizeof(char)) == -1) {
            fclose(input);
            close(fd[WRITE_END]);
            fprintf(stderr, "Error: Unable to write to pipe");
            return -1;
        }

        // Close write end and input file
        close(fd[WRITE_END]);
        fclose(input);  //

    } else {  // In parent process
        // Close write end and input file
        close(fd[WRITE_END]);
        fclose(input);

        // Wait on child process to finish
        wait(0);

        // Read from pipe and write to output file
        char c_out;
        if (read(fd[READ_END], &c_out, sizeof(char)) == -1) {
            fprintf(stderr, "Error: Unable to read from pipe");
            fclose(output);
            close(fd[READ_END]);
            return -1;
        }
        while (c_out != EOF) {
            fprintf(output, "%c", c_out);
            if (read(fd[READ_END], &c_out, sizeof(char)) == -1) {
                fprintf(stderr, "Error: Unable to read from pipe");
                fclose(output);
                close(fd[READ_END]);
                return -1;
            }
        }

        // Close read end and output file
        close(fd[READ_END]);
        fclose(output);
    }

    return 0;
}
