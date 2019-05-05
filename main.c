#include <stdio.h>  // printf
#include <string.h> // strlen
#include <stdlib.h> // exit
#include <unistd.h> // syscalls

#define LINE_BUFFER 1024
#define TOK_DELIM " \t\r\n\a"
#define TOK_BUFSIZE 64

typedef struct Command {
    int argc;
    char** argv;
    enum builtin_t { NONE, QUIT, JOBS, BG, FG } builtin;
} Command;

char prompt[] = "schell> "; // command line prompt

char* read_input();
void tokenize_command(char* input);

void poll() {
    char* input;
    
    printf("%s", prompt);
    input = read_input();
    tokenize_command(input);
}

void tokenize_command(char* input) {
    int bufSize = TOK_BUFSIZE;
    
    Command cmd;
    cmd.argc = 0;
    cmd.argv = malloc(bufSize * sizeof(char*));
    
    char* token;
    token = strtok(input, TOK_DELIM);
    while(token != NULL) {
        cmd.argv[cmd.argc++] = token;
        
        if (cmd.argc >= bufSize) {
            bufSize += TOK_BUFSIZE;
            cmd.argv = realloc(cmd.argv, bufSize);
            if (!cmd.argv) {
                fprintf(stderr, "token buffer reallocation error in tokenize_command()");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, TOK_DELIM);
    }
    cmd.argv[cmd.argc] = NULL; // last entry must always be NULL
 
    cmd.builtin = NONE; // temporarily set cmd.builtin to NONE until builtin commands are implemented
    
    for (int i = 0; i < cmd.argc; ++i){
        printf("%s\n", cmd.argv[i]);
    }
}

char* read_input() {
    char* input = NULL;
    ssize_t bufSize;
    getline(&input, &bufSize, stdin);
    return input;
}

int main() {
    poll();
}
