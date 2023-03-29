#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_LEN 512

void parse_input(char *input, char **args) {
    int i = 0;
    args[i] = strtok(input, " \n");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " \n");
    }
}

void execute_background(char **args) {
    pid_t pid;
    pid = fork();
    if (pid == -1) {
        perror("Error in fork");
        exit(1);
    } else if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("Error in execvp");
            exit(1);
        }
    }
}

void execute_foreground(char **args) {
    pid_t pid;
    pid = fork();
    if (pid == -1) {
        perror("Error in fork");
        exit(1);
    } else if (pid == 0) {
        if (execvp(args[0], args) == -1) {
            perror("Error in execvp");
            exit(1);
        }
    } else {
        wait(NULL);
    }
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
            execute_foreground(args);
        }
    }

    return 0;
}
