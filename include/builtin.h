#include "command.h"

#define UNUSED(x) (void)(x)

int schell_cd(Command* cmd);
int schell_exit(Command* cmd);

char* builtin_str[] = {
    "cd",
    "exit"
};

int (*executeBuiltinCommand[])(Command*) = {
    &schell_cd,
    &schell_exit
};

int num_builtins() {
    return (sizeof(builtin_str) / sizeof(char*));
}

int schell_cd(Command* cmd) {
    if (cmd->argv[1] == NULL) {
        fprintf(stderr, "schell: expected argument to \"cd\"\n");
    } else {
        if (chdir(cmd->argv[1]) != 0) {
            perror("schell");
        }
    }
    return 1;
}

int schell_exit(Command* cmd) {
    UNUSED(cmd);
    return 0;
}
