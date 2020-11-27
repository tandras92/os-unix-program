#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include "minishell.h"


static char *user_input;         /* input entered by the user */

int main (int argc, char *argv[]) {
 /* read command line parameters */
        if (argc != 2) {
            fprintf(stderr, "Usage: launch <launch_set_filename>\n");
            return EXIT_SUCCESS;
        }

        return shell_commands(argv[1]);
}

/* Parses a single command into a command struct */
struct command *parse_command(char *user_input, command_t *cmd) {
    int argc = 0;
    char *token;

/* Calloc allocates the memory and initializes the allocated memory block to zero */
    struct command_t *cmd = calloc(sizeof(struct command_t) + MAX_ARGS * sizeof(char *), 1);

    token = strtok(&user_input, WHITESPACE);

    /* use EXIT_FAILURE -- exit(1) for code portability */
        if (cmd == NULL) {
            fprintf(stderr, "Shell:  Error allocating memory for argument \"%s\": %s\n", return EXIT_FAILURE)
        }

    while (token != NULL && argc < MAX_ARGS) {
        cmd->argv[argc++] = token;
        token = strtok(NULL, WHITESPACE);
    }
    cmd->name = cmd->argv[0];
    cmd->argc = argc;
    return cmd;
}

int shell_commands(const char *filename) {
        int i;
        int pid, numChildren;
        int status;
        FILE *file;
        char cmdLine[MAX_LINE_LEN];
        struct command_t command;

        /* Open a file that contains a set of commands */
        file = fopen(filename, "r");
        if (!file) {
            fprintf(stderr, "launch: Error opening file %s: %s\n", filename, strerror(errno));
            return EXIT_FAILURE;
        } else if (ferror(file)) {
            perror("launch: Error while reading from file.");
            return EXIT_FAILURE;
        }

        /* Process each command in the launch file */
        numChildren = 0;
        pid = fork();
        while (fgets (cmdLine, MAX_LINE_LEN, file) != NULL) {
            parse_command(cmdLine, &command);
            command.argv[command.argc] = NULL;

            /* Create a child process to execute the command */
            if (pid == 0) {
                execv(command.name, command.argv);         /* execute child command */
                fprintf(stderr, "launch: Error executing command '%s': %s\n", command.name, strerror(errno));
                return EXIT_FAILURE;
            } else if (pid < 0) {
                fprintf(stderr, "launch: Error while forking: %s\n", strerror(errno));
                return EXIT_FAILURE;
            }
            numChildren++;      /* Parent continuing to the next command in the file */
        }

        printf("\n\nlaunch: Launched %d commands\n", numChildren);

        /* Should free dynamic storage in command data structure */
        for(i = 0; i < numChildren; i++) {
            pid = wait(&status);

            if (pid < 0) {
                fprintf(stderr, "launch: Error while waiting for child to terminate\n");
                return EXIT_FAILURE;
            } else {
                printf("launch: Child %d terminated successfully\n", pid);
            }
        }
        printf("\n\nlaunch: Terminating successfully\n");
        return EXIT_SUCCESS;
}


void freeCommand(command_t *cmd) {
    int i;
    for (i = 0;((i < cmd->argc) && (cmd->argv[i] != NULL)); i++) {
        free(cmd->argv[i]);
    }
    free(cmd->name);
}


void printPrompt() {

}

void readCommand(char) {

}

int parsePath(char *dirs[]) {

}

char *lookupPath(char **argv, char **dir) {

}