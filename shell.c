/*
 * This is a very minimal shell.  It is only required to find an executable
 * in the PATH, then load it and execute it (using execv).  Since it uses
 * "." (dot) as a separator, it cannot handle file names like "minishell.h"
 * The focus on this exercise is to use fork, PATH variables, and exec.
 * This code can be extended by doing the exercise at the end of Chapter 9.
 *
 */
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <fcntl.h>
#include "minishell.h"


//colors for help info
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_RESET "\x1b[0m"

//global variables
char sysCmdPath[200]="/bin /usr/bin";
char *builtinCommands[]={
    "exit",
    "help",
    "cd",
    "path"
    };

int num_of_builtinCommands=sizeof(builtinCommands)/sizeof(builtinCommands[0]);
char promptString[] = "simple unix shell >";

int main(int argc, const char * argv[]) {
    void runCommand(char **args,char *filename,bool redir);
    char **splitCommand(char *line,char *delim);
    void displayErrorMsg(char *errMsg);
    void readCommand(char *line);
    
    
    //loop until exit command.
    while(1){
        printPrompt();
        
        char *line = NULL;
        size_t len=0;
        getline(&line,&len,stdin);
        
        //process input command.
        readCommand(line);
        
    }
    
    return 0;
}

// print shell prompt
void printPrompt(void) {
    printf("%s ", promptString);
}

//split the command by delim.
char **splitCommand(char *line,char *delim){
    int bufSize=100;
    
    char **args=malloc(bufSize*sizeof(char *));
    char *cmd=NULL;
    
    int i=0;
    cmd=strtok(line,delim);
    
    while(cmd!=NULL){
        args[i]=cmd;
        i++;
        cmd=strtok(NULL,delim);
    }
    args[i]=NULL;
    return args;
}


//show error message.
void displayErrorMsg(char *errMsg){
    if(strcmp(errMsg, "")!=0){
        perror(errMsg);
    }
    char error_message[30] = "An error has occurred\n";
    write(STDERR_FILENO, error_message, strlen(error_message));
}

int exitCommand(char **args) {
    //build in command: exit.
    if(args[1]!=NULL){
        displayErrorMsg("exit cannot have any arguments!");
    }
    else{
        exit(0);
    }
}

void printHelpInformation(void) {
    //build-in command: help.
    int i;
    printf(ANSI_COLOR_GREEN "----------Help--------" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_GREEN "Not all the internal commands are supported." ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_GREEN "Supported internal commands: cd, path, pwd, echo, exit, help " ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_GREEN "Commands can be piped together(more than 1 pipe is allowed also)." ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_GREEN "Ex. ls -a | wc is allowed" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_GREEN "Asyncronous commands are allowed(more than 2 are allowed too) " ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_GREEN "Ex. ls -a & cat file && cat file2 is allowed" ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_GREEN "Output redirection to file is supported: ex. ls > fileOutput " ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_GREEN "Output redirection to file with append is supported: ex. ls >> fileOutput " ANSI_COLOR_RESET "\n");
    printf(ANSI_COLOR_GREEN "Input redirection from file is supported: ex. wc -c < fileInput " ANSI_COLOR_RESET "\n");
    printf("\n");
}

int changeDirectory(char **args) {
    if(args[1]==NULL){
        displayErrorMsg("cd cannot find any argument!");
    }
    else if(args[2]!=NULL){
        displayErrorMsg("cd command can only take one argument!");
    }
    else {
        if(chdir(args[1])!=0){
            displayErrorMsg("simple unix shell error: changeDirectory()");
        }
    }
}

//built in path command
void pathCommand(char **args) {
    //build in command: path
    strcpy(sysCmdPath, "");
    int j=1;
    while(args[j]!=NULL){
        strcat(sysCmdPath, args[j]);
        strcat(sysCmdPath," ");
        j++;
    }
}

//execute the build-in and system commands.
void runCommand(char **args,char *filename,bool redir){
    //build in commands numbers.
    //match build in commands.
    int buildinCmdCode=-1;
    int i;

    for(i=0;i<num_of_builtinCommands;i++){
        if(strcasecmp(builtinCommands[i],args[0])==0){
            buildinCmdCode=i;
            break;
        }
    }
    
    //build in command execution.
    if(buildinCmdCode!=-1){
        switch (buildinCmdCode) {
            case 0:
                exitCommand(args);
                break;
            case 1:
                printHelpInformation();
                break;
            case 2:
                //build in command: cd
                changeDirectory(args);
                break;
            case 3:
                pathCommand(args);
                break;
            default:
                displayErrorMsg("An error has occurred: runCommand()\n");
                break;
        }
    }
    
    //system function execution.
    else{
        int status;
        pid_t pid = fork();

        // child process
        if (pid == 0) {
            //prepare the command  searching path. delimiter -- \t\r\n\a
            char **cmdPaths=splitCommand(sysCmdPath, " \t\r\n\a");
            bool findPath=false;
            
            int i=0;
            char *cmdPath=NULL;
            while(cmdPaths[i]!=NULL){
                cmdPath=(char *)malloc(strlen(cmdPaths[i])+strlen(args[0])+1);
                strcpy(cmdPath, cmdPaths[i]);
                strcat(cmdPath, "/");
                strcat(cmdPath, args[0]);
                
                //check access and execv
                if(access(cmdPath, X_OK)==0){
                    findPath=true;
                    //if not redirection, then execute directly.
                    if(!redir){
                        if(execv(cmdPath, args)==-1){
                            displayErrorMsg("dash error");
                            exit(EXIT_FAILURE);
                        }
                    }
                    break;
                }
                free(cmdPath);
                i++;
            }
            
            //if redirection, output result.
            if(redir){
                int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
                dup2(fd, 1);
                dup2(fd, 2);
                close(fd);
                if(findPath){
                    execv(cmdPath,args);
                }
            }
            
            //error if cannot find command at any path.
            if(!findPath){
                displayErrorMsg("Invalid Command! Please check your input or searching path!");
                exit(EXIT_FAILURE);
            }
        }
        
        // fork error
        else if (pid < 0) {
            displayErrorMsg("dash fork error");
        }
        
        // parent process, wait child to finish.
        else {
            do {
                waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }
}


//check if it is parallel or redirection command. then runCommand
void readCommand(char *line){
    char **readCommands=NULL;
    char **parall=NULL;
    char **args=NULL;
    
    //Parallel check
    parall=splitCommand(line, "&");
    
    int i=0;
    //if not parallel, run once. else run all parallel command.
    while(parall[i]!=NULL){
        args=NULL;
        //Redirection check
        //check redirection command error
        char *redErr=strstr(parall[i], ">>");
        if(redErr){
            printf("%s","Error: Multiple redirection operators!\n");
            displayErrorMsg("");
            break;
        }
        //check redirection command error
        readCommands=splitCommand(parall[i], ">");
        if(readCommands[1]!=NULL && readCommands[2]!=NULL){
            printf("%s","Error: Multiple redirection operators!\n");
            displayErrorMsg("");
            break;
        }
        //REdirection
        else if(readCommands[1]!=NULL){
            char **filename=splitCommand(readCommands[1], " \t\r\n\a");
            //check redirection command error.
            if(filename[1]!=NULL){
                printf("%s","Error: Multiple redirection files!\n");
                displayErrorMsg("");
                break;
            }
            args=NULL;
            args=splitCommand(readCommands[0]," \t\r\n\a");
            if(args[0]!=NULL){
                runCommand(args, filename[0], true);
            }
        }
        //NOT Redirection
        else{
            args=NULL;
            args=splitCommand(readCommands[0]," \t\r\n\a");
            if(args[0]!=NULL){
                runCommand(args, NULL, false);
            }
        }
        i++;
    }
}