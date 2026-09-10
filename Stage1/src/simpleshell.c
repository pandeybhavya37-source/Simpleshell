#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

extern char **environ;

int main(int argc, char *argv[]) {

    char line[MAX_LINE];
    char *args[MAX_ARGS];
    char cwd[PATH_MAX];
    char shell_path[PATH_MAX];
    FILE *input = stdin;
    int i;

    /* batch mode */
    if (argc == 2) {
        input = fopen(argv[1], "r");
        if (input == NULL) {
            perror("batch file");
            return 1;
        }
    }

    /* set shell environment variable */
    if (realpath("../bin/simpleshell", shell_path) != NULL) {
        if (setenv("shell", shell_path, 1) != 0) {
            perror("setenv");
        }
    }

    while (1) {

        /* display prompt only in interactive mode */
        /* display prompt only in interactive mode */
        if (input == stdin) {
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s> ", cwd);
            } else {
                perror("getcwd");
                printf("> ");
            }
        }
        /* read input */
        if (fgets(line, MAX_LINE, input) == NULL) {
            break;
        }

        /* remove newline */
        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0) {
            continue;
        }

        /* tokenize command */
        i = 0;
        args[i] = strtok(line, " \t");
        while (args[i] != NULL && i < MAX_ARGS - 1) {
            i++;
            args[i] = strtok(NULL, " \t");
        }

        if (args[0] == NULL) {
            continue;
        }

        /* quit */
        if (strcmp(args[0], "quit") == 0) {
            break;
        }

        /* clear screen */
        else if (strcmp(args[0], "clr") == 0) {
            system("clear");
        }

        /* echo */
        else if (strcmp(args[0], "echo") == 0) {
            int j = 1;
            while (args[j] != NULL) {
                printf("%s", args[j]);
                if (args[j + 1] != NULL) {
                    printf(" ");
                }
                j++;
            }
            printf("\n");
        }

        /* pause */
        else if (strcmp(args[0], "pause") == 0) {
            int ch;
            printf("Press Enter to continue...");
            fflush(stdout);
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
        }
        /* change directory */
        else if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    printf("%s\n", cwd);
                } else {
                    perror("cd");
                }
            } else {
                if (chdir(args[1]) != 0) {
                    perror("cd");
                } else {
                    if (getcwd(cwd, sizeof(cwd)) != NULL) {
                        setenv("PWD", cwd, 1);
                    }
                }
            }
        }

        /* directory listing */
        else if (strcmp(args[0], "dir") == 0) {
            char command[MAX_LINE];
            if (args[1] == NULL) {
                snprintf(command, sizeof(command), "ls -al");
            } else {
                snprintf(command, sizeof(command), "ls -al %s", args[1]);
            }
            system(command);
        }
        /* environment variables */
        else if (strcmp(args[0], "environ") == 0) {
            char **env = environ;
            while (*env != NULL) {
                printf("%s\n", *env);
                env++;
            }
        }

        /* help */
        else if (strcmp(args[0], "help") == 0) {
            system("more ../manual/readme.txt");
        }

        /* unknown command */
        else {
            printf("Unknown command: %s\n", args[0]);
        }
    }

    if (input != stdin) {
        fclose(input);
        }
}










