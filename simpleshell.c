#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#define MAX_LINE 1024
#define MAX_ARGS 64

int main(int argc, char *argv[]) {
    char line[MAX_LINE];
    char *args[MAX_ARGS];
    char cwd[1024];
    int i;

    while (1) {
        /* show current directory as prompt */
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s> ", cwd);
        } else {
            perror("getcwd");
            printf("> ");
        }

        /* read input */
        if (fgets(line, MAX_LINE, stdin) == NULL) {
            break;
        }

        /* remove newline */
        line[strcspn(line, "\n")] = '\0';

        /* skip empty input */
        if (strlen(line) == 0) {
            continue;
        }

        /* tokenize input */
        i = 0;
        args[i] = strtok(line, " \t");
        while (args[i] != NULL && i < MAX_ARGS - 1) {
            i++;
            args[i] = strtok(NULL, " \t");
        }

        if (args[0] == NULL) {
            continue;
        }
        /* internal commands */
        if (strcmp(args[0], "quit") == 0) {
            break;
        } else if (strcmp(args[0], "clr") == 0) {
            system("clear");
        } else if (strcmp(args[0], "echo") == 0) {
            int j = 1;
            while (args[j] != NULL) {
                printf("%s", args[j]);
                if (args[j + 1] != NULL) {
                    printf(" ");
                }
                j++;
            }
            printf("\n");
        } else if (strcmp(args[0], "pause") == 0) {
            int ch;
            printf("Press Enter to continue...");
            fflush(stdout);
            while ((ch = getchar()) != '\n' && ch != EOF) {
                ;
            }
        } else {
              pid_t pid = fork();

              if (pid == 0) {
                  /* child process */

                  execvp(args[0], args);

                  perror("execvp");
                  exit(1);
              }
              else if (pid > 0) {
                  /* parent process */

                  wait(NULL);
              }
              else {
                   perror("fork");
              }
          }
    }

    return 0;
}
