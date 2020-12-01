#define FALSE    0
#define TRUE     1
#define LINE_LEN    100
#define MAX_ARGS    64
#define MAX_ARG_LEN    16
#define MAX_PATHS    64
#define MAX_PATH_LEN    96
#define READ_COMMAND_BUFFSIZE 1024
#define TOKEN_DELIMITER " \t\r\n\a"
#define INPUT 0
#define OUTPUT 0
#define APPEND 2
#define WHITESPACE    " .,\t"
#ifndef NULL
#endif


struct command_t {
  char *name;
  int argc;
  char *argv[MAX_ARGS];
};

int no_reprint_prmpt;

void signalHandler(int p);