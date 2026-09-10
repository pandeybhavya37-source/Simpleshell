/*
Bhavya Pandey, 11317
CSC1021 Operating Systems Project
SimpleShell - Stage 2
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

/* CONSTANT DEFINITIONS  */

#define MAX_LINE 1024
#define MAX_ARGS 64

/*  GLOBAL VARIABLES*/

extern char **environ;

/* MAIN FUNCTIOION */

int main(int argc, char *argv[]) {

    /* Variable declaration */

    char line[MAX_LINE];
    char *args[MAX_ARGS];
    char cwd[PATH_MAX];
    char shell_path[PATH_MAX];

    FILE *input = stdin;
    int i;
    int background;
    int out_redirect = 0;
    int append_redirect = 0;
    int in_redirect = 0;

    char *outfile = NULL;
    char *infile = NULL;


    /*  BATCH MODE */

    if (argc == 2) {
        input = fopen(argv[1], "r");

        if (input == NULL) {
            perror("batch file");
            return 1;
        }
    }

    /*   SET SHELL ENVIRONMENT VARIABLE      */

    if (realpath("./bin/simpleshell", shell_path) == NULL) {
        if (realpath("bin/simpleshell", shell_path) == NULL) {
            realpath("../Stage2/bin/simpleshell", shell_path);
        }
    }

    if (shell_path[0] != '\0') {
        if (setenv("shell", shell_path, 1) != 0) {
            perror("setenv");
        }
    }

    /*               MAIN SHELL LOOP        */

    while (1) {
        background = 0;
        out_redirect = 0;
        append_redirect = 0;
        in_redirect = 0;

        /* Display prompt  */

        if (input == stdin) {
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s> ", cwd);
            }
            else {
                perror("getcwd");
                printf("> ");
            }
        }

        /* Read command line */

        if (fgets(line, MAX_LINE, input) == NULL) {
            break;
        }

        /* Remove newline character */

        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0) {
            continue;
        }

        /*             TOKENIZE INPUT             */

        i = 0;
        background = 0;

        args[i] = strtok(line, " \t");

        while (args[i] != NULL && i < MAX_ARGS - 1) {
            i++;
            args[i] = strtok(NULL, " \t");
        }

        if (args[0] == NULL) {
            continue;
        }
        /* check for background execution symbol & */
        for (i = 0; args[i] != NULL; i++) {
            if (strcmp(args[i], "&") == 0 && args[i + 1] == NULL) {
               background = 1;
               args[i] = NULL;
               break;
            }
        }

        /*check for redirection symbols*/
        for (i = 0; args[i] != NULL; i++) {

            /* output overwrite > */
            if (strcmp(args[i], ">") == 0) {
                out_redirect = 1;
                outfile = args[i+1];
                args[i] = NULL;
                break;
            }

            /* output append >> */
            else if (strcmp(args[i], ">>") == 0) {
                append_redirect = 1;
                outfile = args[i+1];
                args[i] = NULL;
                break;
            }

            /* input redirection < */
            else if (strcmp(args[i], "<") == 0) {
                in_redirect = 1;
                infile = args[i+1];
                args[i] = NULL;
                break;
            }
        }

        /*         INTERNAL COMMANDS              */

        /*  quit  */

        if (strcmp(args[0], "quit") == 0) {
            break;
        }

        /* clear screen */

        else if (strcmp(args[0], "clr") == 0) {
            system("clear");
        }

        /* echo text */

        else if (strcmp(args[0], "echo") == 0) {
            int saved_stdout = dup(STDOUT_FILENO);

            if (saved_stdout < 0) {
                perror("dup");
            }

            if (out_redirect) {
                int fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if (fd < 0) {
                    perror("open");
                } else {
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
            }
            else if (append_redirect) {
                int fd = open(outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
                if (fd < 0) {
                    perror("open");
                } else {
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                    }
                }
            int j = 1;
            while (args[j] != NULL) {
                printf("%s", args[j]);
                if (args[j + 1] != NULL) {
                    printf(" ");
                }
                j++;
            }

            printf("\n");
            fflush(stdout);

            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdout);
        }
        /* pause shell */

        else if (strcmp(args[0], "pause") == 0) {

            int ch;

            printf("Press Enter to continue...");
            fflush(stdout);

            while ((ch = getchar()) != '\n' && ch != EOF) {
            }
        }

        /* change directory*/
        else if (strcmp(args[0], "cd") == 0) {

            if (args[1] == NULL) {

                if (getcwd(cwd, sizeof(cwd)) != NULL) {
                    printf("%s\n", cwd);
                }
                else {
                    perror("cd");
                }
            }

            else {

                if (chdir(args[1]) != 0) {
                    perror("cd");
                }

                else {

                    if (getcwd(cwd, sizeof(cwd)) != NULL) {

                        if (setenv("PWD", cwd, 1) != 0) {
                            perror("setenv");
                        }
                    }
                }
            }
        }

        /*directory listing */

        else if (strcmp(args[0], "dir") == 0) {
            int saved_stdout = dup(STDOUT_FILENO);

            if (saved_stdout < 0) {
                perror("dup");
            }

            if (out_redirect) {
                int fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if (fd < 0) {
                    perror("open");
                } else {
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
            }

            else if (append_redirect) {
                int fd = open(outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
                if (fd < 0) {
                    perror("open");
                } else {
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
            }

            char command[MAX_LINE];

            if (args[1] == NULL) {
                snprintf(command, sizeof(command), "ls -al");
                } else {
                    snprintf(command, sizeof(command), "ls -al %s", args[1]);
                }

                system(command);
                fflush(stdout);

                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdout);
            }
        /*  display environment variables  */

        else if (strcmp(args[0], "environ") == 0) {
            int saved_stdout = dup(STDOUT_FILENO);

            if (saved_stdout < 0) {
                perror("dup");
            }

            if (out_redirect) {
                int fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if (fd < 0) {
                    perror("open");
                } else {
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
            }

            else if (append_redirect) {
                int fd = open(outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
                if (fd < 0) {
                    perror("open");
                } else {
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
            }
            char **env = environ;

            while (*env != NULL) {
                printf("%s\n", *env);
                env++;
            }

            fflush(stdout);

            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdout);
        }

        /* -------- help command -------- */
        else if (strcmp(args[0], "help") == 0) {
            int saved_stdout = dup(STDOUT_FILENO);

            if (saved_stdout < 0) {
                perror("dup");
            }

            if (out_redirect) {
                int fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if (fd < 0) {
                    perror("open");
                } else {
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
            }

            else if (append_redirect) {
                int fd = open(outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
                if (fd < 0) {
                    perror("open");
                } else {
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
            }

            system("more manual/readme.txt");
            fflush(stdout);

            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdout);
        }
   
        /*    EXTERNAL COMMAND EXECUTION     */

        else {

            pid_t pid = fork();

            if (pid == 0) {

                /* CHILD PROCESS */

                if (setenv("parent", shell_path, 1) != 0) {
                    perror("setenv");
                }
                /* INPUT REDIRECTION  */
                if (in_redirect) {
                    int fd = open(infile, O_RDONLY);
                    if (fd < 0) {
                        perror("open");
                        exit(1);
                    }

                    dup2(fd, STDIN_FILENO);
                    close(fd);
                }

                /* OUTPUT REDIRECTION (overwrite) */
                if (out_redirect) {
                    int fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
                    if (fd < 0) {
                        perror("open");
                        exit(1);
                    }

                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }

                /* OUTPUT REDIRECTION (append) */
                if (append_redirect) {
                    int fd = open(outfile, O_CREAT | O_WRONLY | O_APPEND, 0644);
                    if (fd < 0) {
                        perror("open");
                        exit(1);
                    }

                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
                execvp(args[0], args);

                perror("execvp");
                exit(1);
            }

            else if (pid > 0) {

                /*  PARENT PROCESS  */

                if (background == 0) {
                    wait(NULL);
                }
            }

            else {
                perror("fork");
            }
        }
    }
                    /*  CLEANUP   */


    if (input != stdin) {
        fclose(input);
    }

    return 0;
}
