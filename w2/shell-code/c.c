#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAX_LEN 100

// Function to parse the input from the user
void parse_input(char *input, char **args) {
    int i = 0;
    args[i] = strtok(input, " \n");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " \n");
    }
}

// Function to execute the commands in background
void execute_background(char **args) {
    int pid = fork();
    if (pid == 0) {
        execvp(args[0], args);
        perror("Error in execvp");
        exit(1);
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

        // Check if the input command is to run in the background
        int background = 0;
        int i = 0;
        while (args[i] != NULL) {
            i++;
        }
        if (strcmp(args[i-1], "&") == 0) {
            background = 1;
            args[i-1] = NULL;
        }

        // If the input command is to run in the background, call the execute_background function
        if (background) {
            execute_background(args);
        } else {
            int pid = fork();
            if (pid == 0) {
                execvp(args[0], args);
                perror("Error in execvp");
                exit(1);
            } else {
                waitpid(pid, NULL, 0);
            }
        }
    }
    return 0;
}
