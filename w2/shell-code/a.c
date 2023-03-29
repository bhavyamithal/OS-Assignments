#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define MAX_LEN 512

void parse_input(char *input, char **args) {
    int i = 0;
    args[i] = strtok(input, " \n");
    while (args[i] != NULL) {
        i++;
        args[i] = strtok(NULL, " \n");
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

        int pid = fork();
        if (pid == 0) {
            execvp(args[0], args);
        } else if (pid > 0) {
            wait(NULL);
        } else {
            perror("Error in fork");
        }
    }

    return 0;
}
