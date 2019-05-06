#include <stdio.h>  // printf
#include <string.h> // strlen
#include <stdlib.h> // exit
#include <unistd.h> // syscalls
#include <sys/wait.h> // waitpid

#include "builtin.h"

#define TOK_DELIM " \t\r\n\a"
#define TOK_BUFSIZE 64

char prompt[] = "schell> "; // command line prompt

char* read_input();
void tokenize_command(char* input, Command* cmd);
int execute(Command* cmd);
int executeSystemCommand(Command* cmd);

void poll() {
    char* input;
    Command cmd;
    int status;
    
    do {
        printf("%s", prompt);
        input = read_input();
        tokenize_command(input, &cmd);
        status = execute(&cmd);
        
        free(input);
        free(cmd.argv);
    } while (status);
}

int execute(Command* cmd) {
    
    if (cmd->argc == 0) {
        return 1; // empty command, do nothing
    }
    
    for (int i = 0; i < num_builtins(); i++) {
        if (strcmp(cmd->argv[0], builtin_str[i]) == 0) {
            return (*executeBuiltinCommand[i])(cmd);
        }
    }
    
    return executeSystemCommand(cmd);
}

int executeSystemCommand(Command* cmd) {

    pid_t childPid, wPid;
    int status;
    
    if ((childPid = fork()) < 0) {
        perror("fork() error");
    } else if (childPid == 0) { // is child execute command
        if (execvp(cmd->argv[0], cmd->argv) < 0) {
            printf("%s: command not found\n", cmd->argv[0]);
            exit(EXIT_FAILURE);
        }
    } else { // is parent shell continues
        do {
            wPid = waitpid(childPid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    
    return 1;
}

void tokenize_command(char* input, Command* cmd) {
    int bufSize = TOK_BUFSIZE;
    
    cmd->argc = 0;
    cmd->argv = malloc(bufSize * sizeof(char*));
    
    char* token;
    token = strtok(input, TOK_DELIM);
    while(token != NULL) {
        cmd->argv[cmd->argc++] = token;
        
        if (cmd->argc >= bufSize) {
            bufSize += TOK_BUFSIZE;
            cmd->argv = realloc(cmd->argv, bufSize);
            if (!cmd->argv) {
                fprintf(stderr, "token buffer reallocation error in tokenize_command()");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, TOK_DELIM);
    }
    cmd->argv[cmd->argc] = NULL; // last entry must always be NULL
}

char* read_input() {
    char* input = NULL;
    size_t bufSize;
    getline(&input, &bufSize, stdin);
    return input;
}

int main() {
    poll();
}
