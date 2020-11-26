/* This parser program determines the command name and constructs the parameter list.
This function will build argv[] and set the argc value.
argc is the number of "tokens" or words on the command line
argv[] is an array of strings (pointers to char *).  The last element in argv[]
must be NULL.  As we scan the command line from the left, the first token
goes in argv[0], the second in argv[1], and so on.  each time we add a token to argv[], we increment argc.
*/

/* Constants */
#define MAX_LINE_LEN 80         /* max length of line */
#define MAX_ARGS 64         /* max number of arguments */
#define MAX_ARGS_LEN 16
#define MAX_PATHS 64
#define MAX_PATH_LEN 96
#define WHITESPACE ".,\t\n"
#define HISTORY_MAX_LEN 20     /* max number of arguments in the history */

#ifdef NULL
#define NULL ...
##endif

struct command_t {          /* struct is used to store a parsed command pipeline */
    char *name;             /* name of the command */
    int argc;               /* number of arguments in the command */
    char *argv[MAX_ARGS];   /* arguments */
};

struct command {
    int cmd_count;          /* struct is used to store a command pipeline */
    struct command *cmds[]; /* number of commands */
};

int clear_hst(void);

int parseCommand(char *, struct command_t *);

}

