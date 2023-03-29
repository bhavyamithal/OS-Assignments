#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define MAX_LEN 100

void parse_input(char *input, char **args) {
    int i = 0;
    char *token = strtok(input, " \n");
    while (token != NULL) {
        args[i++] = token;
        token = strtok(NULL, " \n");
    }
    args[i] = NULL;
}

int execute(char **args) {
    pid_t pid = fork();
    if (pid == -1) {
        perror("Error in fork");
        return -1;
    }
    if (pid == 0) {
        int i = 0;
        int redirection = 0;
        char *input_file = NULL;
        char *output_file = NULL;
        int input_fd;
        int output_fd;
        while (args[i] != NULL) {
            if (strcmp(args[i], "<") == 0) {
                redirection = 1;
                input_file = args[i+1];
                args[i] = NULL;
                break;
            }
            if (strcmp(args[i], ">") == 0) {
                redirection = 2;
                output_file = args[i+1];
                args[i] = NULL;
                break;
            }
            i++;
        }
        if (redirection == 1) {
            input_fd = open(input_file, O_RDONLY);
            if (input_fd == -1) {
                perror("Error in opening input file");
                exit(1);
            }
            if (dup2(input_fd, STDIN_FILENO) == -1) {
                perror("Error in dup2 for input redirection");
                exit(1);
            }
            close(input_fd);
        }
        if (redirection == 2) {
            output_fd = open(output_file, O_CREAT|O_WRONLY, 0666);
            if (output_fd == -1) {
                perror("Error in opening output file");
                exit(1);
            }
            if (dup2(output_fd, STDOUT_FILENO) == -1) {
                perror("Error in dup2 for output redirection");
                exit(1);
            }
            close(output_fd);
        }
        if (execvp(args[0], args) == -1) {
            perror("Error in execvp");
            exit(1);
        }
    } else {
        int status;
        waitpid(pid, &status, 0);
        return status;
    }
    return 0;
}

int main() {
    char input[MAX_LEN];
    char *args[MAX_LEN];

    while (1) {
        printf("$ ");
        if (fgets(input, MAX_LEN, stdin) == NULL) {
            break;
        }
        parse_input(input, args);
        if (strcmp(args[0], "exit") == 0) {
            break;
        }
        if (strcmp(args[0], "cd") == 0) {
            if (chdir(args[1]) == -1) {
                perror("Error in chdir");
            }
            continue;
        }

        int background = 0;
        if (strcmp(args[0], "cd") != 0 && strcmp(args[0], "exit") != 0) {
            int i = 0;
            while (args[i] != NULL) {
                i++;
            }
            if (strcmp(args[i-1], "&") == 0) {
                background = 1;
                args[i-1] = NULL;
            }
        }

        if (background) {
            execute_background(args);
        } else {
            execute_command(args);
        }
    }
    return 0;
}

