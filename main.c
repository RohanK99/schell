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
}

char* read_input() {
    int bufSize = LINE_BUFFER;
    int bufIdx = 0;
    char* buffer = malloc(sizeof(char) * bufSize);
    int c;
    
    if (!buffer) {
        fprintf(stderr, "buffer allocation error in read_input()");
        exit(EXIT_FAILURE);
    }
    
    while (1) {
        c = getchar();
        
        if (c == EOF || c == '\n') {
            buffer[bufIdx] = '\0';
            return buffer;
        } else {
            buffer[bufIdx] = c;
        }
        ++bufIdx;
        
        if (bufIdx >= bufSize) {
            bufSize += LINE_BUFFER;
            buffer = realloc(buffer, bufSize);
            if (!buffer) {
                fprintf(stderr, "buffer reallocation error in read_input()");
                exit(EXIT_FAILURE);
            }
        }
    }
}

int main() {
    poll();
}
