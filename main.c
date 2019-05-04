#include <stdio.h>  // printf
#include <string.h> // strlen
#include <stdlib.h> // exit
#include <unistd.h> // syscalls

#define LINE_BUFFER 1024

char prompt[] = "schell> "; // command line prompt

char* read_input();

void poll() {
    printf("%s", prompt);
    read_input();
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
