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
#define HISTORY_MAX_LEN 10     /* max number of arguments in the history */
#define PARSE "|\n"
#define FILEPARSE ">"
#define Exit "exit"

#ifdef NULL
#define NULL ...
##endif

struct command_t {          /* struct is used to store a parsed command pipeline */
    char *name;             /* name of the command */
    int argc;               /* number of arguments in the command */
    char *argv[MAX_ARGS];   /* arguments */
};

void printPrompt();
void readCommand(char* commandLine);
void executePipe(command_t* command,int i,int noOfCommands,int** pipes);
void executeFile(command_t* command,int i,char** parsedfile,int** pipes,int noOfCommands);
int parsePipe(char *commandLine,char** parsedpipe);
void parseSpace(char* file_Name_With_LeadingSpace, char** parsedfile);
int parsePath(char *dirs[]);
char* lookupPath(char**argv,char**dirs);
bool checkfile (char* commandLine);
void parseFile(char *commandLine,char** parsedfile);
void freeCommand(command_t *cmd);

